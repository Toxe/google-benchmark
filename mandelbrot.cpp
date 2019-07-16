#include <algorithm>
#include <benchmark/benchmark.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <numeric>
#include <tuple>
#include <vector>
#include <float.h>

bool equal_enough_v1(double a, double b)
{
    a = std::abs(a);
    b = std::abs(b);

    return std::abs(a - b) <= std::max(a, b) * DBL_EPSILON;
}

bool equal_enough_v2(double a, double b)
{
    a = std::abs(a);
    b = std::abs(b);

    return std::abs(a - b) <= std::max(a, b) * std::numeric_limits<double>::epsilon();
}

bool equal_enough_v3(double a, double b)
{
    static constexpr double epsilon = std::numeric_limits<double>::epsilon();

    a = std::abs(a);
    b = std::abs(b);

    return std::abs(a - b) <= std::max(a, b) * epsilon;
}

bool equal_enough_v4(double a, double b)
{
    constexpr double epsilon = std::numeric_limits<double>::epsilon();

    a = std::abs(a);
    b = std::abs(b);

    return std::abs(a - b) <= std::max(a, b) * epsilon;
}

bool equal_enough_v5(float a, float b)
{
    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    a = std::abs(a);
    b = std::abs(b);

    return std::abs(a - b) <= std::max(a, b) * epsilon;
}

struct PixelColor {
    unsigned char r, g, b;
};

struct GradientColor {
    double pos;
    double r, g, b;
};

struct Gradient {
    std::vector<GradientColor> colors;
};

struct GradientColor_v2 {
    float pos;
    float r, g, b;
    bool operator==(const float p) const { return equal_enough_v5(p, pos); }
    bool operator<(const GradientColor_v2& col) const { return pos < col.pos; }
};

struct Gradient_v2 {
    std::vector<GradientColor_v2> colors;
};

Gradient make_gradient()
{
    Gradient gradient;
    gradient.colors.push_back(GradientColor{0.00, 0.0, 0.0, 0.0});
    gradient.colors.push_back(GradientColor{0.50, 0.0, 0.0, 1.0});
    gradient.colors.push_back(GradientColor{0.75, 0.0, 1.0, 0.0});
    gradient.colors.push_back(GradientColor{0.90, 1.0, 0.0, 0.0});
    gradient.colors.push_back(GradientColor{1.00, 1.0, 1.0, 0.0});
    return gradient;
}

Gradient_v2 make_gradient_v2()
{
    Gradient_v2 gradient;
    gradient.colors.push_back(GradientColor_v2{0.00f, 0.0f, 0.0f, 0.0f});
    gradient.colors.push_back(GradientColor_v2{0.50f, 0.0f, 0.0f, 1.0f});
    gradient.colors.push_back(GradientColor_v2{0.75f, 0.0f, 1.0f, 0.0f});
    gradient.colors.push_back(GradientColor_v2{0.90f, 1.0f, 0.0f, 0.0f});
    gradient.colors.push_back(GradientColor_v2{1.00f, 1.0f, 1.0f, 0.0f});
    return gradient;
}

void color_from_gradient_range_v1(const GradientColor& left, const GradientColor& right, const double pos, double& r, double& g, double& b)
{
    double pos2 = (pos - left.pos) / (right.pos - left.pos);

    r = ((right.r - left.r) * pos2) + left.r;
    g = ((right.g - left.g) * pos2) + left.g;
    b = ((right.b - left.b) * pos2) + left.b;
}

bool color_from_gradient_v1(const Gradient& gradient, double pos, double& r, double& g, double& b)
{
    if (pos < 0.0)
        pos = 0.0;

    if (pos > 1.0)
        pos = 1.0;

    for (std::size_t i = 1; i < gradient.colors.size(); ++i) {
        const GradientColor& left = gradient.colors[i - 1];
        const GradientColor& right = gradient.colors[i];

        if (pos >= left.pos && pos <= right.pos) {
            color_from_gradient_range_v1(left, right, pos, r, g, b);
            return true;
        }
    }

    return false;
}

void color_from_gradient_range_v2(const GradientColor& left, const GradientColor& right, const double pos, double& r, double& g, double& b)
{
    double pos2 = (pos - left.pos) / (right.pos - left.pos);

    r = ((right.r - left.r) * pos2) + left.r;
    g = ((right.g - left.g) * pos2) + left.g;
    b = ((right.b - left.b) * pos2) + left.b;
}

bool color_from_gradient_v2(const Gradient& gradient, double pos, double& r, double& g, double& b)
{
    if (pos < 0.0)
        pos = 0.0;

    if (pos > 1.0)
        pos = 1.0;

    for (std::size_t i = 1; i < gradient.colors.size(); ++i) {
        const GradientColor& left = gradient.colors[i - 1];
        const GradientColor& right = gradient.colors[i];

        if (pos >= left.pos && pos <= right.pos) {
            color_from_gradient_range_v2(left, right, pos, r, g, b);
            return true;
        }
    }

    return false;
}

void color_from_gradient_range_v3(const GradientColor& left, const GradientColor& right, const double pos, double& r, double& g, double& b)
{
    const double pos2 = (pos - left.pos) / (right.pos - left.pos);

    r = ((right.r - left.r) * pos2) + left.r;
    g = ((right.g - left.g) * pos2) + left.g;
    b = ((right.b - left.b) * pos2) + left.b;
}

bool color_from_gradient_v3(const Gradient& gradient, double pos, double& r, double& g, double& b)
{
    if (pos < 0.0)
        pos = 0.0;

    if (pos > 1.0)
        pos = 1.0;

    for (std::size_t i = 1; i < gradient.colors.size(); ++i) {
        const GradientColor& left = gradient.colors[i - 1];
        const GradientColor& right = gradient.colors[i];

        if (pos >= left.pos && pos <= right.pos) {
            color_from_gradient_range_v3(left, right, pos, r, g, b);
            return true;
        }
    }

    return false;
}

PixelColor color_from_gradient_range_v4(const GradientColor& left, const GradientColor& right, const double pos)
{
    const double pos2 = (pos - left.pos) / (right.pos - left.pos);

    return PixelColor{
        static_cast<unsigned char>(255.0 * (((right.r - left.r) * pos2) + left.r)),
        static_cast<unsigned char>(255.0 * (((right.g - left.g) * pos2) + left.g)),
        static_cast<unsigned char>(255.0 * (((right.b - left.b) * pos2) + left.b))
    };
}

PixelColor color_from_gradient_v4(const Gradient& gradient, double pos)
{
    if (pos < 0.0)
        pos = 0.0;

    if (pos > 1.0)
        pos = 1.0;

    for (std::size_t i = 1; i < gradient.colors.size(); ++i) {
        const GradientColor& left = gradient.colors[i - 1];
        const GradientColor& right = gradient.colors[i];

        if (pos >= left.pos && pos <= right.pos)
            return color_from_gradient_range_v4(left, right, pos);
    }

    static constexpr PixelColor black{0, 0, 0};
    return black;
}

void color_from_gradient_range_v5(const GradientColor& left, const GradientColor& right, const double pos, PixelColor& pixel_color)
{
    const double pos2 = (pos - left.pos) / (right.pos - left.pos);

    pixel_color.r = static_cast<unsigned char>(255.0 * (((right.r - left.r) * pos2) + left.r));
    pixel_color.g = static_cast<unsigned char>(255.0 * (((right.g - left.g) * pos2) + left.g));
    pixel_color.b = static_cast<unsigned char>(255.0 * (((right.b - left.b) * pos2) + left.b));
}

void color_from_gradient_v5(const Gradient& gradient, double pos, PixelColor& pixel_color)
{
    for (std::size_t i = 1; i < gradient.colors.size(); ++i) {
        const GradientColor& left = gradient.colors[i - 1];
        const GradientColor& right = gradient.colors[i];

        if (pos >= left.pos && pos <= right.pos) {
            color_from_gradient_range_v5(left, right, pos, pixel_color);
            break;
        }
    }
}

void color_from_gradient_range_v6(const GradientColor_v2& left, const GradientColor_v2& right, const float pos, PixelColor& pixel_color)
{
    const float pos2 = (pos - left.pos) / (right.pos - left.pos);

    pixel_color.r = static_cast<unsigned char>(255.0f * (((right.r - left.r) * pos2) + left.r));
    pixel_color.g = static_cast<unsigned char>(255.0f * (((right.g - left.g) * pos2) + left.g));
    pixel_color.b = static_cast<unsigned char>(255.0f * (((right.b - left.b) * pos2) + left.b));
}

void color_from_gradient_v6(const Gradient_v2& gradient, float pos, PixelColor& pixel_color)
{
    for (std::size_t i = 1; i < gradient.colors.size(); ++i) {
        const GradientColor_v2& left = gradient.colors[i - 1];
        const GradientColor_v2& right = gradient.colors[i];

        if (pos >= left.pos && pos <= right.pos) {
            color_from_gradient_range_v6(left, right, pos, pixel_color);
            break;
        }
    }
}

void color_from_gradient_v7(const Gradient_v2& gradient, const float pos, PixelColor& pixel_color)
{
    const auto end = gradient.colors.cend();

    const auto it = std::adjacent_find(gradient.colors.cbegin(), end,
        [&](const GradientColor_v2& left, const GradientColor_v2& right) { return left.pos <= pos && pos <= right.pos; });

    if (it != end)
        color_from_gradient_range_v6(*it, *(it+1), pos, pixel_color);
}

void mandelbrot_calc_v1(const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height,
                        std::vector<int>& histogram, std::vector<int>& iterations_per_pixel, std::vector<double>& smoothed_distances_to_next_iteration_per_pixel)
{
    const double width = height * (static_cast<double>(image_width) / static_cast<double>(image_height));

    const double x_left   = center_x - width / 2.0;
 // const double x_right  = center_x + width / 2.0;
    const double y_top    = center_y + height / 2.0;
 // const double y_bottom = center_y - height / 2.0;

    const double bailout = 20.0;
    const double bailout_squared = bailout * bailout;
    const double log_log_bailout = std::log(std::log(bailout));
    const double log_2 = std::log(2.0);

    double final_magnitude = 0.0;

    for (auto& h : histogram)
        h = 0;

    for (int pixel_y = 0; pixel_y < image_height; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < image_width; ++pixel_x) {
            const double x0 = x_left + width * (static_cast<double>(pixel_x) / static_cast<double>(image_width));
            const double y0 = y_top - height * (static_cast<double>(pixel_y) / static_cast<double>(image_height));

            double x = 0.0;
            double y = 0.0;

            // iteration, will be from 1 to max_iterations once the loop is done
            int iter = 0;

            while (iter < max_iterations) {
                const double x_squared = x*x;
                const double y_squared = y*y;

                if (x_squared + y_squared >= bailout_squared) {
                    final_magnitude = std::sqrt(x_squared + y_squared);
                    break;
                }

                const double xtemp = x_squared - y_squared + x0;
                y = 2.0*x*y + y0;
                x = xtemp;

                ++iter;
            }

            const int pixel = pixel_y * image_width + pixel_x;

            if (iter < max_iterations) {
                smoothed_distances_to_next_iteration_per_pixel[static_cast<std::size_t>(pixel)] = 1.0 - std::min(1.0, (std::log(std::log(final_magnitude)) - log_log_bailout) / log_2);
                ++histogram[static_cast<std::size_t>(iter)];  // no need to count histogram[max_iterations]
            }

            iterations_per_pixel[static_cast<std::size_t>(pixel)] = iter;  // 1 .. max_iterations
        }
    }
}

void mandelbrot_calc_v2(const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height,
                        std::vector<int>& histogram, std::vector<int>& iterations_per_pixel, std::vector<double>& smoothed_distances_to_next_iteration_per_pixel)
{
    const double width = height * (static_cast<double>(image_width) / static_cast<double>(image_height));

    const double x_left   = center_x - width / 2.0;
 // const double x_right  = center_x + width / 2.0;
    const double y_top    = center_y + height / 2.0;
 // const double y_bottom = center_y - height / 2.0;

    constexpr double bailout = 20.0;
    constexpr double bailout_squared = bailout * bailout;
    const double log_log_bailout = std::log(std::log(bailout));
    const double log_2 = std::log(2.0);

    double final_magnitude = 0.0;

    std::fill(histogram.begin(), histogram.end(), 0);

    for (int pixel_y = 0; pixel_y < image_height; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < image_width; ++pixel_x) {
            const double x0 = x_left + width * (static_cast<double>(pixel_x) / static_cast<double>(image_width));
            const double y0 = y_top - height * (static_cast<double>(pixel_y) / static_cast<double>(image_height));

            double x = 0.0;
            double y = 0.0;

            // iteration, will be from 1 to max_iterations once the loop is done
            int iter = 0;

            while (iter < max_iterations) {
                const double x_squared = x*x;
                const double y_squared = y*y;

                if (x_squared + y_squared >= bailout_squared) {
                    final_magnitude = std::sqrt(x_squared + y_squared);
                    break;
                }

                const double xtemp = x_squared - y_squared + x0;
                y = 2.0*x*y + y0;
                x = xtemp;

                ++iter;
            }

            const int pixel = pixel_y * image_width + pixel_x;

            if (iter < max_iterations) {
                smoothed_distances_to_next_iteration_per_pixel[static_cast<std::size_t>(pixel)] = 1.0 - std::min(1.0, (std::log(std::log(final_magnitude)) - log_log_bailout) / log_2);
                ++histogram[static_cast<std::size_t>(iter)];  // no need to count histogram[max_iterations]
            }

            iterations_per_pixel[static_cast<std::size_t>(pixel)] = iter;  // 1 .. max_iterations
        }
    }
}

void mandelbrot_calc_v3(const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height,
                     std::vector<int>& histogram, std::vector<int>& iterations_per_pixel, std::vector<double>& smoothed_distances_to_next_iteration_per_pixel)
{
    const double width = height * (static_cast<double>(image_width) / static_cast<double>(image_height));

    const double x_left   = center_x - width / 2.0;
 // const double x_right  = center_x + width / 2.0;
    const double y_top    = center_y + height / 2.0;
 // const double y_bottom = center_y - height / 2.0;

    constexpr double bailout = 20.0;
    constexpr double bailout_squared = bailout * bailout;
    const double log_log_bailout = std::log(std::log(bailout));
    const double log_2 = std::log(2.0);

    double final_magnitude = 0.0;

    std::fill(histogram.begin(), histogram.end(), 0);

    for (int pixel_y = 0; pixel_y < image_height; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < image_width; ++pixel_x) {
            const double x0 = x_left + width * (static_cast<double>(pixel_x) / static_cast<double>(image_width));
            const double y0 = y_top - height * (static_cast<double>(pixel_y) / static_cast<double>(image_height));

            double x = 0.0;
            double y = 0.0;

            // iteration, will be from 1 to max_iterations once the loop is done
            int iter = 0;

            while (iter < max_iterations) {
                const double x_squared = x*x;
                const double y_squared = y*y;

                if (x_squared + y_squared >= bailout_squared) {
                    final_magnitude = std::sqrt(x_squared + y_squared);
                    break;
                }

                const double xtemp = x_squared - y_squared + x0;
                y = 2.0*x*y + y0;
                x = xtemp;

                ++iter;
            }

            const int pixel = pixel_y * image_width + pixel_x;

            if (iter < max_iterations) {
                smoothed_distances_to_next_iteration_per_pixel[static_cast<std::size_t>(pixel)] = 1.0 - std::min(1.0, (std::log(std::log(final_magnitude)) - log_log_bailout) / log_2);
                ++histogram[static_cast<std::size_t>(iter)];  // no need to count histogram[max_iterations]
            }

            iterations_per_pixel[static_cast<std::size_t>(pixel)] = iter;  // 1 .. max_iterations
        }
    }
}

void mandelbrot_calc_v4(const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height,
                     std::vector<int>& histogram, std::vector<int>& iterations_per_pixel, std::vector<double>& smoothed_distances_to_next_iteration_per_pixel)
{
    const double width = height * (static_cast<double>(image_width) / static_cast<double>(image_height));

    const double x_left   = center_x - width / 2.0;
 // const double x_right  = center_x + width / 2.0;
    const double y_top    = center_y + height / 2.0;
 // const double y_bottom = center_y - height / 2.0;

    constexpr double bailout = 20.0;
    constexpr double bailout_squared = bailout * bailout;
    const double log_log_bailout = std::log(std::log(bailout));
    const double log_2 = std::log(2.0);

    double final_magnitude = 0.0;

    std::fill(histogram.begin(), histogram.end(), 0);

    for (int pixel_y = 0; pixel_y < image_height; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < image_width; ++pixel_x) {
            const double x0 = x_left + width * (static_cast<double>(pixel_x) / static_cast<double>(image_width));
            const double y0 = y_top - height * (static_cast<double>(pixel_y) / static_cast<double>(image_height));

            double x = 0.0;
            double y = 0.0;

            // iteration, will be from 1 to max_iterations once the loop is done
            int iter = 0;

            while (iter < max_iterations) {
                const double x_squared = x*x;
                const double y_squared = y*y;

                if (x_squared + y_squared >= bailout_squared) {
                    final_magnitude = std::sqrt(x_squared + y_squared);
                    break;
                }

                const double xtemp = x_squared - y_squared + x0;
                y = 2.0*x*y + y0;
                x = xtemp;

                ++iter;
            }

            const int pixel = pixel_y * image_width + pixel_x;

            if (iter < max_iterations) {
                smoothed_distances_to_next_iteration_per_pixel[static_cast<std::size_t>(pixel)] = 1.0 - std::min(1.0, (std::log(std::log(final_magnitude)) - log_log_bailout) / log_2);
                ++histogram[static_cast<std::size_t>(iter)];  // no need to count histogram[max_iterations]
            }

            iterations_per_pixel[static_cast<std::size_t>(pixel)] = iter;  // 1 .. max_iterations
        }
    }
}

void mandelbrot_calc_v5(const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height,
                     std::vector<int>& histogram, std::vector<int>& iterations_per_pixel, std::vector<float>& smoothed_distances_to_next_iteration_per_pixel)
{
    const double width = height * (static_cast<double>(image_width) / static_cast<double>(image_height));

    const double x_left   = center_x - width / 2.0;
 // const double x_right  = center_x + width / 2.0;
    const double y_top    = center_y + height / 2.0;
 // const double y_bottom = center_y - height / 2.0;

    constexpr double bailout = 20.0;
    constexpr double bailout_squared = bailout * bailout;
    const double log_log_bailout = std::log(std::log(bailout));
    const double log_2 = std::log(2.0);

    double final_magnitude = 0.0;

    std::fill(histogram.begin(), histogram.end(), 0);

    for (int pixel_y = 0; pixel_y < image_height; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < image_width; ++pixel_x) {
            const double x0 = x_left + width * (static_cast<double>(pixel_x) / static_cast<double>(image_width));
            const double y0 = y_top - height * (static_cast<double>(pixel_y) / static_cast<double>(image_height));

            double x = 0.0;
            double y = 0.0;

            // iteration, will be from 1 to max_iterations once the loop is done
            int iter = 0;

            while (iter < max_iterations) {
                const double x_squared = x*x;
                const double y_squared = y*y;

                if (x_squared + y_squared >= bailout_squared) {
                    final_magnitude = std::sqrt(x_squared + y_squared);
                    break;
                }

                const double xtemp = x_squared - y_squared + x0;
                y = 2.0*x*y + y0;
                x = xtemp;

                ++iter;
            }

            const int pixel = pixel_y * image_width + pixel_x;

            if (iter < max_iterations) {
                smoothed_distances_to_next_iteration_per_pixel[static_cast<std::size_t>(pixel)] = 1.0f - std::min(1.0f, static_cast<float>((std::log(std::log(final_magnitude)) - log_log_bailout) / log_2));
                ++histogram[static_cast<std::size_t>(iter)];  // no need to count histogram[max_iterations]
            }

            iterations_per_pixel[static_cast<std::size_t>(pixel)] = iter;  // 1 .. max_iterations
        }
    }
}

void mandelbrot_colorize_v1(const int image_width, const int image_height, const int max_iterations, const Gradient& gradient,
                            unsigned char* image_data, const std::vector<int>& histogram, const std::vector<int>& iterations_per_pixel, const std::vector<double>& smoothed_distances_to_next_iteration_per_pixel, std::vector<double>& normalized_colors)
{
    for (auto& d : normalized_colors)
        d = 0.0;

    // Sum all iterations, not counting the last one at position histogram[max_iterations] (which
    // are points in the Mandelbrot Set).
    int total_iterations = 0;

    for (int i = 1; i < max_iterations; ++i)
        total_iterations += histogram[static_cast<std::size_t>(i)];

    // Normalize the colors (0.0 .. 1.0) based on how often they are used in the image, not counting
    // histogram[max_iterations] (which are points in the Mandelbrot Set).
    int running_total = 0;

    for (int i = 1; i < max_iterations; ++i) {
        running_total += histogram[static_cast<std::size_t>(i)];
        normalized_colors[static_cast<std::size_t>(i)] = static_cast<double>(running_total) / static_cast<double>(total_iterations);
    }

    for (int pixel_y = 0; pixel_y < image_height; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < image_width; ++pixel_x) {
            int pixel = pixel_y * image_width + pixel_x;
            int iter = iterations_per_pixel[static_cast<std::size_t>(pixel)];  // 1 .. max_iterations

            if (iter == max_iterations) {
                // pixels with max. iterations (aka. inside the Mandelbrot Set) are always black
                image_data[3 * pixel + 0] = 0;
                image_data[3 * pixel + 1] = 0;
                image_data[3 * pixel + 2] = 0;
            } else {
                // we use the color of the previous iteration in order to cover the full gradient range
                double color_of_previous_iter = normalized_colors[static_cast<std::size_t>(iter - 1)];
                double color_of_current_iter  = normalized_colors[static_cast<std::size_t>(iter)];

                double smoothed_distance_to_next_iteration = smoothed_distances_to_next_iteration_per_pixel[static_cast<std::size_t>(pixel)];  // 0 .. <1.0
                double pos_in_gradient = color_of_previous_iter + smoothed_distance_to_next_iteration * (color_of_current_iter - color_of_previous_iter);

                double r, g, b;
                color_from_gradient_v1(gradient, pos_in_gradient, r, g, b);

                image_data[3 * pixel + 0] = static_cast<unsigned char>(255.0 * r);
                image_data[3 * pixel + 1] = static_cast<unsigned char>(255.0 * g);
                image_data[3 * pixel + 2] = static_cast<unsigned char>(255.0 * b);
            }
        }
    }
}

void mandelbrot_colorize_v2(const int image_width, const int image_height, const int max_iterations, const Gradient& gradient,
                            unsigned char* image_data, const std::vector<int>& histogram, const std::vector<int>& iterations_per_pixel, const std::vector<double>& smoothed_distances_to_next_iteration_per_pixel, std::vector<double>& normalized_colors)
{
    std::fill(normalized_colors.begin(), normalized_colors.end(), 0.0);

    // Sum all iterations, not counting the last one at position histogram[max_iterations] (which
    // are points in the Mandelbrot Set).
    int total_iterations = 0;

    for (int i = 1; i < max_iterations; ++i)
        total_iterations += histogram[static_cast<std::size_t>(i)];

    // Normalize the colors (0.0 .. 1.0) based on how often they are used in the image, not counting
    // histogram[max_iterations] (which are points in the Mandelbrot Set).
    int running_total = 0;

    for (int i = 1; i < max_iterations; ++i) {
        running_total += histogram[static_cast<std::size_t>(i)];
        normalized_colors[static_cast<std::size_t>(i)] = static_cast<double>(running_total) / static_cast<double>(total_iterations);
    }

    for (int pixel_y = 0; pixel_y < image_height; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < image_width; ++pixel_x) {
            int pixel = pixel_y * image_width + pixel_x;
            int iter = iterations_per_pixel[static_cast<std::size_t>(pixel)];  // 1 .. max_iterations

            if (iter == max_iterations) {
                // pixels with max. iterations (aka. inside the Mandelbrot Set) are always black
                image_data[3 * pixel + 0] = 0;
                image_data[3 * pixel + 1] = 0;
                image_data[3 * pixel + 2] = 0;
            } else {
                // we use the color of the previous iteration in order to cover the full gradient range
                double color_of_previous_iter = normalized_colors[static_cast<std::size_t>(iter - 1)];
                double color_of_current_iter  = normalized_colors[static_cast<std::size_t>(iter)];

                double smoothed_distance_to_next_iteration = smoothed_distances_to_next_iteration_per_pixel[static_cast<std::size_t>(pixel)];  // 0 .. <1.0
                double pos_in_gradient = color_of_previous_iter + smoothed_distance_to_next_iteration * (color_of_current_iter - color_of_previous_iter);

                double r, g, b;
                color_from_gradient_v2(gradient, pos_in_gradient, r, g, b);

                image_data[3 * pixel + 0] = static_cast<unsigned char>(255.0 * r);
                image_data[3 * pixel + 1] = static_cast<unsigned char>(255.0 * g);
                image_data[3 * pixel + 2] = static_cast<unsigned char>(255.0 * b);
            }
        }
    }
}

void mandelbrot_colorize_v3(const int image_width, const int image_height, const int max_iterations, const Gradient& gradient,
                         unsigned char* image_data, const std::vector<int>& histogram, const std::vector<int>& iterations_per_pixel, const std::vector<double>& smoothed_distances_to_next_iteration_per_pixel, std::vector<double>& normalized_colors)
{
    // Sum all iterations, not counting the last one at position histogram[max_iterations] (which
    // are points in the Mandelbrot Set).
    const double total_iterations = std::accumulate(std::next(histogram.cbegin()), std::prev(histogram.cend()), 0);

    // Normalize the colors (0.0 .. 1.0) based on how often they are used in the image, not counting
    // histogram[max_iterations] (which are points in the Mandelbrot Set).
    int running_total = 0;

    for (std::size_t i = 1; i < static_cast<std::size_t>(max_iterations); ++i) {
        running_total += histogram[i];
        normalized_colors[i] = running_total / total_iterations;
    }

    for (int pixel_y = 0; pixel_y < image_height; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < image_width; ++pixel_x) {
            const int pixel = pixel_y * image_width + pixel_x;
            const int iter = iterations_per_pixel[static_cast<std::size_t>(pixel)];  // 1 .. max_iterations

            if (iter == max_iterations) {
                // pixels with max. iterations (aka. inside the Mandelbrot Set) are always black
                image_data[3 * pixel + 0] = 0;
                image_data[3 * pixel + 1] = 0;
                image_data[3 * pixel + 2] = 0;
            } else {
                // we use the color of the previous iteration in order to cover the full gradient range
                const double color_of_previous_iter = normalized_colors[static_cast<std::size_t>(iter - 1)];
                const double color_of_current_iter  = normalized_colors[static_cast<std::size_t>(iter)];

                const double smoothed_distance_to_next_iteration = smoothed_distances_to_next_iteration_per_pixel[static_cast<std::size_t>(pixel)];  // 0 .. <1.0
                const double pos_in_gradient = color_of_previous_iter + smoothed_distance_to_next_iteration * (color_of_current_iter - color_of_previous_iter);

                double r, g, b;
                color_from_gradient_v3(gradient, pos_in_gradient, r, g, b);

                image_data[3 * pixel + 0] = static_cast<unsigned char>(255.0 * r);
                image_data[3 * pixel + 1] = static_cast<unsigned char>(255.0 * g);
                image_data[3 * pixel + 2] = static_cast<unsigned char>(255.0 * b);
            }
        }
    }
}

void mandelbrot_colorize_v4(const int image_width, const int image_height, const int max_iterations, const Gradient& gradient,
                         PixelColor* image_data, const std::vector<int>& histogram, const std::vector<int>& iterations_per_pixel, const std::vector<double>& smoothed_distances_to_next_iteration_per_pixel, std::vector<double>& normalized_colors)
{
    constexpr PixelColor black{0, 0, 0};

    // Sum all iterations, not counting the last one at position histogram[max_iterations] (which
    // are points in the Mandelbrot Set).
    const double total_iterations = std::accumulate(std::next(histogram.cbegin()), std::prev(histogram.cend()), 0);

    // Normalize the colors (0.0 .. 1.0) based on how often they are used in the image, not counting
    // histogram[max_iterations] (which are points in the Mandelbrot Set).
    int running_total = 0;

    for (std::size_t i = 1; i < static_cast<std::size_t>(max_iterations); ++i) {
        running_total += histogram[i];
        normalized_colors[i] = running_total / total_iterations;
    }

    for (int pixel_y = 0; pixel_y < image_height; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < image_width; ++pixel_x) {
            const int pixel = pixel_y * image_width + pixel_x;
            const int iter = iterations_per_pixel[static_cast<std::size_t>(pixel)];  // 1 .. max_iterations

            if (iter == max_iterations) {
                // pixels with max. iterations (aka. inside the Mandelbrot Set) are always black
                image_data[pixel] = black;
            } else {
                // we use the color of the previous iteration in order to cover the full gradient range
                const double color_of_previous_iter = normalized_colors[static_cast<std::size_t>(iter - 1)];
                const double color_of_current_iter  = normalized_colors[static_cast<std::size_t>(iter)];

                const double smoothed_distance_to_next_iteration = smoothed_distances_to_next_iteration_per_pixel[static_cast<std::size_t>(pixel)];  // 0 .. <1.0
                const double pos_in_gradient = color_of_previous_iter + smoothed_distance_to_next_iteration * (color_of_current_iter - color_of_previous_iter);

                image_data[pixel] = color_from_gradient_v4(gradient, pos_in_gradient);
            }
        }
    }
}

void mandelbrot_colorize_v5(const int image_width, const int image_height, const int max_iterations, const Gradient& gradient,
                         PixelColor* image_data, const std::vector<int>& histogram, const std::vector<int>& iterations_per_pixel, const std::vector<double>& smoothed_distances_to_next_iteration_per_pixel, std::vector<double>& normalized_colors)
{
    // Sum all iterations, not counting the last one at position histogram[max_iterations] (which
    // are points in the Mandelbrot Set).
    const double total_iterations = std::accumulate(std::next(histogram.cbegin()), std::prev(histogram.cend()), 0);

    // Normalize the colors (0.0 .. 1.0) based on how often they are used in the image, not counting
    // histogram[max_iterations] (which are points in the Mandelbrot Set).
    int running_total = 0;

    for (std::size_t i = 1; i < static_cast<std::size_t>(max_iterations); ++i) {
        running_total += histogram[i];
        normalized_colors[i] = running_total / total_iterations;
    }

    for (int pixel_y = 0; pixel_y < image_height; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < image_width; ++pixel_x) {
            const int pixel = pixel_y * image_width + pixel_x;
            const int iter = iterations_per_pixel[static_cast<std::size_t>(pixel)];  // 1 .. max_iterations

            if (iter == max_iterations) {
                // pixels with max. iterations (aka. inside the Mandelbrot Set) are always black
                image_data[pixel].r = 0;
                image_data[pixel].g = 0;
                image_data[pixel].b = 0;
            } else {
                // we use the color of the previous iteration in order to cover the full gradient range
                const double color_of_previous_iter = normalized_colors[static_cast<std::size_t>(iter - 1)];
                const double color_of_current_iter  = normalized_colors[static_cast<std::size_t>(iter)];

                const double smoothed_distance_to_next_iteration = smoothed_distances_to_next_iteration_per_pixel[static_cast<std::size_t>(pixel)];  // 0 .. <1.0
                const double pos_in_gradient = color_of_previous_iter + smoothed_distance_to_next_iteration * (color_of_current_iter - color_of_previous_iter);

                color_from_gradient_v5(gradient, pos_in_gradient, image_data[pixel]);
            }
        }
    }
}

void mandelbrot_colorize_v6(const int max_iterations, const Gradient& gradient,
                         std::vector<PixelColor>& image_data, const std::vector<int>& histogram, const std::vector<int>& iterations_per_pixel, const std::vector<double>& smoothed_distances_to_next_iteration_per_pixel, std::vector<double>& normalized_colors)
{
    // Sum all iterations, not counting the last one at position histogram[max_iterations] (which
    // are points in the Mandelbrot Set).
    const double total_iterations = std::accumulate(std::next(histogram.cbegin()), std::prev(histogram.cend()), 0);

    // Normalize the colors (0.0 .. 1.0) based on how often they are used in the image, not counting
    // histogram[max_iterations] (which are points in the Mandelbrot Set).
    int running_total = 0;

    for (std::size_t i = 1; i < static_cast<std::size_t>(max_iterations); ++i) {
        running_total += histogram[i];
        normalized_colors[i] = running_total / total_iterations;
    }

    auto pixel_color = image_data.begin();
    auto iter = iterations_per_pixel.cbegin();
    auto smoothed_distance_to_next_iteration = smoothed_distances_to_next_iteration_per_pixel.cbegin();

    while (pixel_color != image_data.end()) {
        if (*iter == max_iterations) {
            // pixels with max. iterations (aka. inside the Mandelbrot Set) are always black
            pixel_color->r = 0;
            pixel_color->g = 0;
            pixel_color->b = 0;
        } else {
            // we use the color of the previous iteration in order to cover the full gradient range
            const double color_of_previous_iter = normalized_colors[static_cast<std::size_t>(*iter - 1)];
            const double color_of_current_iter  = normalized_colors[static_cast<std::size_t>(*iter)];
            const double pos_in_gradient = color_of_previous_iter + *smoothed_distance_to_next_iteration * (color_of_current_iter - color_of_previous_iter);

            color_from_gradient_v5(gradient, pos_in_gradient, *pixel_color);
        }

        ++pixel_color;
        ++iter;
        ++smoothed_distance_to_next_iteration;
    }
}

void mandelbrot_colorize_v7(const int max_iterations, const Gradient& gradient,
                         std::vector<PixelColor>& image_data, const std::vector<int>& histogram, const std::vector<int>& iterations_per_pixel, const std::vector<double>& smoothed_distances_to_next_iteration_per_pixel, std::vector<double>& normalized_colors)
{
    // Sum all iterations, not counting the last one at position histogram[max_iterations] (which
    // are points in the Mandelbrot Set).
    const double total_iterations = std::accumulate(std::next(histogram.cbegin()), std::prev(histogram.cend()), 0);

    // Normalize the colors (0.0 .. 1.0) based on how often they are used in the image, not counting
    // histogram[max_iterations] (which are points in the Mandelbrot Set).
    int running_total = 0;

    for (std::size_t i = 1; i < static_cast<std::size_t>(max_iterations); ++i) {
        running_total += histogram[i];
        normalized_colors[i] = running_total / total_iterations;
    }

    auto iter = iterations_per_pixel.cbegin();
    auto smoothed_distance_to_next_iteration = smoothed_distances_to_next_iteration_per_pixel.cbegin();

    for (auto& pixel_color : image_data) {
        if (*iter == max_iterations) {
            // pixels with max. iterations (aka. inside the Mandelbrot Set) are always black
            pixel_color.r = 0;
            pixel_color.g = 0;
            pixel_color.b = 0;
        } else {
            // we use the color of the previous iteration in order to cover the full gradient range
            const double color_of_previous_iter = normalized_colors[static_cast<std::size_t>(*iter - 1)];
            const double color_of_current_iter  = normalized_colors[static_cast<std::size_t>(*iter)];
            const double pos_in_gradient = color_of_previous_iter + *smoothed_distance_to_next_iteration * (color_of_current_iter - color_of_previous_iter);

            color_from_gradient_v5(gradient, pos_in_gradient, pixel_color);
        }

        ++iter;
        ++smoothed_distance_to_next_iteration;
    }
}

void mandelbrot_colorize_v8(const int max_iterations, const Gradient_v2& gradient,
                         std::vector<PixelColor>& image_data, const std::vector<int>& histogram, const std::vector<int>& iterations_per_pixel, const std::vector<float>& smoothed_distances_to_next_iteration_per_pixel, std::vector<float>& normalized_colors)
{
    // Sum all iterations, not counting the last one at position histogram[max_iterations] (which
    // are points in the Mandelbrot Set).
    const float total_iterations = std::accumulate(std::next(histogram.cbegin()), std::prev(histogram.cend()), 0);

    // Normalize the colors (0.0 .. 1.0) based on how often they are used in the image, not counting
    // histogram[max_iterations] (which are points in the Mandelbrot Set).
    int running_total = 0;

    for (std::size_t i = 1; i < static_cast<std::size_t>(max_iterations); ++i) {
        running_total += histogram[i];
        normalized_colors[i] = running_total / total_iterations;
    }

    auto iter = iterations_per_pixel.cbegin();  // in range of 1 .. max_iterations
    auto smoothed_distance_to_next_iteration = smoothed_distances_to_next_iteration_per_pixel.cbegin();  // in range of 0 .. <1.0

    for (auto& pixel : image_data) {
        if (*iter == max_iterations) {
            // pixels with max. iterations (aka. inside the Mandelbrot Set) are always black
            pixel.r = 0;
            pixel.g = 0;
            pixel.b = 0;
        } else {
            // we use the color of the previous iteration in order to cover the full gradient range
            const float color_of_previous_iter = normalized_colors[static_cast<std::size_t>(*iter - 1)];
            const float color_of_current_iter  = normalized_colors[static_cast<std::size_t>(*iter)];
            const float pos_in_gradient = color_of_previous_iter + *smoothed_distance_to_next_iteration * (color_of_current_iter - color_of_previous_iter);

            color_from_gradient_v7(gradient, pos_in_gradient, pixel);
        }

        ++iter;
        ++smoothed_distance_to_next_iteration;
    }
}

int calc_running_total_v1(const int max_iterations, const int total_iterations, std::vector<int>& histogram, std::vector<double>& normalized_colors)
{
    int running_total = 0;

    for (int i = 1; i < max_iterations; ++i) {
        running_total += histogram[static_cast<std::size_t>(i)];
        normalized_colors[static_cast<std::size_t>(i)] = static_cast<double>(running_total) / static_cast<double>(total_iterations);
    }

    return running_total;
}

int calc_running_total_v2(const int max_iterations, const double total_iterations, std::vector<int>& histogram, std::vector<double>& normalized_colors)
{
    int running_total = 0;

    for (int i = 1; i < max_iterations; ++i) {
        running_total += histogram[static_cast<std::size_t>(i)];
        normalized_colors[static_cast<std::size_t>(i)] = running_total / total_iterations;
    }

    return running_total;
}

int calc_running_total_v3(const int, const double total_iterations, std::vector<int>& histogram, std::vector<double>& normalized_colors)
{
    int running_total = 0;

    auto h = std::next(histogram.cbegin());
    auto c = std::next(normalized_colors.begin());
    auto h_end = std::prev(histogram.cend());

    for (; h != h_end; ++h, ++c) {
        running_total += *h;
        *c = running_total / total_iterations;
    }

    return running_total;
}

int calc_running_total_v4(const std::size_t max_iterations, const double total_iterations, std::vector<int>& histogram, std::vector<double>& normalized_colors)
{
    int running_total = 0;

    for (std::size_t i = 1; i < max_iterations; ++i) {
        running_total += histogram[i];
        normalized_colors[i] = running_total / total_iterations;
    }

    return running_total;
}

static void BM_EqualEnough_v1(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(equal_enough_v1(0.0000043, 0.0000044));
}

static void BM_EqualEnough_v2(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(equal_enough_v2(0.0000043, 0.0000044));
}

static void BM_EqualEnough_v3(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(equal_enough_v3(0.0000043, 0.0000044));
}

static void BM_EqualEnough_v4(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(equal_enough_v4(0.0000043, 0.0000044));
}

static void BM_EqualEnough_v5(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(equal_enough_v5(0.0000043f, 0.0000044f));
}

static void BM_TotalIterations_v1(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    unsigned char* image_data = new unsigned char[static_cast<unsigned long>(image_width * image_height * 3)];
    mandelbrot_calc_v1(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    for (auto _ : state) {
        int total_iterations = 0;

        for (int i = 1; i < max_iterations; ++i)
            total_iterations += histogram[static_cast<std::size_t>(i)];

        benchmark::DoNotOptimize(total_iterations);
    }

    delete[] image_data;
}

static void BM_TotalIterations_v2(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    unsigned char* image_data = new unsigned char[static_cast<unsigned long>(image_width * image_height * 3)];
    mandelbrot_calc_v1(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    for (auto _ : state)
        benchmark::DoNotOptimize(std::accumulate(histogram.cbegin(), histogram.cend(), 0));

    delete[] image_data;
}

static void BM_TotalIterations_v3(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    unsigned char* image_data = new unsigned char[static_cast<unsigned long>(image_width * image_height * 3)];
    mandelbrot_calc_v1(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    for (auto _ : state)
        benchmark::DoNotOptimize(std::accumulate(std::next(histogram.cbegin()), std::prev(histogram.cend()), 0));

    delete[] image_data;
}

static void BM_TotalIterations_v4(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    unsigned char* image_data = new unsigned char[static_cast<unsigned long>(image_width * image_height * 3)];
    mandelbrot_calc_v1(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    for (auto _ : state){
        double total_iterations = std::accumulate(std::next(histogram.cbegin()), std::prev(histogram.cend()), 0);
        benchmark::DoNotOptimize(total_iterations);
    }

    delete[] image_data;
}

static void BM_RunningTotal_v1(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    int counter = 0;

    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<double> normalized_colors(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    unsigned char* image_data = new unsigned char[static_cast<unsigned long>(image_width * image_height * 3)];
    mandelbrot_calc_v1(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    const int total_iterations = std::accumulate(std::next(histogram.cbegin()), std::prev(histogram.cend()), 0);

    for (auto _ : state)
        benchmark::DoNotOptimize(counter = calc_running_total_v1(max_iterations, total_iterations, histogram, normalized_colors));

    delete[] image_data;
    state.counters["counter"] = counter;
}

static void BM_RunningTotal_v2(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    int counter = 0;

    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<double> normalized_colors(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    unsigned char* image_data = new unsigned char[static_cast<unsigned long>(image_width * image_height * 3)];
    mandelbrot_calc_v1(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    const double total_iterations = std::accumulate(std::next(histogram.cbegin()), std::prev(histogram.cend()), 0);

    for (auto _ : state)
        benchmark::DoNotOptimize(counter = calc_running_total_v2(max_iterations, total_iterations, histogram, normalized_colors));

    delete[] image_data;
    state.counters["counter"] = counter;
}

static void BM_RunningTotal_v3(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    int counter = 0;

    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<double> normalized_colors(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    unsigned char* image_data = new unsigned char[static_cast<unsigned long>(image_width * image_height * 3)];
    mandelbrot_calc_v1(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    const double total_iterations = std::accumulate(std::next(histogram.cbegin()), std::prev(histogram.cend()), 0);

    for (auto _ : state)
        benchmark::DoNotOptimize(counter = calc_running_total_v3(max_iterations, total_iterations, histogram, normalized_colors));

    delete[] image_data;
    state.counters["counter"] = counter;
}

static void BM_RunningTotal_v4(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    int counter = 0;

    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<double> normalized_colors(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    unsigned char* image_data = new unsigned char[static_cast<unsigned long>(image_width * image_height * 3)];
    mandelbrot_calc_v1(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    const double total_iterations = std::accumulate(std::next(histogram.cbegin()), std::prev(histogram.cend()), 0);

    for (auto _ : state)
        benchmark::DoNotOptimize(counter = calc_running_total_v4(static_cast<std::size_t>(max_iterations), total_iterations, histogram, normalized_colors));

    delete[] image_data;
    state.counters["counter"] = counter;
}

static void BM_ColorFromGradient_v5(benchmark::State& state)
{
    Gradient gradient{make_gradient()};

    for (auto _ : state) {
        PixelColor col;
        color_from_gradient_v5(gradient, 0.0, col);
        color_from_gradient_v5(gradient, 0.1, col);
        color_from_gradient_v5(gradient, 0.5, col);
        color_from_gradient_v5(gradient, 0.55, col);
        color_from_gradient_v5(gradient, 0.8, col);
        color_from_gradient_v5(gradient, 0.95, col);
        color_from_gradient_v5(gradient, 1.0, col);
        benchmark::DoNotOptimize(col);
    }
}

static void BM_ColorFromGradient_v6(benchmark::State& state)
{
    Gradient_v2 gradient{make_gradient_v2()};

    for (auto _ : state) {
        PixelColor col;
        color_from_gradient_v6(gradient, 0.0f, col);
        color_from_gradient_v6(gradient, 0.1f, col);
        color_from_gradient_v6(gradient, 0.5f, col);
        color_from_gradient_v6(gradient, 0.55f, col);
        color_from_gradient_v6(gradient, 0.8f, col);
        color_from_gradient_v6(gradient, 0.95f, col);
        color_from_gradient_v6(gradient, 1.0f, col);
        benchmark::DoNotOptimize(col);
    }
}

static void BM_ColorFromGradient_v7(benchmark::State& state)
{
    Gradient_v2 gradient{make_gradient_v2()};

    for (auto _ : state) {
        PixelColor col;
        color_from_gradient_v7(gradient, 0.0f, col);
        color_from_gradient_v7(gradient, 0.1f, col);
        color_from_gradient_v7(gradient, 0.5f, col);
        color_from_gradient_v7(gradient, 0.55f, col);
        color_from_gradient_v7(gradient, 0.8f, col);
        color_from_gradient_v7(gradient, 0.95f, col);
        color_from_gradient_v7(gradient, 1.0f, col);
        benchmark::DoNotOptimize(col);
    }
}

static void BM_MandelbrotCalc_v1(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    for (auto _ : state)
        mandelbrot_calc_v1(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);
}

static void BM_MandelbrotCalc_v2(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    for (auto _ : state)
        mandelbrot_calc_v2(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);
}

static void BM_MandelbrotCalc_v3(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    for (auto _ : state)
        mandelbrot_calc_v3(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);
}

static void BM_MandelbrotCalc_v4(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    for (auto _ : state)
        mandelbrot_calc_v4(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);
}

static void BM_MandelbrotCalc_v5(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<float> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    for (auto _ : state)
        mandelbrot_calc_v5(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);
}

static void BM_MandelbrotColorize_v1(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<double> normalized_colors(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    Gradient gradient{make_gradient()};
    unsigned char* image_data = new unsigned char[static_cast<unsigned long>(image_width * image_height * 3)];

    mandelbrot_calc_v1(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    for (auto _ : state) {
        benchmark::DoNotOptimize(image_data);
        mandelbrot_colorize_v1(image_width, image_height, max_iterations, gradient, image_data, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel, normalized_colors);
    }

    delete[] image_data;
}

static void BM_MandelbrotColorize_v2(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<double> normalized_colors(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    Gradient gradient{make_gradient()};
    unsigned char* image_data = new unsigned char[static_cast<unsigned long>(image_width * image_height * 3)];

    mandelbrot_calc_v2(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    for (auto _ : state) {
        benchmark::DoNotOptimize(image_data);
        mandelbrot_colorize_v2(image_width, image_height, max_iterations, gradient, image_data, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel, normalized_colors);
    }

    delete[] image_data;
}

static void BM_MandelbrotColorize_v3(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<double> normalized_colors(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    Gradient gradient{make_gradient()};
    unsigned char* image_data = new unsigned char[static_cast<unsigned long>(image_width * image_height * 3)];

    mandelbrot_calc_v3(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    for (auto _ : state) {
        benchmark::DoNotOptimize(image_data);
        mandelbrot_colorize_v3(image_width, image_height, max_iterations, gradient, image_data, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel, normalized_colors);
    }

    delete[] image_data;
}

static void BM_MandelbrotColorize_v4(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<double> normalized_colors(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    Gradient gradient{make_gradient()};
    PixelColor* image_data = new PixelColor[static_cast<unsigned long>(image_width * image_height)];

    mandelbrot_calc_v4(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    for (auto _ : state) {
        benchmark::DoNotOptimize(image_data);
        mandelbrot_colorize_v4(image_width, image_height, max_iterations, gradient, image_data, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel, normalized_colors);
    }

    delete[] image_data;
}

static void BM_MandelbrotColorize_v5(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<double> normalized_colors(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));

    Gradient gradient{make_gradient()};
    PixelColor* image_data = new PixelColor[static_cast<unsigned long>(image_width * image_height)];

    mandelbrot_calc_v4(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    for (auto _ : state) {
        benchmark::DoNotOptimize(image_data);
        mandelbrot_colorize_v5(image_width, image_height, max_iterations, gradient, image_data, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel, normalized_colors);
    }

    delete[] image_data;
}

static void BM_MandelbrotColorize_v6(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<double> normalized_colors(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<PixelColor> image_data(static_cast<unsigned long>(image_width * image_height));

    Gradient gradient{make_gradient()};

    mandelbrot_calc_v4(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    for (auto _ : state) {
        benchmark::DoNotOptimize(image_data);
        mandelbrot_colorize_v6(max_iterations, gradient, image_data, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel, normalized_colors);
    }
}

static void BM_MandelbrotColorize_v7(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<double> normalized_colors(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<double> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<PixelColor> image_data(static_cast<unsigned long>(image_width * image_height));

    Gradient gradient{make_gradient()};

    mandelbrot_calc_v4(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    for (auto _ : state) {
        benchmark::DoNotOptimize(image_data);
        mandelbrot_colorize_v7(max_iterations, gradient, image_data, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel, normalized_colors);
    }
}

static void BM_MandelbrotColorize_v8(benchmark::State& state, const int image_width, const int image_height, const int max_iterations, const double center_x, const double center_y, const double height)
{
    std::vector<int> histogram(static_cast<std::size_t>(max_iterations + 1));
    std::vector<float> normalized_colors(static_cast<std::size_t>(max_iterations + 1));
    std::vector<int> iterations_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<float> smoothed_distances_to_next_iteration_per_pixel(static_cast<std::size_t>(image_width * image_height));
    std::vector<PixelColor> image_data(static_cast<unsigned long>(image_width * image_height));

    Gradient_v2 gradient{make_gradient_v2()};

    mandelbrot_calc_v5(image_width, image_height, max_iterations, center_x, center_y, height, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel);

    for (auto _ : state) {
        benchmark::DoNotOptimize(image_data);
        mandelbrot_colorize_v8(max_iterations, gradient, image_data, histogram, iterations_per_pixel, smoothed_distances_to_next_iteration_per_pixel, normalized_colors);
    }
}

BENCHMARK(BM_EqualEnough_v1);
BENCHMARK(BM_EqualEnough_v2);
BENCHMARK(BM_EqualEnough_v3);
BENCHMARK(BM_EqualEnough_v4);
BENCHMARK(BM_EqualEnough_v5);

BENCHMARK_CAPTURE(BM_TotalIterations_v1, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_TotalIterations_v2, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_TotalIterations_v3, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_TotalIterations_v4, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);

BENCHMARK_CAPTURE(BM_RunningTotal_v1, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_RunningTotal_v2, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_RunningTotal_v3, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_RunningTotal_v4, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);

BENCHMARK(BM_ColorFromGradient_v5);
BENCHMARK(BM_ColorFromGradient_v6);
BENCHMARK(BM_ColorFromGradient_v7);

BENCHMARK_CAPTURE(BM_MandelbrotCalc_v1, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_MandelbrotCalc_v2, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_MandelbrotCalc_v3, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_MandelbrotCalc_v4, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_MandelbrotCalc_v5, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);

BENCHMARK_CAPTURE(BM_MandelbrotColorize_v1, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_MandelbrotColorize_v2, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_MandelbrotColorize_v3, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_MandelbrotColorize_v4, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_MandelbrotColorize_v5, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_MandelbrotColorize_v6, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_MandelbrotColorize_v7, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);
BENCHMARK_CAPTURE(BM_MandelbrotColorize_v8, 640x480@100, 640, 480, 100, -0.8, 0.0, 2.2);

BENCHMARK_MAIN();
