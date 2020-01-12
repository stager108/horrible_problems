#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

class window {
    int ind = 0, size = 0, jind = 0;
    int64_t cur_maximum = 0, cost = 0, window_size = 0;
    std::deque<int64_t> maximums;

public:
    void initialize(int64_t mw, int64_t cs) {
        window_size = mw;
        cost = cs;
        ind = -1;
        size = 0;
        jind = -1;
        cur_maximum = -1;
        maximums.clear();
    };

    explicit window() = default;

    ~window() = default;

    int64_t step(int64_t value) {

        int64_t answer;
        ++size;
        maximums.push_back(value);
        cur_maximum += cost;

        if (size > window_size) {
            maximums.pop_front();
            --size;
            --ind;
            --jind;
        }

        if ((ind < 0) or (cur_maximum < value)) {
            cur_maximum = value;
            ind = size - 1;
        }

        if (jind < 0) {
            maximums[size - 1] = value;
            for (int j = size - 2; j >= 0; --j) {
                if (maximums[j + 1] > maximums[j] + cost) {
                    maximums[j] = maximums[j + 1] - cost;
                }
            }
            cur_maximum = -1;
            ind = -1;
            jind = size - 1;
        }
        if (maximums[0] + cost * (size - 1) > cur_maximum) {
            answer = maximums[0] + cost * (size - 1);
        } else {
            answer = cur_maximum;
        }
        return answer;
    }
};


void solve() {
    int k_value, s_value;
    int64_t cursum, cost, weight, number;
    window wind;
    std::cin >> k_value >> s_value;
    ++s_value;
    std::vector<int64_t> answer(static_cast<size_t>(s_value), 0);
    std::vector<int64_t> answer_n(static_cast<size_t>(s_value), 0);

    for (int ind = 0; ind < k_value; ++ind) {
        std::cin >> weight >> cost >> number;

        number = std::min(s_value / weight, number);

        for (int i = 0; i < s_value; ++i) {
            answer[i] = answer_n[i];
        }

        for (int i = 0; i < weight; ++i) {
            wind.initialize(number + 1, cost);
            for (int jj = i; jj < s_value; jj += weight) {
                cursum = wind.step(answer[jj]);
                if (answer_n[jj] < cursum) {
                    answer_n[jj] = cursum;
                }
            }
        }
    }

    cursum = answer_n[0];
    for (int i = 1; i < s_value; ++i) {
        if (cursum < answer_n[i]) {
            cursum = answer_n[i];
        }
    }
    std::cout << cursum;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    solve();
    return 0;
}
