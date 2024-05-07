
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
#include <cmath>

#include "globals.hpp"
#include "lab1.hpp"

/**
 *
 * Returns width and height as vector
 */
std::vector<int> get_resolution()
{
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    int window_width = mode->width;
    int window_height = mode->height;

    return {window_width, window_height};
}

void run_lab1()
{
    static bool first = true;

    GLfloat center[2] = {0.0, 0.0}; // changing center
    GLfloat radius_big = 0.5;
    GLfloat radius_small = 0.4;

    if (first)
    { // only displaying the resolution the very first time
        first = false;
        std::vector<int> resolution_dims = get_resolution();
        std::cout << "Display Resolution width: " << resolution_dims[0] << " height: " << resolution_dims[1] << std::endl;
    }

    glClearColor(BACKGROUND_COLOR[0], BACKGROUND_COLOR[1], BACKGROUND_COLOR[2], BACKGROUND_COLOR[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    render_ku_logo(center, radius_big, radius_small);
}

void render_ku_logo(GLfloat center[2], GLfloat radius_big, GLfloat radius_small)
{
    GLfloat middle_circle_radius = (radius_big + radius_small) / 2.3;
    GLfloat radius_for_red_circles = 0.23;

    bool dynamically_allocated_center = false;
    if (center == nullptr)
    {
        dynamically_allocated_center = true;

        center = new GLfloat(2); // center of the logo
        center[0] = 0.0;
        center[1] = 0.0;
    }

    GLfloat color_light_blue[3] = {169 / 255.0, 198 / 255.0, 228 / 255.0};
    GLfloat color_yellow[3] = {244 / 255.0, 232 / 255.0, 112 / 255.0};
    GLfloat color_brown_like[3] = {253 / 255.0, 174 / 255.0, 172 / 255.0};
    GLfloat color_black[3] = {0.0, 0.0, 0.0};
    GLfloat color_white[3] = {1.0, 1.0, 1.0};
    GLfloat color_red[3] = {1.0, 0.0, 0.0};

    GLfloat ctr[2];
    GLfloat start_angle;
    GLfloat end_angle;
    for (uint8_t i = 0; i < N_CIRCLES_AROUND_LOGO; i++)
    {
        start_angle = (GLfloat)i / (GLfloat)N_CIRCLES_AROUND_LOGO * 360.0 - 30;
        end_angle = (GLfloat)i / (GLfloat)N_CIRCLES_AROUND_LOGO * 360.0 + 30;

        ctr[0] = center[0] + (middle_circle_radius * cos(i * 2 * M_PI / N_CIRCLES_AROUND_LOGO));
        ctr[1] = center[1] + (middle_circle_radius * sin(i * 2 * M_PI / N_CIRCLES_AROUND_LOGO));
        draw_filled_circle_between_angles(ctr, radius_for_red_circles, color_red, nullptr, 0, 360);
    }

    // lines between the red circle
    GLfloat radius_for_lines = radius_big + 0.09;
    glLineWidth(5);
    GLfloat x, y;
    for (int angle = 15; angle < 360; angle += 30)
    {
        glColor3fv(color_black);
        glBegin(GL_LINES);

        glVertex2fv(center);
        x = radius_for_lines * cos(angle * M_PI / 180);
        y = radius_for_lines * sin(angle * M_PI / 180);
        glVertex2f(x, y);
        glEnd();
    }

    GLfloat triangle_base_width = 1.75 * radius_small;
    GLfloat triangle_height = (sqrt(3) / 2.0) * triangle_base_width; // equilateral triangle

    // a blue colored circle in the back
    draw_filled_circle(center, radius_big, color_light_blue, color_black);

    // a yellow colored circle
    draw_filled_circle(center, radius_small, color_yellow, color_black);

    GLfloat triangle_center_with_center_above_origin[2] = {center[0], center[1] + (GLfloat)0.1};
    draw_inverted_filled_triangle_with_base_and_height(triangle_base_width, triangle_height, triangle_center_with_center_above_origin, color_brown_like, color_black);

    draw_hollow_triangle_with_base_and_height(triangle_base_width, triangle_height, triangle_center_with_center_above_origin, color_black);

    // drawing small circles at the triangle's vertices
    GLfloat big_triangle_bottom_left_vertex[2] = {triangle_center_with_center_above_origin[0] - triangle_base_width / 2, triangle_center_with_center_above_origin[1] - triangle_height / 2};
    GLfloat big_triangle_bottom_right_vertex[2] = {triangle_center_with_center_above_origin[0] + triangle_base_width / 2, triangle_center_with_center_above_origin[1] - triangle_height / 2};
    GLfloat big_triangle_top_vertex[2] = {triangle_center_with_center_above_origin[0], triangle_center_with_center_above_origin[1] + triangle_height / 2};

    for (GLfloat *vertex : {big_triangle_bottom_left_vertex, big_triangle_bottom_right_vertex, big_triangle_top_vertex})
    {
        draw_filled_circle(vertex, 0.05, color_white, color_black);
    }

    GLfloat smaller_triangle_base_width = triangle_base_width - 0.1;
    GLfloat smaller_triangle_height = (sqrt(3) / 2.0) * smaller_triangle_base_width;
    GLfloat smaller_triangle_center[2] = {triangle_center_with_center_above_origin[0], triangle_center_with_center_above_origin[1] - (GLfloat)0.015};
    draw_hollow_triangle_with_base_and_height(smaller_triangle_base_width, smaller_triangle_height, smaller_triangle_center, color_black);

    // drawing small circles at the triangle's vertices
    GLfloat small_triangle_bottom_left_vertex[2] = {smaller_triangle_center[0] - smaller_triangle_base_width / 2, smaller_triangle_center[1] - smaller_triangle_height / 2};
    GLfloat small_triangle_bottom_right_vertex[2] = {smaller_triangle_center[0] + smaller_triangle_base_width / 2, smaller_triangle_center[1] - smaller_triangle_height / 2};
    GLfloat small_triangle_top_vertex[2] = {smaller_triangle_center[0], smaller_triangle_center[1] + smaller_triangle_height / 2};

    // draw the filled triangle as polygon
    GLfloat *vertices[] = {
        big_triangle_top_vertex, small_triangle_top_vertex,
        small_triangle_bottom_left_vertex, big_triangle_bottom_left_vertex,
        big_triangle_bottom_right_vertex, small_triangle_bottom_right_vertex,
        small_triangle_top_vertex, big_triangle_top_vertex, small_triangle_top_vertex}; // repeated to connect the loop

    draw_filled_polygon(vertices, 9, 0, 4, color_white);
    draw_filled_polygon(vertices, 9, 2, 6, color_white);
    draw_filled_polygon(vertices, 9, 4, 8, color_white);

    GLfloat center_for_vertical_rectangle[] = {center[0] - 0.08, center[1]};
    GLfloat height_for_vertical_rectangle = 0.26;
    GLfloat width_for_vertical_rectangle = 0.045;
    draw_vertical_rectangle(center_for_vertical_rectangle, height_for_vertical_rectangle, width_for_vertical_rectangle, color_black);

    GLfloat center_for_u[] = {center_for_vertical_rectangle[0] + 2 * width_for_vertical_rectangle + (GLfloat)0.05, center_for_vertical_rectangle[1]};
    draw_rotated_u((GLfloat)(2.0 / 3.3 * height_for_vertical_rectangle), (GLfloat)(4 * width_for_vertical_rectangle), width_for_vertical_rectangle, center_for_u, color_black, -30);

    if (dynamically_allocated_center)
    {
        delete[] center;
    }
}

void draw_hollow_triangle(GLfloat vertices[3][3], GLfloat color[3])
{
    glColor3fv(color);

    glLineWidth(5);

    // draw triangle

    glBegin(GL_LINE_LOOP);

    glVertex3fv(vertices[0]); // bottom left vertex
    glVertex3fv(vertices[1]); // bottom right vertex
    glVertex3fv(vertices[2]); // top vertex

    glEnd();
}

void draw_filled_triangle(GLfloat vertices[3][3], GLfloat background_color[3], GLfloat triangle_color[3])
{
    glColor3fv(background_color);

    // draw triangle

    glBegin(GL_TRIANGLES);

    glVertex3fv(vertices[0]); // bottom left vertex
    glVertex3fv(vertices[1]); // bottom right vertex
    glVertex3fv(vertices[2]); // top vertex

    glEnd();

    if (triangle_color != nullptr)
    {
        // draw the triangle outline in different color (if required)
        glColor3fv(triangle_color);

        glLineWidth(5);

        // draw triangle

        glBegin(GL_LINE_LOOP);

        glVertex3fv(vertices[0]); // bottom left vertex
        glVertex3fv(vertices[1]); // bottom right vertex
        glVertex3fv(vertices[2]); // top vertex

        glEnd();
    }
}

void draw_hollow_triangle_with_base_and_height(GLfloat base_width, GLfloat height, GLfloat center_x_y[2], GLfloat color[3])
{
    GLfloat vertices[3][3] = {
        {center_x_y[0] - base_width / 2, center_x_y[1] - height / 2, 0.0},
        {center_x_y[0] + base_width / 2, center_x_y[1] - height / 2, 0.0},
        {center_x_y[0], center_x_y[1] + height / 2, 0.0}};

    draw_hollow_triangle(vertices, color);
}

void draw_inverted_triangle_with_base_and_height(GLfloat base_width, GLfloat height, GLfloat center_x_y[2], GLfloat color[3])
{
    GLfloat vertices[3][3] = {
        {center_x_y[0], -(center_x_y[1] + height / 2), 0.0},
        {center_x_y[0] + base_width / 2, -(center_x_y[1] - height / 2), 0.0},
        {center_x_y[0] - base_width / 2, -(center_x_y[1] - height / 2), 0.0}};

    draw_hollow_triangle(vertices, color);
}

void draw_filled_triangle_with_base_and_height(GLfloat base_width, GLfloat height, GLfloat center_x_y[2], GLfloat background_color[3], GLfloat triangle_color[3])
{
    GLfloat vertices[3][3] = {
        {center_x_y[0] - base_width / 2, center_x_y[1] - height / 2, 0.0},
        {center_x_y[0] + base_width / 2, center_x_y[1] - height / 2, 0.0},
        {center_x_y[0], center_x_y[1] + height / 2, 0.0}};

    draw_filled_triangle(vertices, background_color, triangle_color);
}

void draw_inverted_filled_triangle_with_base_and_height(GLfloat base_width, GLfloat height, GLfloat center_x_y[2], GLfloat background_color[3], GLfloat triangle_color[3])
{
    GLfloat vertices[3][3] = {
        {center_x_y[0], -(center_x_y[1] + height / 2), 0.0},
        {center_x_y[0] + base_width / 2, -(center_x_y[1] - height / 2), 0.0},
        {center_x_y[0] - base_width / 2, -(center_x_y[1] - height / 2), 0.0}};

    draw_filled_triangle(vertices, background_color, triangle_color);
}

void draw_hollow_circle(GLfloat center[2], GLfloat radius, GLfloat color[3])
{
    glColor3fv(color);

    glLineWidth(5);

    glBegin(GL_LINE_LOOP);

    for (uint16_t i = 0; i < N_LINES_FOR_CIRCLE; i++)
    {
        glVertex3f(
            center[0] + (radius * cos(2 * M_PI * i / N_LINES_FOR_CIRCLE)),
            center[1] + (radius * sin(2 * M_PI * i / N_LINES_FOR_CIRCLE)),
            0.0);
    }

    glEnd();
}

void draw_hollow_circle_betweeen_angles(GLfloat center[2], GLfloat radius, GLfloat color[3], GLfloat start_angle, GLfloat end_angle)
{
    if (start_angle < 0 && end_angle < 0)
    {
        start_angle = 360 + start_angle;
        end_angle = 360 + end_angle;
    }

    // normalizing angle from 0-360 to 0 to N_LINES_FOR_CIRCLE
    uint16_t start_i = (start_angle / 360) * N_LINES_FOR_CIRCLE;
    uint16_t end_i = (end_angle / 360) * N_LINES_FOR_CIRCLE;

    glColor3fv(color);

    glLineWidth(5);

    glBegin(GL_LINE_LOOP);
    if (start_angle < 0 && end_angle >= 0)
    {
    }
    else if (start_angle >= 0 && end_angle < 0)
    {
    }
    else
    {

        for (uint16_t i = start_i; i < end_i; i++)
        {
            glVertex3f(
                center[0] + (radius * cos(2 * M_PI * i / N_LINES_FOR_CIRCLE)),
                center[1] + (radius * sin(2 * M_PI * i / N_LINES_FOR_CIRCLE)),
                0.0);
        }
    }

    glEnd();
}

void draw_filled_circle(GLfloat center[2], GLfloat radius, GLfloat background_color[3], GLfloat circle_color[3])
{
    glColor3fv(background_color);

    // draw circle

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(center[0], center[1]); // the first vertex of triangle is the center of circle
    for (uint16_t i = 0; i < N_TRIANGLES_FOR_FILLED_CIRCLE; i++)
    {
        glVertex2f(
            center[0] + (radius * cos(2 * M_PI * i / N_TRIANGLES_FOR_FILLED_CIRCLE)),
            center[1] + (radius * sin(2 * M_PI * i / N_TRIANGLES_FOR_FILLED_CIRCLE)));
    }
    // adding these two as the last triangle drawn was incomplete
    glVertex2f(center[0], center[1] + radius);
    glVertex2f(center[0], center[1]);
    glEnd();

    if (circle_color != nullptr)
    {
        glLineWidth(5);

        // draw circle

        draw_hollow_circle(center, radius, circle_color);
    }
}

void draw_filled_circle_between_angles(GLfloat center[2], GLfloat radius, GLfloat background_color[3], GLfloat circle_color[3], GLfloat start_angle, GLfloat end_angle)
{
    glColor3fv(background_color);

    if (start_angle < 0 && end_angle < 0)
    {
        start_angle = 360 + start_angle;
        end_angle = 360 + end_angle;
    }

    // normalizing angle from 0-360 to 0 to N_TRIANGLES_FOR_FILLED_CIRCLE
    uint16_t start_i = (start_angle / 360) * N_TRIANGLES_FOR_FILLED_CIRCLE;
    uint16_t end_i = (end_angle / 360) * N_TRIANGLES_FOR_FILLED_CIRCLE;

    // draw circle

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(center[0], center[1]); // the first vertex of triangle is the center of circle
    for (uint16_t i = start_i; i < end_i; i++)
    {
        glVertex2f(
            center[0] + (radius * cos(2 * M_PI * i / N_TRIANGLES_FOR_FILLED_CIRCLE)),
            center[1] + (radius * sin(2 * M_PI * i / N_TRIANGLES_FOR_FILLED_CIRCLE)));
    }
    // adding these two as the last triangle drawn was incomplete
    glVertex2f(center[0], center[1] + radius);
    glVertex2f(center[0], center[1]);
    glEnd();

    if (circle_color != nullptr)
    {
        // draw circle

        draw_hollow_circle_betweeen_angles(center, radius, circle_color, start_angle, end_angle);
    }
}

void draw_filled_polygon(GLfloat *vertices[], uint32_t len, uint32_t start, uint32_t end, GLfloat color[3])
{
    glColor3fv(color);
    glBegin(GL_POLYGON);
    for (uint32_t i = start; i < len && i < end; i++)
    {
        glVertex2fv(vertices[i]);
    }
    glVertex2fv(vertices[start]);
    glEnd();
}

void draw_vertical_rectangle(GLfloat center[2], GLfloat height, GLfloat width, GLfloat color[3])
{
    GLfloat bottom_left_vertex[] = {center[0] - width / 2, center[1] - height / 2};
    GLfloat bottom_right_vertex[] = {center[0] + width / 2, center[1] - height / 2};
    GLfloat top_right_vertex[] = {center[0] + width / 2, center[1] + height / 2};
    GLfloat top_left_vertex[] = {center[0] - width / 2, center[1] + height / 2};

    glColor3fv(color);
    glBegin(GL_QUADS);
    for (GLfloat *vertex : {bottom_left_vertex, bottom_right_vertex, top_right_vertex, top_left_vertex})
    {
        glVertex2fv(vertex);
    }
    glEnd();
}

void draw_rotated_u(GLfloat height, GLfloat width, GLfloat thickness, GLfloat center[2], GLfloat color[3], GLfloat angle)
{
    glPushMatrix();
    GLfloat center_x = center[0];
    GLfloat center_y = center[1];

    glTranslatef(center_x, center_y, 0);
    glRotatef(angle, 0, 0, 1);
    glTranslatef(-center_x, -center_y, 0);
    glColor3fv(color);

    // create  u
    glBegin(GL_QUADS);

    glVertex2f(center_x - width / 2, center_y + height / 2);
    glVertex2f(center_x - width / 2 + thickness, center_y + height / 2);
    glVertex2f(center_x - width / 2 + thickness, center_y - height / 2);
    glVertex2f(center_x - width / 2, center_y - height / 2);

    glVertex2f(center_x + width / 2, center_y + height / 2);
    glVertex2f(center_x + width / 2 - thickness, center_y + height / 2);
    glVertex2f(center_x + width / 2 - thickness, center_y - height / 2);
    glVertex2f(center_x + width / 2, center_y - height / 2);

    glVertex2f(center_x - width / 2 + thickness, center_y - height / 2 + thickness);
    glVertex2f(center_x + width / 2 - thickness, center_y - height / 2 + thickness);
    glVertex2f(center_x + width / 2 - thickness, center_y - height / 2);
    glVertex2f(center_x - width / 2 + thickness, center_y - height / 2);

    glEnd();

    glPopMatrix();
}