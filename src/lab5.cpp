
#include <inttypes.h>
#include <utility>
#include <vector>

#include <glad/glad.h>

#include "lab5.hpp"
#include "globals.hpp"

// defining bits for four regions
// TBRL: TOP BOTTOM RIGHT LEFT
constexpr uint8_t LEFT = 1 << 0;   // 0000 0001
constexpr uint8_t RIGHT = 1 << 1;  // 0000 0010
constexpr uint8_t BOTTOM = 1 << 2; // 0000 0100
constexpr uint8_t TOP = 1 << 3;    // 0000 1000

coordinate_point_t bottom_left = {-1.0, -1.0}; // (x_min, y_min)
coordinate_point_t top_right = {1.0, 1.0};     // (x_max, y_max)

uint8_t get_region_code(coordinate_point_t point)
{
    // let the point is inside the window
    uint8_t region_code = 0;

    if (point.first < bottom_left.first) // if x < x_min, the point is to the left of the window
        region_code |= LEFT;

    else if (point.first > top_right.first) // if x > x_max, the point is to the right of the window
        region_code |= RIGHT;

    if (point.second < bottom_left.second) // if y < y_min, the point is to the bottom of the window
        region_code |= BOTTOM;
    else if (point.second > top_right.second) // if y > y_man, the point is to the top of the window
        region_code |= TOP;

    return region_code;
}

void lab5_setup_window(coordinate_point_t _top_right, coordinate_point_t _bottom_left)
{
    top_right = _top_right;
    bottom_left = _bottom_left;
}

// Implementing Cohen-Sutherland algorithm
// Clipping a line from P1 = (x2, y2) to P2 = (x2, y2)
bool perform_cohen_suther_land_clipping(coordinate_point_t p1,
                                        coordinate_point_t p2,
                                        coordinate_point_t &p1_prime,
                                        coordinate_point_t &p2_prime)
{
    bool accepted = false;

    uint8_t p1_region_code = get_region_code(p1);
    uint8_t p2_region_code = get_region_code(p2);
    uint8_t code_out = p1_region_code;
    coordinate_point_t temp_point = {0, 0};
    double slope = (p2.second - p1.second) / (p2.first - p1.first); // the slope of line is same

    while (!accepted)
    {
        if ((p1_region_code == 0) && (p2_region_code == 0))
        { // if both are inside the region, accept
            accepted = true;
            break;
        }
        else if ((p1_region_code & p2_region_code) != 0)
        { // if both are in the same region and not inside, reject
            accepted = false;
            break;
        }

        if (p1_region_code != 0)
        {
            code_out = p1_region_code;
        }
        else
        {
            code_out = p2_region_code;
        }

        if (code_out & LEFT)
        { // the point is to the left of the window, so performing left clipping
            temp_point.first = bottom_left.first;
            temp_point.second = slope * (temp_point.first - p2.first) + p2.second; // y = m ( x - x1 ) + y1
        }
        else if (code_out & RIGHT)
        { // the point is to the right of the window, so performing right clipping
            temp_point.first = top_right.first;
            temp_point.second = slope * (temp_point.first - p1.first) + p1.second; // y = m ( x - x1 ) + y1
        }
        else if (code_out & BOTTOM)
        { // the point is below the window, so performing bottom clipping
            temp_point.second = bottom_left.second;
            temp_point.first = (temp_point.second - p1.second) / slope + p1.first; // (y - y1) / m + x1
        }
        else if (code_out & TOP)
        { // the point is above the widow, so performing top clipping
            temp_point.second = top_right.second;
            temp_point.first = (temp_point.second - p2.second) / slope + p2.first; // (y - y1) / m + x1
        }

        // updating the line with the intersection point
        if (code_out == p1_region_code)
        {
            p1 = temp_point;
            p1_region_code = get_region_code(p1);
        }
        else
        {
            p2 = temp_point;
            p2_region_code = get_region_code(p2);
        }
    }

    if (accepted)
    {
        p1_prime = p1;
        p2_prime = p2;
    }

    return accepted;
}

void perform_line_clipping_and_display(coordinate_point_t p1, coordinate_point_t p2)
{
    glColor3fv(COLOR_BLACK);
    glBegin(GL_LINES);
    glVertex2f(p1.first, p1.second);
    glVertex2f(p2.first, p2.second);
    glEnd();

    coordinate_point_t p1_prime, p2_prime;

    perform_cohen_suther_land_clipping(p1, p2, p1_prime, p2_prime);

    glColor3fv(COLOR_BLUE);
    glBegin(GL_LINES);
    glVertex2f(p1_prime.first, p1_prime.second);
    glVertex2f(p2_prime.first, p2_prime.second);
    glEnd();
}

/**
 * Returns 1 if inside, 0 if outside
 *
 */
bool inside_or_outside(coordinate_point_t point, uint8_t clipping_type)
{
    switch (clipping_type)
    {
    case LEFT:
        return (point.first > bottom_left.first);
        break;
    case RIGHT:
        return (point.first < top_right.first);
        break;
    case BOTTOM:
        return (point.second > bottom_left.second);
        break;
    case TOP:
        return (point.second < top_right.second);
        break;
    default:
        return false;
        break;
    }
}

coordinate_point_t calculate_insersection_point(coordinate_point_t a, coordinate_point_t b, coordinate_point_t c, coordinate_point_t d)
{
    float x1 = a.first, y1 = a.second;
    float x2 = b.first, y2 = b.second;
    float x3 = c.first, y3 = c.second;
    float x4 = d.first, y4 = d.second;

    float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    float nume_a = (x1 * y2 - y1 * x2);
    float nume_b = (x3 * y4 - y3 * x4);

    float x = (nume_a * (x3 - x4) - (x1 - x2) * nume_b) / denom;
    float y = (nume_a * (y3 - y4) - (y1 - y2) * nume_b) / denom;

    return {x, y};
}

std::vector<coordinate_point_t>  perform_sutherland_hodgeman_polygon_clipping(std::vector<coordinate_point_t> points)
{
    // keeping track of the clipped polygon
    std::vector<coordinate_point_t> clipped_points_buffer_0 = {};
    std::vector<coordinate_point_t> clipped_points_buffer_1 = points;

    // this means clipped_points_buffer_0 is changing and clipped_points_buffer_1 is constant
    uint8_t buffer_id = 0;

    size_t current_clipped_index = 0;

    bool clipped_all = false;

    coordinate_point_t temp_v1;
    coordinate_point_t temp_v2;

    std::vector<coordinate_point_t> *changing_buffer;
    std::vector<coordinate_point_t> *constant_buffer = &points;

    for (uint8_t clip : {LEFT, RIGHT, BOTTOM, TOP})
    {
        if (buffer_id) {
            changing_buffer = &clipped_points_buffer_1;
            constant_buffer = &clipped_points_buffer_0;
        }else {
            constant_buffer = &clipped_points_buffer_1;
            changing_buffer = &clipped_points_buffer_0;
        }

        std::pair<coordinate_point_t, coordinate_point_t> clip_line = {bottom_left, {bottom_left.first, top_right.second}};

        switch (clip)
        {
        case LEFT:
        default:
            break;
        case RIGHT:
            clip_line.first.first = top_right.first;
            clip_line.first.second = bottom_left.second;
            clip_line.second.first = top_right.first;
            clip_line.second.second = top_right.second;
            break;
        case BOTTOM:
            clip_line.second.first = top_right.first;
            clip_line.second.second = bottom_left.second;
            break;
        case TOP:
            clip_line.first.first = bottom_left.first;
            clip_line.first.second = top_right.second;
            clip_line.second.first = top_right.first;
            clip_line.second.second = top_right.second;
            break;
        }

        if (buffer_id)
        {
            changing_buffer = &clipped_points_buffer_1;
            constant_buffer = &clipped_points_buffer_0;
        }
        else
        {
            changing_buffer = &clipped_points_buffer_0;
            constant_buffer = &clipped_points_buffer_1;
        }
        changing_buffer->clear();

        // left right bottom top
        size_t n_points = constant_buffer->size();
        for (size_t i = 0; i < n_points; i++)
        {
            // checking for the edge
            temp_v1 = constant_buffer->at(i);
            if (i == n_points - 1)
            {
                temp_v2 = constant_buffer->at(0);
            }
            else
            {
                temp_v2 = constant_buffer->at(i + 1);
            }

            // v1 outside v2 inside  => save v1' and v2
            if (!inside_or_outside(temp_v1, clip) && inside_or_outside(temp_v2, clip))
            {
                // calculating v1'
                changing_buffer->push_back(calculate_insersection_point(temp_v1, temp_v2, clip_line.first, clip_line.second));
                changing_buffer->push_back( temp_v2);
            }

            // v1 inside v2 inside => save v2
            else if (inside_or_outside(temp_v1, clip) && inside_or_outside(temp_v2, clip))
            {
                changing_buffer->push_back(temp_v2);
            }

            // v1 inside v2 outside => save v2'
            else if (inside_or_outside(temp_v1, clip) && !inside_or_outside(temp_v2, clip))
            {
                // calculating v2'
                changing_buffer->push_back(calculate_insersection_point(temp_v1, temp_v2, clip_line.first, clip_line.second));
            }
            // v1 outside v2 outside => None
            else if (!inside_or_outside(temp_v1, clip) && !inside_or_outside(temp_v2, clip))
            {
                continue;
            }
        }

        buffer_id = !buffer_id; // changing the buffer
    }

    return clipped_points_buffer_1;
}

void run_line_clipping()
{

    coordinate_point_t p1 = {-0.8, -0.8};
    coordinate_point_t p2 = {0.8, 0.7};

    perform_line_clipping_and_display(p1, p2);

    p1.first = -0.9;
    p2.first = 0.5;

    perform_line_clipping_and_display(p1, p2);

    p1.first = 0.4;
    p2.first = 0.4;

    perform_line_clipping_and_display(p1, p2);

    p1.first = -0.9;
    p2.first = 0.5;
    p1.second = p2.second = 0.4;

    perform_line_clipping_and_display(p1, p2);
}

// Render a polygon
void render_polygon(const std::vector<coordinate_point_t> &polygon, const GLfloat color[3] = COLOR_RED)
{
    glColor3fv(color);
    glBegin(GL_LINE_LOOP);
    for (const coordinate_point_t &p : polygon)
    {
        glVertex2f(p.first, p.second);
    }
    glEnd();
}

void run_polygon_clipping()
{
    // defining a polygon
    std::vector<coordinate_point_t> polygon = {
        { -0.6f, -0.2f },
        { -0.2f, -0.8f },
        {  0.1f, -0.4f },
        {  0.7f, -0.6f },
        {  0.4f,  0.2f },
        { -0.3f,  0.5f }
    };

    glLineWidth(2);
    render_polygon(polygon, COLOR_BLUE);

    auto clipped_polygon = perform_sutherland_hodgeman_polygon_clipping(polygon);

    glLineWidth(5);
    render_polygon(clipped_polygon, COLOR_GREEN);

}

void run_lab5()
{

    // draw the window
    glColor3fv(COLOR_RED);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    glVertex2f(bottom_left.first, bottom_left.second);
    glVertex2f(top_right.first, bottom_left.second);
    glVertex2f(top_right.first, top_right.second);
    glVertex2f(bottom_left.first, top_right.second);
    glVertex2f(bottom_left.first, bottom_left.second);
    glEnd();

    // run_line_clipping();

    run_polygon_clipping();
}
