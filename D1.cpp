#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iomanip>

struct my_pair {
    int x_coord = 0;
    int y_coord = 0;
};

struct my_segment {
    double left = 0.0;
    double right = 0.0;
};

std::vector<my_pair> read_points(int number) {
    std::vector<my_pair> points(static_cast<size_t>(number));
    for (int i = 0; i < number; ++i) {
        std::cin >> points[i].x_coord >> points[i].y_coord;
    }
    return points;
}

my_segment get_segment(my_pair point, double radius) {
    my_segment segment;
    double length = radius * radius - point.y_coord * point.y_coord;
    if (length >= 0) {
        double distance = sqrt(length);
        segment.left = static_cast<double>(point.x_coord) - distance;
        segment.right = static_cast<double>(point.x_coord) + distance;
    } else {
        segment.left = -300000.0;
        segment.right = -300000.0;
    }
    return segment;
}

int scanline(std::vector<double> &left_ends, std::vector<double> &right_ends, int size) {
    int count = 0;
    int answer = 0;
    std::sort(left_ends.begin(), left_ends.end());
    std::sort(right_ends.begin(), right_ends.end());

    int left_index = 0, right_index = 0;
    while ((left_index < size) and (right_index < size)) {
        if (left_ends[left_index] <= right_ends[right_index]) {
            ++count;
            ++left_index;
            if (count > answer) {
                answer = count;
            }
        } else {
            --count;
            ++right_index;
        }
    }
    return answer;
}

double get_answer(std::vector<my_pair> &points, int k_value, int size) {
    double left = 0.0;
    double right = 2000.0;
    while (right - left > 10e-6) {
        double middle = (right + left) / 2;
        std::vector<double> left_ends;
        std::vector<double> right_ends;
        int count = 0;
        for (int i = 0; i < size; ++i) {
            my_segment new_segment = get_segment(points[i], middle);
            if (new_segment.left > -300000.0) {
                left_ends.push_back(new_segment.left);
                right_ends.push_back(new_segment.right);
                ++count;
            }
        }
        int current_k = scanline(left_ends, right_ends, count);
        if (current_k < k_value) {
            left = middle;
        } else {
            right = middle;
        }
    }
    return right;
}


int main() {
    std::ios::sync_with_stdio(false);
    int n_value, k_value;
    std::cin >> n_value >> k_value;
    std::vector<my_pair> points = read_points(n_value);
    double answer = get_answer(points, k_value, n_value);
    std::cout << std::fixed << std::setprecision(6) << answer;
    return 0;
}
