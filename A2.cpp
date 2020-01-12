#include <iostream>
#include <vector>
#include <algorithm>

void print_answers(const std::vector<int> &values, const std::vector<char> &actions,
                   int nn, int mm) {

    std::vector<int> maximums(static_cast<size_t>(nn), -1000000002);
    std::vector<int> cur_maximums(static_cast<size_t>(mm), -1000000002);
    int cur_maximum = values[0];
    int left = 0, right = 0;

    for (int ind = 0; ind < mm; ++ind) {

        if (actions[ind] == 'L') {
            ++left;
            if (maximums[left] == -1000000002) {
                maximums[right] = values[right];
                for (int j = right - 1; j >= left; --j) {
                    maximums[j] = std::max(maximums[j + 1], values[j]);
                }
                cur_maximum = -1000000002;
            }
            cur_maximums[ind] = std::max(maximums[left], cur_maximum);

        } else {
            ++right;
            cur_maximum = std::max(cur_maximum, values[right]);
            cur_maximums[ind] = std::max(cur_maximum, maximums[left]);
        }
    }
    for (int i = 0; i < mm - 1; ++i) {
        std::cout << cur_maximums[i] << ' ';
    }
    if (mm > 0) {
        std::cout << cur_maximums[mm - 1];
    }
}


int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n_value;
    int m_value;
    std::cin >> n_value;
    std::vector<int> values(static_cast<size_t>(n_value));
    for (int i = 0; i < n_value; ++i) {
        std::cin >> values[i];
    }
    std::cin >> m_value;
    std::vector<char> actions(static_cast<size_t>(m_value));

    for (int i = 0; i < m_value; ++i) {
        std::cin >> actions[i];
    }
    print_answers(values, actions, n_value, m_value);
}
