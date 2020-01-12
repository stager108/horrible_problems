#include <iostream>
#include <vector>

std::vector<int> read_sequence(int number) {
    std::vector<int> sequence(static_cast<size_t>(number));
    int value;
    for (int i = 0; i < number; ++i) {
        std::cin >> value;
        sequence[i] = value;
    }
    return sequence;
}

std::vector<std::vector<int>> read_array(const int number, const int length) {
    std::vector<std::vector<int>> arrays(static_cast<size_t>(number));
    for (int i = 0; i < number; ++i) {
        arrays[i] = read_sequence(length);
    }
    return arrays;
}


int get_index(const std::vector<int> &a_arrays, const std::vector<int> &b_arrays, int length) {
    int answer;
    if (a_arrays[0] >= b_arrays[0]) {
        answer = 0;
    } else {
        if (a_arrays[length - 1] <= b_arrays[length - 1]) {
            answer = length - 1;
        } else {
            int left = 0, right = length - 1;

            while (right - left > 1) {
                int middle = (left + right) / 2;
                if (a_arrays[middle] <= b_arrays[middle]) {
                    left = middle;
                } else {
                    right = middle;
                }
            }
            answer = (b_arrays[left] > a_arrays[right]) ? right : left;
        }
    }
    return answer + 1;
}


int main() {
    std::ios::sync_with_stdio(false);
    int n_value, m_value, l_value, k_value;
    int x_value, y_value;
    std::vector<int> answers;
    answers.clear();

    std::cin >> n_value >> m_value >> l_value;
    std::vector<std::vector<int>> a_arrays = read_array(n_value, l_value);
    std::vector<std::vector<int>> b_arrays = read_array(m_value, l_value);

    std::cin >> k_value;

    for (int i = 0; i < k_value; ++i) {
        std::cin >> x_value >> y_value;
        answers.push_back(get_index(a_arrays[x_value - 1], b_arrays[y_value - 1], l_value));
    }

    for (int i = 0; i < k_value - 1; ++i) {
        std::cout << answers[i] << '\n';
    }
    std::cout << answers[k_value - 1];

    return 0;
}
