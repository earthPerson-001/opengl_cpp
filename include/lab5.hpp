#pragma once

#include <inttypes.h>
#include <utility>

typedef std::pair<double, double> coordinate_point_t;

void lab5_setup_window(coordinate_point_t _top_right, coordinate_point_t _bottom_left);

// Implementing Cohen-Sutherland algorithm
// Clipping a line from P1 = (x2, y2) to P2 = (x2, y2)
bool perform_cohen_suther_land_clipping(coordinate_point_t p1,
                                        coordinate_point_t p2,
                                        coordinate_point_t &p1_prime,
                                        coordinate_point_t &p2_prime);

void run_lab5();
