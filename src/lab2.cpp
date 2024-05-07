/**
 * Implement the following line drawing algorithm
 *
 * a) DDA Algorithm
 * b) BLA for both slopes
 *
 *
 * Use the above line drawing algorithm to draw a line graph for a given frequency.
 *
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
#include <cmath>
#include <array>
#include <memory>

#include "globals.hpp"
#include "lab2.hpp"

std::array<GLfloat, 2> normalized_to_device_specific(GLfloat x, GLfloat y)
{
    return {
        (x + 1) * SCR_WIDTH / 2,
        (y + 1) * SCR_HEIGHT / 2};
}

std::array<GLfloat, 2> device_specific_to_normalized(GLfloat x, GLfloat y)
{
    return {
        2 * (x / SCR_WIDTH) - 1,
        2 * (y / SCR_HEIGHT) - 1};
}

/***
 * The co-ordinates provided should be window specific coordinates
 */
void draw_line_using_dda(GLfloat start_point[2], GLfloat end_point[2], GLfloat color[3] = (GLfloat *)COLOR_RED )
{
    std::array<GLfloat, 2> normalized_start_point = device_specific_to_normalized(start_point[0], start_point[1]);
    std::array<GLfloat, 2> normalized_end_point = device_specific_to_normalized(end_point[0], end_point[1]);

    // getting the normalized value corresponding to 1 pixel in each dimensions
    GLfloat one_px[] = {1.0 / SCR_WIDTH, 1.0 / SCR_HEIGHT};

    GLfloat dx = normalized_end_point[0] - normalized_start_point[0];
    GLfloat dy = normalized_end_point[1] - normalized_start_point[1];

    GLfloat steps = ((std::abs(dy) > std::abs(dx)) ? std::abs(dy) : std::abs(dx));
    GLfloat x_increment = one_px[0] * dx / steps;
    GLfloat y_increment = one_px[1] * dy / steps;
    GLfloat step_increment = ((std::abs(dy) > std::abs(dx)) ? one_px[1] : one_px[0]);

    GLfloat x = normalized_start_point[0];
    GLfloat y = normalized_start_point[1];

    glColor3fv(color);
    glPointSize(5);

    for (float i = 0; i < steps; i += step_increment)
    {
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();

        x += x_increment;
        y += y_increment;
    }
}

/***
 * The co-ordinates provided should be window specific coordinates
 *
 */
void draw_line_using_bla(GLfloat start_point[2], GLfloat end_point[2], GLfloat color[3] = (GLfloat *)COLOR_RED)
{
    std::array<GLfloat, 2> normalized_start_point = device_specific_to_normalized(start_point[0], start_point[1]);
    std::array<GLfloat, 2> normalized_end_point = device_specific_to_normalized(end_point[0], end_point[1]);

    // getting the normalized value corresponding to 1 pixel in each dimensions
    GLfloat one_px[] = {1.0 / SCR_WIDTH, 1.0 / SCR_HEIGHT};

    GLfloat dx = normalized_end_point[0] - normalized_start_point[0];
    GLfloat dy = normalized_end_point[1] - normalized_start_point[1];
    GLfloat dx_times_2 = 2 * dx;
    GLfloat dy_times_2 = 2 * dy;

    GLfloat x = normalized_start_point[0];
    GLfloat y = normalized_start_point[1];

    glColor3fv(color);
    glPointSize(5);

    if (std::abs(dx) > std::abs(dy))
    {

        // initial decision parameter
        GLfloat p_k = dy_times_2 - dx;

        for (float i = 0; i < std::abs(dx); i += one_px[0])
        {

            glBegin(GL_POINTS);
            glVertex2f(x, y);
            glEnd();

            x = x + 1 * one_px[0]; //  x + 1;

            if (p_k < 0) // select the below point
            {
                p_k = p_k + dy_times_2;
            }
            else // select the above point
            {
                y = y + one_px[1]; // y + 1;
                p_k = p_k + dy_times_2 - dx_times_2;
            }
        }
    }
    else
    {
        // initial decision parameter
        GLfloat p_k = dx_times_2 - dy;

        for (float i = 0; i < std::abs(dy); i += one_px[1])
        {
            glBegin(GL_POINTS);
            glVertex2f(x, y);
            glEnd();

            y = y + one_px[1]; // y + 1;

            if (p_k < 0) // select the left point
            {
                p_k = p_k + dx_times_2;
            }
            else // select the right point
            {
                x = x + one_px[0]; // x + 1;
                p_k = p_k + dx_times_2 - dy_times_2;
            }
        }
    }
}

/**
 * Draw horizontal line using DDA
 *
 */
void draw_horizontal_line_segment(GLfloat start_points[2], GLfloat width, GLfloat color[3] = (GLfloat *)COLOR_RED)
{
    GLfloat end_points[2] = {start_points[0] + width, start_points[1]};

    draw_line_using_dda(start_points, end_points, color);
}

/**
 * Draw vertical line using BLA
 *
 */
void draw_vertical_line_segment(GLfloat start_points[2], GLfloat height, GLfloat color[3] = (GLfloat *)COLOR_RED)
{
    GLfloat end_points[2] = {start_points[0], start_points[1] + height};

    draw_line_using_bla(start_points, end_points, color);
}

void draw_historgram(uint16_t *data_id, uint16_t *frequencies, size_t len)
{

    // creating the axes
    GLfloat start_point[] = {20, 20};
    GLfloat axes_width = SCR_WIDTH - 2 * start_point[0];
    GLfloat axes_height = SCR_HEIGHT - 2 * start_point[1];
    GLfloat histogram_offset = 10;

    // drawing the axes
    draw_horizontal_line_segment(start_point, axes_width);
    draw_vertical_line_segment(start_point, axes_height);

    // determine the total sum of frequencies
    uint16_t max_frequeycy = 0;
    for(size_t i = 1; i < len; i++) {
        if(frequencies[i] > max_frequeycy)
        max_frequeycy = frequencies[i];
    }

    // create heights depending upon the relative frequency
    std::unique_ptr<float[]> heights = std::make_unique<float[]>(len);
    for(size_t i=0; i< len; i++) {
        heights.get()[i] = ((float)frequencies[i] / (float)max_frequeycy) * (axes_height - histogram_offset);
    }

    // dividing horizontal space into equally spaced data
    GLfloat entry_width = (axes_width - histogram_offset) / len;

    glColor3fv(COLOR_BLUE);
    GLfloat height_to_plot;
    GLfloat last_plotted[] = {start_point[0] + histogram_offset, start_point[1] + histogram_offset}; 
    for(size_t i = 0; i < len; i++) {
        height_to_plot = heights.get()[i];
        draw_horizontal_line_segment(last_plotted, entry_width, (GLfloat *)COLOR_BLUE);
        draw_vertical_line_segment(last_plotted, height_to_plot, (GLfloat *)COLOR_BLUE);
        last_plotted[1] = start_point[1] + height_to_plot + histogram_offset;
        draw_horizontal_line_segment(last_plotted, entry_width, (GLfloat *)COLOR_BLUE);
        last_plotted[0] = last_plotted[0] + entry_width;
        last_plotted[1] = last_plotted[1] - height_to_plot;
        draw_vertical_line_segment(last_plotted, height_to_plot, (GLfloat *)COLOR_BLUE);
    }

    
}

void run_lab2()
{
    // GLfloat start_point[] = {SCR_WIDTH / 2, SCR_HEIGHT / 2};
    // GLfloat end_point[] = {SCR_WIDTH / 2 + 500, SCR_WIDTH / 2 + 500};

    // draw_line_using_dda(start_point, end_point);
    // draw_line_using_bla(start_point, end_point);

    uint16_t data_id[] = {0, 1, 2, 3, 4, 5};
    uint16_t frequencies[] = {10, 2, 3, 3, 4, 15};

    size_t len = sizeof(data_id) / sizeof(uint16_t);

    draw_historgram(data_id, frequencies, len);
}
