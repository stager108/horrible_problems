#include <iostream>
#include <vector>
#include <algorithm>

void MergeSort(int start, int finish, std::vector<int64_t> &values,
               std::vector<int64_t> &buffer, int64_t &answer, int64_t xx, int64_t yy) {
    if (start + 1 < finish) {
        int left = start + (finish - start) / 2;
        MergeSort(start, left, values, buffer, answer, xx, yy);
        MergeSort(left, finish, values, buffer, answer, xx, yy);

        int jj = left;
        for (int i = start; i < left; ++i) {
            while ((jj < finish) and (values[jj] - values[i] < xx)) {
                ++jj;
            }
            answer += finish - jj + 1;
        }

        jj = left;
        for (int i = start; i < left; ++i) {
            while ((jj < finish) and (values[jj] - values[i] < yy)) {
                ++jj;
            }
            answer -= finish - jj + 1;
        }

        int ind = start, jnd = left, kk = 0;
        while ((ind < left) && (jnd < finish)) {
            if (values[ind] > values[jnd]) {
                buffer[kk] = values[jnd];
                ++jnd;
                ++kk;
            } else {
                buffer[kk] = values[ind];
                ++ind;
                ++kk;
            }
        }
        if (ind < left) {
            for (int i = ind; i < left; ++i) {
                buffer[kk] = values[i];
                ++kk;
            }
        } else {
            for (int i = jnd; i < finish; ++i) {
                buffer[kk] = values[i];
                ++kk;
            }
        }
        for (int i = 0; i < kk; ++i) {
            values[start + i] = buffer[i];
        }
    } else {
        if ((values[start] >= xx) and (values[start] < yy)) {
            ++answer;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int64_t xx_f, xx_s;
    int n_value;
    std::cin >> n_value;
    std::vector<int64_t> values(static_cast<size_t>(n_value));
    std::vector<int64_t> buffer(static_cast<size_t>(n_value));
    std::cin >> values[0];
    for (int i = 1; i < n_value; ++i) {
        std::cin >> values[i];
        values[i] += values[i - 1];
    }
    std::cin >> xx_f >> xx_s;
    int64_t answer = 0;
    MergeSort(0, n_value, values, buffer, answer, xx_f, xx_s + 1);
    std::cout << answer;
    return 0;
}
