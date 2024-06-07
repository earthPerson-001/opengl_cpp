/**
1. Write a Program to implement:
    2D Translation
    2D Rotation
    2D Scaling
    2D Reflection
    2D Shearing
    Composite Transformation (Should be able to perform atleast 3 transformations)
    (For doing these Transformations consider any 2D shapes (Line, Triangle, Rectangle etc), and use Homogeneous coordinate Systems)
*/

#include <array>
#include <cmath>
#include <iostream>

#include <glad/glad.h>

#include "globals.hpp"
#include "lab4.hpp"

enum TWO_D_TRANSFORMATIONS
{
    SIMPLE_NONE = 0,
    TRANSLATION,
    ROTATION,
    SCALING,
    REFLECTION,
    SHEARING
};

enum COMPOSITE_TWO_D_TRANSFORMATIONS {
    COMPOSITE_NONE = 0,
    REFLECT_ABOUT_45_DEGREE_LINE,
    SCALE_ABOUT_A_POINT,
    ROTATE_ABOUT_A_POINT
};

/**
 * out = A * B
 * Returns  true on success and false on failure
 *
 */
static bool matrix_multiplication(GLfloat *A, GLfloat *B, size_t row_A, size_t column_A, size_t row_B, size_t column_B, GLfloat *out)
{
    if (column_A != row_B) // cannot perform multiplication in this case
    {
        return false;
    }

    for (int i = 0; i < row_A; i++)
    { // repeating for number of rows in the first matrix
        for (int j = 0; j < column_B; j++)
        { // for repeating in number of columns
            GLfloat sum = 0;
            for (int k = 0; k < row_B; k++)
            { // repeating for each row elemenet of the column
                sum += A[i * column_A + k] * B[k * column_B + j];
            }
            out[i * column_B + j] = sum;
        }
    }

    return true;
}

bool perform_2d_transformation(TWO_D_TRANSFORMATIONS transformation, GLfloat translate_by[2], GLfloat input_points[2], GLfloat output_points[2])
{
    GLfloat matrix_a[9] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1};

    GLfloat cos_theta = cos(translate_by[0] * M_PI / 180);
    GLfloat sin_theta = sin(translate_by[0] * M_PI / 180);

    switch (transformation)
    {
    case TRANSLATION:
        matrix_a[2] = translate_by[0];
        matrix_a[5] = translate_by[1];
        break;
    case ROTATION:
        matrix_a[0] = cos_theta;
        matrix_a[1] = -sin_theta;
        matrix_a[3] = sin_theta;
        matrix_a[4] = cos_theta;
        break;
    case SCALING:
        matrix_a[0] = translate_by[0];
        matrix_a[4] = translate_by[1];
        break;
    case REFLECTION:         // if both is given, translate about is origin is done
        if (translate_by[0]) // about x-axis
        {
            matrix_a[4] = -1;
        }
        if (translate_by[1]) // about y-axis
        {
            matrix_a[0] = -1;
        }
        break;

    case SHEARING: // if both is given, shearing about both the axes is done

        matrix_a[1] = translate_by[0]; // x' = x + shx.y
        matrix_a[3] = translate_by[1]; // y = y + shy.x

        break;
    default:
        return false;
        break;
    }

    GLfloat matrix_b[3] = {input_points[0], input_points[1], 1};

    GLfloat out[9];

    if (!matrix_multiplication(matrix_a, matrix_b, 3, 3, 3, 1, out))
    {
        return false;
    }
    else
    {
        output_points[0] = out[0];
        output_points[1] = out[1];
    }

    return true;
}

// scale about an arbitrary point
bool scale_about_a_point(GLfloat scale_by[2], GLfloat reference_point[2], GLfloat input_points[2], GLfloat output_points[2])
{

    bool ret_val = true;

    GLfloat out[9];

    GLfloat translation_matrix[9] = {
        1, 0, -reference_point[0],
        0, 1, -reference_point[1],
        0, 0, 1};

    GLfloat scaling_matrix[9] = {
        scale_by[0], 0, 0,
        0, scale_by[1], 0,
        0, 0, 1};

    GLfloat inverse_translation_matrix[9] = {
        1, 0, reference_point[0],
        0, 1, reference_point[1],
        0, 0, 1};

    // determine composite transformation matrix
    ret_val &= matrix_multiplication(inverse_translation_matrix, scaling_matrix, 3, 3, 3, 3, out);
    GLfloat another_out[9];
    ret_val &= matrix_multiplication(out, translation_matrix, 3, 3, 3, 3, another_out);

    // the final output of the point
    GLfloat final_output[3];
    GLfloat homogeneous_points[3] = {input_points[0], input_points[1], 1};
    ret_val &= matrix_multiplication(another_out, homogeneous_points, 3, 3, 3, 1, final_output);

    output_points[0] = final_output[0];
    output_points[1] = final_output[1];

    return ret_val;
}

bool rotate_about_a_point(GLfloat rotate_by[2], GLfloat reference_point[2], GLfloat input_points[2], GLfloat output_points[2])
{

    bool ret_val = true;

    GLfloat cos_theta = cos(rotate_by[0] * M_PI / 180);
    GLfloat sin_theta = sin(rotate_by[0] * M_PI / 180);

    GLfloat out[9];

    GLfloat translation_matrix[9] = {
        1, 0, -reference_point[0],
        0, 1, -reference_point[1],
        0, 0, 1};

    GLfloat rotation_matrix[9] = {
        cos_theta, -sin_theta, 0,
        sin_theta, cos_theta, 0,
        0, 0, 1};

    GLfloat inverse_translation_matrix[9] = {
        1, 0, reference_point[0],
        0, 1, reference_point[1],
        0, 0, 1};

    // determine composite transformation matrix
    ret_val &= matrix_multiplication(inverse_translation_matrix, rotation_matrix, 3, 3, 3, 3, out);
    GLfloat another_out[9];
    ret_val &= matrix_multiplication(out, translation_matrix, 3, 3, 3, 3, another_out);

    // the final output of the point
    GLfloat final_output[3];
    GLfloat homogeneous_points[3] = {input_points[0], input_points[1], 1};
    ret_val &= matrix_multiplication(another_out, homogeneous_points, 3, 3, 3, 1, final_output);

    output_points[0] = final_output[0];
    output_points[1] = final_output[1];

    return ret_val;
}

bool reflect_about_45_degree_line(GLfloat input_points[2], GLfloat output_points[2])
{

    bool ret_val = true;

    GLfloat line_angle = 45;

    GLfloat cos_theta = cos(line_angle * M_PI / 180);
    GLfloat sin_theta = sin(line_angle * M_PI / 180);

    GLfloat out[9];

    // rotate the line y=x to y-axis
    GLfloat rotation_matrix[9] = {
        cos_theta, -sin_theta, 0,
        sin_theta, cos_theta, 0,
        0, 0, 1};

    // reflection about y-axis
    GLfloat reflection_matrix[9] = {
        -1, 0, 0,
        0, 1, 0,
        0, 0, 1};

    // rotate back the line
    GLfloat inv_rotation_matrix[9] = {
        cos_theta, sin_theta, 0,
        -sin_theta, cos_theta, 0,
        0, 0, 1};

    // determine composite transformation matrix
    ret_val &= matrix_multiplication(inv_rotation_matrix, reflection_matrix, 3, 3, 3, 3, out);
    GLfloat another_out[9];
    ret_val &= matrix_multiplication(out, rotation_matrix, 3, 3, 3, 3, another_out);

    // the final output of the point
    GLfloat final_output[3];
    GLfloat homogeneous_points[3] = {input_points[0], input_points[1], 1};
    ret_val &= matrix_multiplication(another_out, homogeneous_points, 3, 3, 3, 1, final_output);

    output_points[0] = final_output[0];
    output_points[1] = final_output[1];

    return ret_val;
}

void run_demo()
{

    GLfloat A[] = {0, 0};
    GLfloat B[] = {1, 1};

    // using original points
    glBegin(GL_LINES);
    glColor3fv(COLOR_GREEN);
    glVertex2fv((GLfloat*)A);
    glVertex2fv((GLfloat*)B);
    glEnd();

    // performing translation
    GLfloat translate_by[2] = {-1, -1};

    GLfloat A_prime[2];
    GLfloat B_prime[2];

    perform_2d_transformation(TWO_D_TRANSFORMATIONS::TRANSLATION, translate_by, A, A_prime);
    perform_2d_transformation(TWO_D_TRANSFORMATIONS::TRANSLATION, translate_by, B, B_prime);

    // using translated points
    glBegin(GL_LINES);
    glColor3fv(COLOR_BLUE);
    glVertex2fv((GLfloat*)A_prime);
    glVertex2fv((GLfloat*)B_prime);
    glEnd();

    // performing rotation
    GLfloat rotation_angle[2] = {30, 0}; // in degrees (anticlockwise)
    GLfloat A_double_prime[2];
    GLfloat B_double_prime[2];

    perform_2d_transformation(TWO_D_TRANSFORMATIONS::ROTATION, rotation_angle, A_prime, A_double_prime);
    perform_2d_transformation(TWO_D_TRANSFORMATIONS::ROTATION, rotation_angle, B_prime, B_double_prime);

    // using translated points
    glBegin(GL_LINES);
    glColor3fv(COLOR_BLACK);
    glVertex2fv((GLfloat*)A_double_prime);
    glVertex2fv((GLfloat*)B_double_prime);
    glEnd();

    // demonstrating scaling using a triangle
    GLfloat C[] = {-0.5, -0.5};
    GLfloat D[] = {0.5, -0.5};
    GLfloat E[] = {0, 0.5};

    glBegin(GL_TRIANGLES);
    glVertex3fv((GLfloat*)C);
    glVertex3fv((GLfloat*)D);
    glVertex3fv((GLfloat*)E);
    glEnd();

    GLfloat C_prime[2];
    GLfloat D_prime[2];
    GLfloat E_prime[2];

    // performing scaling
    GLfloat scaling_coefficients[2] = {0.5, 0.5};
    perform_2d_transformation(TWO_D_TRANSFORMATIONS::SCALING, scaling_coefficients, C, C_prime);
    perform_2d_transformation(TWO_D_TRANSFORMATIONS::SCALING, scaling_coefficients, D, D_prime);
    perform_2d_transformation(TWO_D_TRANSFORMATIONS::SCALING, scaling_coefficients, E, E_prime);

    glColor3fv(COLOR_RED);
    glBegin(GL_TRIANGLES);
    glVertex3fv((GLfloat*)C_prime);
    glVertex3fv((GLfloat*)D_prime);
    glVertex3fv((GLfloat*)E_prime);
    glEnd();

    // performing reflection
    GLfloat F[] = {-0.5, -0.5};
    GLfloat G[] = {-0.1, -0.5};
    GLfloat H[] = {-0.3, 0.5};

    glColor3fv(COLOR_BLUE);
    glBegin(GL_TRIANGLES);
    glVertex3fv((GLfloat*)F);
    glVertex3fv((GLfloat*)G);
    glVertex3fv((GLfloat*)H);
    glEnd();

    GLfloat F_prime[2];
    GLfloat G_prime[2];
    GLfloat H_prime[2];

    GLfloat reflection_about[2] = {0, 1}; // about y-axis

    perform_2d_transformation(TWO_D_TRANSFORMATIONS::REFLECTION, reflection_about, F, F_prime);
    perform_2d_transformation(TWO_D_TRANSFORMATIONS::REFLECTION, reflection_about, G, G_prime);
    perform_2d_transformation(TWO_D_TRANSFORMATIONS::REFLECTION, reflection_about, H, H_prime);

    glColor3fv(COLOR_GREEN);
    glBegin(GL_TRIANGLES);
    glVertex3fv((GLfloat*)F_prime);
    glVertex3fv((GLfloat*)G_prime);
    glVertex3fv((GLfloat*)H_prime);
    glEnd();

    // performing shearing using a square
    GLfloat I[] = {0, 0};
    GLfloat J[] = {0.5, 0};
    GLfloat K[] = {0.5, 0.5};
    GLfloat L[] = {0, 0.5};

    glColor3fv(COLOR_BLUE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv((GLfloat*)I);
    glVertex2fv((GLfloat*)J);
    glVertex2fv((GLfloat*)K);
    glVertex2fv((GLfloat*)L);
    glVertex2fv((GLfloat*)I);
    glEnd();

    GLfloat I_prime[2];
    GLfloat J_prime[2];
    GLfloat K_prime[2];
    GLfloat L_prime[2];

    GLfloat shearing_factor[] = {1, 0};

    perform_2d_transformation(TWO_D_TRANSFORMATIONS::SHEARING, shearing_factor, I, I_prime);
    perform_2d_transformation(TWO_D_TRANSFORMATIONS::SHEARING, shearing_factor, J, J_prime);
    perform_2d_transformation(TWO_D_TRANSFORMATIONS::SHEARING, shearing_factor, K, K_prime);
    perform_2d_transformation(TWO_D_TRANSFORMATIONS::SHEARING, shearing_factor, L, L_prime);

    glColor3fv(COLOR_YELLOW);
    glBegin(GL_LINE_STRIP);
    glVertex2fv((GLfloat*)I_prime);
    glVertex2fv((GLfloat*)J_prime);
    glVertex2fv((GLfloat*)K_prime);
    glVertex2fv((GLfloat*)L_prime);
    glVertex2fv((GLfloat*)I_prime);
    glEnd();

    // composite transformation
    // reflection about line y=x
    GLfloat M[] = {-0.99, -0.9};
    GLfloat N[] = {-0.92, -0.9};
    GLfloat O[] = {-0.97, -0.6};

    glColor3fv(COLOR_GREEN);
    glBegin(GL_TRIANGLES);
    glVertex3fv(M);
    glVertex3fv(N);
    glVertex3fv(O);
    glEnd();

    GLfloat M_prime[2], N_prime[2], O_prime[2];

    reflect_about_45_degree_line(M, M_prime);
    reflect_about_45_degree_line(N, N_prime);
    reflect_about_45_degree_line(O, O_prime);

    glColor3fv(COLOR_RED);
    glBegin(GL_TRIANGLES);
    glVertex3fv(M_prime);
    glVertex3fv(N_prime);
    glVertex3fv(O_prime);
    glEnd();

    // scaling the triangle about the centroid
    GLfloat scale_by[] = {0.5, 0.25};
    GLfloat centroid[] = {(M[0] + N[0] + O[0]) / 3, (M[1] + N[1] + O[1]) / 3};
    scale_about_a_point(scale_by, centroid, M, M_prime);
    scale_about_a_point(scale_by, centroid, N, N_prime);
    scale_about_a_point(scale_by, centroid, O, O_prime);

    glColor3fv(COLOR_BLACK);
    glBegin(GL_TRIANGLES);
    glVertex3fv(M_prime);
    glVertex3fv(N_prime);
    glVertex3fv(O_prime);
    glEnd();

    // rotation about a point
    GLfloat P[] = {0.99, -0.9};
    GLfloat Q[] = {0.92, -0.9};
    GLfloat R[] = {0.97, -0.6};

    glColor3fv(COLOR_GREEN);
    glBegin(GL_TRIANGLES);
    glVertex3fv(P);
    glVertex3fv(Q);
    glVertex3fv(R);
    glEnd();

    // rotate 30 degrees about (1, -1)
    GLfloat rotate_by[] = {30, 0};
    GLfloat reference_point[] = {1, -1};
    GLfloat P_prime[2], Q_prime[2], R_prime[2];
    rotate_about_a_point(rotate_by, reference_point, P, P_prime);
    rotate_about_a_point(rotate_by, reference_point, Q, Q_prime);
    rotate_about_a_point(rotate_by, reference_point, R, R_prime);

    glColor3fv(COLOR_RED);
    glBegin(GL_TRIANGLES);
    glVertex3fv(P_prime);
    glVertex3fv(Q_prime);
    glVertex3fv(R_prime);
    glEnd();
}

TWO_D_TRANSFORMATIONS g_current_transformation = TWO_D_TRANSFORMATIONS::SIMPLE_NONE;
COMPOSITE_TWO_D_TRANSFORMATIONS g_current_composite_transformation = COMPOSITE_TWO_D_TRANSFORMATIONS::COMPOSITE_NONE;

GLfloat g_translate_by[2] = {0, 0};
GLfloat g_reference_point[2] = {0, 0};

GLfloat g_point_A[] = {0, 0};
GLfloat g_point_B[] = {0, 0};
GLfloat g_point_C[] = {0, 0};

void take_input_from_menu()
{
    int input = 1; // for taking menu input

    std::cout << "\n\nThe program demonstrates 2D Transformations on a traingle.\n\n";
    std::cout << "Enter the coordinates for the traingle (counter clockwise order):\n";
    std::cout << "\tPoint A: ";
    std::cin >> g_point_A[0] >> g_point_A[1];
    std::cout << "\tPoint B: ";
    std::cin >> g_point_B[0] >> g_point_B[1];
    std::cout << "\tPoint C: ";
    std::cin >> g_point_C[0] >> g_point_C[1];

    std::cout << "Please select an Operation: \
    \n\t1) Simple Transformation \
    \n\t2) Composite Transformation \
    \n\t";
    std::cin >> input;

    switch (input)
    {
    default:
        std::cout << "Invalid operation selected, performing simple transformation.\n";
    case 1:
        g_current_composite_transformation = COMPOSITE_TWO_D_TRANSFORMATIONS::COMPOSITE_NONE;
        std::cout << "Please select a type of Simple Transformation:\
        \n\t1) 2D Translation \
        \n\t2) 2D Rotation \
        \n\t3) 2D Scaling \
        \n\t4) 2D Reflection \
        \n\t5) 2D Shearing \
        \n\t";
        std::cin >> input;

        switch (input)
        {
        default:
            std::cout << "Invalid operation selected, performing 2d translation.\n";
            g_current_transformation = TWO_D_TRANSFORMATIONS::TRANSLATION;
        case 1:
            std::cout << "You've Selected 2D Translation.\n";
            g_current_transformation = TWO_D_TRANSFORMATIONS::TRANSLATION;
            std::cout << "Enter the translation parameters(seperated by space): [tx ty]: ";
            std::cin >> g_translate_by[0] >> g_translate_by[1];
            break;
        case 2:
            std::cout << "You've Selected 2D Rotation.\n";
            g_current_transformation = TWO_D_TRANSFORMATIONS::ROTATION;
            std::cout << "Enter the rotation angle theta (in degrees): ";
            std::cin >> g_translate_by[0];
            break;
        case 3:
            std::cout << "You've Selected 2D Scaling.\n";
            g_current_transformation = TWO_D_TRANSFORMATIONS::SCALING;
            std::cout << "Enter the scaling parameters(seperated by space): [sx sy]: ";
            std::cin >> g_translate_by[0] >> g_translate_by[1];
            break;
        case 4:
            std::cout << "You've Selected 2D Reflection.\n";
            g_current_transformation = TWO_D_TRANSFORMATIONS::REFLECTION;
            std::cout << "Enter the reflection parameters: \n\t1) X-axis \n\t2) Y-axis \n\t3) Origin \n\t";
            std::cin >> input;

            switch (input)
            {
            case 1:
            default:
                g_translate_by[0] = 1;
                g_translate_by[1] = 0;
                break;
            case 2:
                g_translate_by[0] = 0;
                g_translate_by[1] = 1;
                break;
            case 3:
                g_translate_by[0] = 1;
                g_translate_by[1] = 1;
                break;
            }

            break;
        case 5:
            std::cout << "You've Selected 2D Shearing.\n";
            g_current_transformation = TWO_D_TRANSFORMATIONS::SHEARING;
            std::cout << "Enter the shearing parameters(seperated by space): [Shx Shy]: ";
            std::cin >> g_translate_by[0] >> g_translate_by[1];
            break;
        }

        break;
    case 2:    
        g_current_transformation = TWO_D_TRANSFORMATIONS::SIMPLE_NONE;

        std::cout << "Please select a type of Composite Transformation:\
        \n\t1) Rotate about a reference point \
        \n\t2) Scale about a reference point\
        \n\t3) Reflect about the line y=x \
        \n\t";
        std::cin >> input;

        switch (input)
        {
        default:
            std::cout << "Invalid operation selected, performing rotation about the reference point.\n";
        case 1:
            g_current_composite_transformation = COMPOSITE_TWO_D_TRANSFORMATIONS::ROTATE_ABOUT_A_POINT;
            std::cout << "Enter the reference point co-ordinate separated by space (Xr, Yr): ";
            std::cin >> g_reference_point[0] >> g_reference_point[1];
            std::cout << "Enter the rotation angle (in degrees): ";
            std::cin >> g_translate_by[0];
            break;
        case 2:
            g_current_composite_transformation = COMPOSITE_TWO_D_TRANSFORMATIONS::SCALE_ABOUT_A_POINT;
            std::cout << "Enter the reference point co-ordinate separated by space (Xr, Yr): ";
            std::cin >> g_reference_point[0] >> g_reference_point[1];
            std::cout << "Enter the scaling parameters separated by space (Sx, Sy): ";
            std::cin >> g_translate_by[0] >> g_translate_by[1];
            break;
        case 3:
            g_current_composite_transformation = COMPOSITE_TWO_D_TRANSFORMATIONS::REFLECT_ABOUT_45_DEGREE_LINE;
            break;
        }
        break;

    }
}

void display_triangle(const GLfloat color[3], const GLfloat pt_A[2], const GLfloat pt_B[2], const GLfloat pt_C[2]) 
{
    glColor3fv(color);
    glBegin(GL_TRIANGLES);
    glVertex2fv((GLfloat*)pt_A);
    glVertex2fv((GLfloat*)pt_B);
    glVertex2fv((GLfloat*)pt_C);
    glEnd();
}

void run_from_given_input()
{
    GLfloat A_prime[2], B_prime[2], C_prime[2];

    // displaying the coordinate system
    glBegin(GL_LINES);
    glVertex2f(0, 1);
    glVertex2f(0, -1);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(1, 0);
    glVertex2f(-1, 0);
    glEnd();


    // previsous triangle
    display_triangle(COLOR_GREEN, g_point_A, g_point_B, g_point_C);

    switch (g_current_transformation) {
        case TWO_D_TRANSFORMATIONS::SIMPLE_NONE: // composite transformation
            switch (g_current_composite_transformation)
            {
            case COMPOSITE_TWO_D_TRANSFORMATIONS::COMPOSITE_NONE:
                break;
            case COMPOSITE_TWO_D_TRANSFORMATIONS::REFLECT_ABOUT_45_DEGREE_LINE:
                reflect_about_45_degree_line((GLfloat*)g_point_A, (GLfloat*)A_prime);
                reflect_about_45_degree_line((GLfloat*)g_point_B, (GLfloat*)B_prime);
                reflect_about_45_degree_line((GLfloat*)g_point_C, (GLfloat*)C_prime);
                break;
            case COMPOSITE_TWO_D_TRANSFORMATIONS::ROTATE_ABOUT_A_POINT:
                rotate_about_a_point((GLfloat*)g_translate_by, (GLfloat*)g_reference_point,(GLfloat*)g_point_A, (GLfloat*)A_prime);
                rotate_about_a_point((GLfloat*)g_translate_by, (GLfloat*)g_reference_point,(GLfloat*)g_point_B, (GLfloat*)B_prime);
                rotate_about_a_point((GLfloat*)g_translate_by, (GLfloat*)g_reference_point,(GLfloat*)g_point_C, (GLfloat*)C_prime);
                break;
            case COMPOSITE_TWO_D_TRANSFORMATIONS::SCALE_ABOUT_A_POINT:
                scale_about_a_point((GLfloat*)g_translate_by, (GLfloat*)g_reference_point,(GLfloat*)g_point_A, (GLfloat*)A_prime);
                scale_about_a_point((GLfloat*)g_translate_by, (GLfloat*)g_reference_point,(GLfloat*)g_point_B, (GLfloat*)B_prime);
                scale_about_a_point((GLfloat*)g_translate_by, (GLfloat*)g_reference_point,(GLfloat*)g_point_C, (GLfloat*)C_prime);
                break;
            default:
                // error handling
                break;
            }
            break;
        case TWO_D_TRANSFORMATIONS::TRANSLATION:
        case TWO_D_TRANSFORMATIONS::ROTATION:
        case TWO_D_TRANSFORMATIONS::REFLECTION:
        case TWO_D_TRANSFORMATIONS::SHEARING:
        case TWO_D_TRANSFORMATIONS::SCALING:
            // transformation
            perform_2d_transformation(g_current_transformation, g_translate_by, g_point_A, A_prime);
            perform_2d_transformation(g_current_transformation, g_translate_by, g_point_B, B_prime);
            perform_2d_transformation(g_current_transformation, g_translate_by, g_point_C, C_prime);
        default:
            // error handling
            break;
    }


    // new traingle
    display_triangle(COLOR_RED, A_prime, B_prime, C_prime);
}

void run_lab4()
{

    // run_demo();

    run_from_given_input();
}