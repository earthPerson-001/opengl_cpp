#pragma once

#include <glad/glad.h>

constexpr uint8_t N_CIRCLES_AROUND_LOGO = 12;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void render_ku_logo(GLfloat center[2], GLfloat radius_big = 0.5, GLfloat radius_small = 0.4);
void draw_hollow_triangle(GLfloat vertices[3][3], GLfloat color[3]);
void draw_filled_triangle(GLfloat vertices[3][3], GLfloat background_color[3], GLfloat triangle_color[3] = nullptr);
void draw_hollow_triangle_with_base_and_height(GLfloat base_width, GLfloat height, GLfloat center_x_y[2], GLfloat color[3]);
void draw_hollow_circle(GLfloat center[2], GLfloat radius, GLfloat color[3]);
void draw_filled_circle(GLfloat center[2], GLfloat radius, GLfloat background_color[3], GLfloat circle_color[3]);
void draw_inverted_filled_triangle_with_base_and_height(GLfloat base_width, GLfloat height, GLfloat center_x_y[2], GLfloat background_color[3], GLfloat triangle_color[3]);
void draw_filled_polygon(GLfloat *vertices[], uint32_t len, uint32_t start, uint32_t end, GLfloat color[3]);
void draw_vertical_rectangle(GLfloat center[2], GLfloat height, GLfloat width, GLfloat color[3]);
void draw_rotated_u(GLfloat height, GLfloat width, GLfloat thickness, GLfloat center[2], GLfloat color[3], GLfloat angle);
void draw_filled_circle_between_angles(GLfloat center[2], GLfloat radius, GLfloat background_color[3], GLfloat circle_color[3], GLfloat start_angle, GLfloat end_angle);
void draw_hollow_circle_betweeen_angles(GLfloat center[2], GLfloat radius, GLfloat color[3], GLfloat start_angle, GLfloat end_angle);
void draw_hollow_circle_between_points(GLfloat center[2], GLfloat radius, GLfloat color[3], GLfloat from[2], GLfloat to[2]);

// run the whole lab1
void run_lab1();
