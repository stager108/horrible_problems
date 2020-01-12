#include <iostream>
#include "stdlib.h"
#include "algorithm"
#include <random>
#include <memory>
#include <vector>
#include <iomanip>


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t n_value, h_value;
    std::cin >> n_value >> h_value;
    if (n_value < h_value) {
        std::cout << "0\n";
    } else {
        std::vector<std::vector<double>> answer(n_value + 2);
        answer[0].resize(h_value + 2, 1);
        answer[1].resize(h_value + 2, 1);
        answer[1][0] = 0;
        for (size_t i = 2; i < n_value + 2; ++i) {
            answer[i].resize(h_value + 2, 0);
        }
        for (size_t ii = 2; ii < n_value + 1; ++ii) {
            for (size_t jj = 1; jj < h_value + 2; ++jj) {
                answer[ii][jj] = 0;
                for (size_t kk = 1; kk <= ii; ++kk) {
                    answer[ii][jj] += answer[kk - 1][jj - 1] * answer[ii - kk][jj - 1];
                }
                answer[ii][jj] /= ii;
            }
        }
        std::cout << std::fixed << std::setprecision(10) <<
                  answer[n_value][h_value + 1] - answer[n_value][h_value];
    }
    return 0;
}
