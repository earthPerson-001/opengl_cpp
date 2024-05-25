/**
 1. Write a Program to implement mid- point Circle Drawing Algorithm
 2. Write a Program to implement mid- point Ellipse Drawing Algorithm
 3. Write a program to implement circle drawing algorithm using Polar Coordinate system

*/

#include <array>
#include <cmath>

#include "globals.hpp"
#include "lab3.hpp"

static std::array<GLfloat, 2> device_specific_to_normalized(GLfloat x, GLfloat y)
{
    return {
        2 * (x / SCR_WIDTH) - 1,
        2 * (y / SCR_HEIGHT) - 1};
}

static void plot_wrt_center(GLfloat center[2], std::array<GLfloat, 2> point, GLfloat color[3] = (GLfloat *)COLOR_RED)
{

    std::array<GLfloat, 2> new_point = device_specific_to_normalized(center[0] + point[0], center[1] + point[1]);

    glPointSize(5);
    glColor3fv(color);
    glBegin(GL_POINTS);
    glVertex2fv(new_point.data());
    glEnd();
}

/**
 * For circle
 *
 */
void plot_point_with_symmetry_first_45_degrees(GLfloat center[2], std::array<GLfloat, 2> point, GLfloat color[3] = (GLfloat *)COLOR_RED)
{
    plot_wrt_center(center, {point[0], point[1]}, color);
    plot_wrt_center(center, {-point[0], point[1]}, color);
    plot_wrt_center(center, {-point[0], -point[1]}, color);
    plot_wrt_center(center, {point[0], -point[1]}, color);
    plot_wrt_center(center, {point[1], point[0]}, color);
    plot_wrt_center(center, {-point[1], point[0]}, color);
    plot_wrt_center(center, {-point[1], -point[0]}, color);
    plot_wrt_center(center, {point[1], -point[0]}, color);
}

void draw_circle_using_midpoint_circle_drawing_algorithm(GLfloat center[2], GLfloat radius)
{
    GLfloat x, y;

    // initial point
    x = 0, y = radius;

    // initial decision parameter
    GLfloat p_k = 5 / 4 - radius;

    while (x < y)
    {
        plot_point_with_symmetry_first_45_degrees(center, {x, y}, (GLfloat *)COLOR_GREEN);

        if (p_k < 0)
        {
            x = x + 1;
            p_k = p_k + 2 * x + 1; // here Xk is the updated value i.e. Xk + 1
        }
        else
        {
            x = x + 1;
            y = y - 1;
            p_k = p_k + 2 * x + 1 - 2 * y; // both Xk and Yk are already updated i.e. Xk + 1 and Yk -1
        }
    }
}

/**
 * For Ellipse
 */
void plot_point_with_symmetry_first_90_degrees(GLfloat center[2], std::array<GLfloat, 2> point, GLfloat color[3] = (GLfloat *)COLOR_RED)
{
    plot_wrt_center(center, {point[0], point[1]}, color);
    plot_wrt_center(center, {-point[0], point[1]}, color);
    plot_wrt_center(center, {-point[0], -point[1]}, color);
    plot_wrt_center(center, {point[0], -point[1]}, color);
}

void draw_ellipse_using_midpoint_ellipse_drawing_algorithm(GLfloat center[2], GLfloat radius_x, GLfloat radius_y)
{
    GLfloat x, y;
    double radius_x_sq = pow(radius_x, 2);
    double radius_y_sq = pow(radius_y, 2);

    // for region 1: |m| < 1

    // initial point for the region 1
    x = 0, y = radius_y; // starting from (0, ry)

    // initial decision parameter
    double p1_k = radius_y_sq - radius_x_sq * radius_y + 0.25 * radius_x_sq;

    while ((2 * radius_y_sq * x) < (2 * radius_x_sq * y))
    {
        plot_point_with_symmetry_first_90_degrees(center, {x, y}, (GLfloat *)COLOR_BLUE);
        if (p1_k < 0)
        {
            x = x + 1;
            p1_k = p1_k + 2 * radius_y_sq * x + radius_y_sq; // x is already incremented
        }
        else
        {
            x = x + 1;
            y = y - 1;
            p1_k = p1_k + 2 * radius_y_sq * x - 2 * radius_x_sq * y + radius_y_sq;
        }
    }

    // for region 2: |m| > 1
    double p2_k = radius_y_sq * pow(x + 0.5, 2) + radius_x_sq * pow(y - 1, 2) - radius_x_sq * radius_y_sq;

    while (y > 0)
    {
        plot_point_with_symmetry_first_90_degrees(center, {x, y}, (GLfloat *)COLOR_BLACK);

        if (p2_k <= 0)
        {
            x = x + 1;
            y = y - 1;
            p2_k = p2_k + 2 * radius_y_sq * x - 2 * radius_x_sq * y + radius_x_sq;
        }
        else
        {
            y = y - 1;
            p2_k = p2_k - 2 * radius_x_sq * y + radius_x_sq;
        }
    }
}

void draw_circle_using_polar_coordinates(GLfloat center[2], GLfloat radius, GLfloat color[3] = (GLfloat *)COLOR_YELLOW)
{
    glColor3fv(color);

    glLineWidth(5);

    for (double i = 0; i <= 45; i += 1 / radius)
    {
        plot_point_with_symmetry_first_45_degrees(center,
                                                  {(GLfloat)(radius * cos(M_PI * i / 180)), (GLfloat)(radius * sin(M_PI * i / 180))},
                                                  color);
    }
}

void run_lab3()
{

    GLfloat center[2] = {SCR_WIDTH / 2 - 0.25 * SCR_WIDTH, SCR_HEIGHT / 2};
    GLfloat radius = 100;
    // draw_circle_using_midpoint_circle_drawing_algorithm(center, radius);
    draw_circle_using_polar_coordinates(center, radius);

    center[0] = center[0] + SCR_WIDTH * 0.55;
    draw_ellipse_using_midpoint_ellipse_drawing_algorithm(center, radius + 50, radius);
}
