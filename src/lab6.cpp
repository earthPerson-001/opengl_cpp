/**
 *
 * 1. Implement the following 3D transformations using the 3D shapes provided by Opengl:

        Translation
        Rotation
        Shearing
        Scaling

 * 2.  Implement the orthographic Projection

*/

#include <inttypes.h>
#include <utility>
#include <vector>
#include <cmath>

#include <glad/glad.h>

#include "lab6.hpp"
#include "globals.hpp"

enum THREE_D_TRANSFORMATIONS
{
    NONE = 0,
    TRANSLATION,
    ROTATION,
    SCALING,
};

typedef struct Point3D
{
    double x, y, z;
} Point3D;

/**
 * The vertices order are from bottom left corner, anticlockwise, first front face then back face
 *
 */
void render_cube(const std::vector<Point3D> &vertices, const GLfloat color[3] = COLOR_RED, GLfloat line_color[3] = (GLfloat*)COLOR_BLACK)
{
    // the cube
    glColor3fv(color);

    // first four points forms a quadrilateral // front
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++)
    {
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
    }
    glEnd();

    // other four points forms another quadrilateral // back
    glBegin(GL_QUADS);
    for (int i = 4; i < 8; i++)
    {
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
    }
    glEnd();

    // left side
    glBegin(GL_QUADS);
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
    glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glEnd();

    // right side
    glBegin(GL_QUADS);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
    glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glEnd();

    // top side
    glBegin(GL_QUADS);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
    glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glEnd();

    // bottom side
    glBegin(GL_QUADS);
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
    glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
    glEnd();

    // the lines
    glColor3fv(line_color);
    glBegin(GL_LINE_STRIP);
    for (const Point3D &p : vertices)
    {
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();

    // connecting the faces
    glBegin(GL_LINES);
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
    glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
    glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
    glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
    glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
    glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
    glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
    glEnd();
}

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

bool perform_3d_translation(THREE_D_TRANSFORMATIONS transformation, GLfloat translation_params[3], GLfloat points[3], GLfloat ret_out[16])
{
    GLfloat matrix_a[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

    switch (transformation)
    {
    case THREE_D_TRANSFORMATIONS::TRANSLATION:
        matrix_a[3] = translation_params[0];
        matrix_a[7] = translation_params[1];
        matrix_a[11] = translation_params[2];
        break;
    case THREE_D_TRANSFORMATIONS::SCALING:
        matrix_a[0] = translation_params[0];
        matrix_a[5] = translation_params[1];
        matrix_a[10] = translation_params[2];
        break;
    case THREE_D_TRANSFORMATIONS::ROTATION:
        break;
    case THREE_D_TRANSFORMATIONS::NONE:
    default:
        break;
    }

    GLfloat matrix_b[4] = {points[0], points[1], points[2], 1};

    GLfloat out[4];

    bool multiply_success = matrix_multiplication(matrix_a, matrix_b, 4, 4, 4, 1, out);

    ret_out[0] = out[0];
    ret_out[1] = out[1];
    ret_out[2] = out[2];

    return multiply_success;

}

void run_lab6()
{
    // drawing the grid lines
    glColor3f(0, 1, 1);
    glBegin(GL_LINES);
    for (int i = 0; i <= 10; i++)
    {
        // horizontal
        glVertex3f(-50.0f + i * 10.0f, -50.0f, 0.0f);
        glVertex3f(-50.0f + i * 10.0f, 50.0f, 0.0f);

        // vertical
        glVertex3f(-50.0f, -50.0f + i * 10.0f, 0.0f);
        glVertex3f(50.0f, -50.0f + i * 10.0f, 0.0f);
    }
    glEnd();

    std::vector<Point3D> cube_vertices = {
        {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, // front face
        {-0.5, -0.5, 0.5},
        {0.5, -0.5, 0.5},
        {0.5, 0.5, 0.5},
        {-0.5, 0.5, 0.5}, // back face
    };

    render_cube(cube_vertices);

    std::vector<Point3D>  out_vertices;

    THREE_D_TRANSFORMATIONS transformation_type = THREE_D_TRANSFORMATIONS::TRANSLATION;
    GLfloat translate_by[3] = {1.0, -1.5, -1.5};

    int max_len = cube_vertices.size();
    for (int i = 0; i < max_len; i++)
    {
        GLfloat vertex[3] = {cube_vertices[i].x, cube_vertices[i].y, cube_vertices[i].z};
        GLfloat out_vertex[3];
        perform_3d_translation(transformation_type, translate_by, vertex, out_vertex);

        out_vertices.push_back({out_vertex[0], out_vertex[1], out_vertex[2]});
    }

    render_cube(out_vertices, COLOR_GREEN, (GLfloat*)COLOR_WHITE);

    std::vector<Point3D>  scaling_out_vertices;

    transformation_type = THREE_D_TRANSFORMATIONS::SCALING;
    GLfloat scale_by[3] = {1.0, -1.5, -1.5};

    for (int i = 0; i < max_len; i++)
    {
        GLfloat vertex[3] = {cube_vertices[i].x, cube_vertices[i].y, cube_vertices[i].z};
        GLfloat out_vertex[3];
        perform_3d_translation(transformation_type, scale_by, vertex, out_vertex);

        scaling_out_vertices.push_back({out_vertex[0], out_vertex[1], out_vertex[2]});
    }

    render_cube(scaling_out_vertices, COLOR_BLUE, (GLfloat*)COLOR_YELLOW);
}
