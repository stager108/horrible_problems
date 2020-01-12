#include <iostream>
#include "stdlib.h"
#include "algorithm"
#include <random>
#include <memory>
#include <vector>
#include <map>

class Tree {
    std::vector<int64_t> real_length;
    std::vector<int> covering_segments_count;
    std::vector<int64_t> covered_segments_length;
    int size;

public:
    explicit Tree(size_t nn, std::vector<int> &values) {
        real_length.resize(2 * nn, 0);
        covering_segments_count.resize(2 * nn, 0);
        covered_segments_length.resize(2 * nn, 0);
        size = static_cast<int>( nn);
        for (int i = 0; i < size; ++i) {
            real_length[size + i] = values[i + 1] - values[i];
        }
        size = 2 * size;
    };

    ~Tree() = default;

    void build(int vert, int left, int right) {
        if (left != right) {
            int middle = (left + right) / 2;
            build(vert * 2, left, middle);
            build(vert * 2 + 1, middle + 1, right);
            real_length[vert] = real_length[vert * 2] + real_length[vert * 2 + 1];
        }
    }

    void add(int vert, int tleft, int trigth, int left, int right) {
        if (left > right)
            return;
        if ((left == tleft) and (right == trigth)) {
            if (covering_segments_count[vert] == 0) {
                covered_segments_length[vert] = real_length[vert];
            }
            ++covering_segments_count[vert];
            return;
        }
        int tmiddle = (tleft + trigth) / 2;
        add(vert * 2, tleft, tmiddle, left, std::min(right, tmiddle));
        add(vert * 2 + 1, tmiddle + 1, trigth, std::max(left, tmiddle + 1), right);
        if (covering_segments_count[vert] == 0) {
            covered_segments_length[vert] = covered_segments_length[vert * 2] +
                                            covered_segments_length[vert * 2 + 1];
        }
    }

    void remove(int vert, int tleft, int trigth, int left, int right) {
        if (left > right)
            return;
        if ((left == tleft) and (right == trigth)) {
            --covering_segments_count[vert];
            if (covering_segments_count[vert] == 0) {
                covered_segments_length[vert] = 0;
                if (vert * 2 < size) {
                    covered_segments_length[vert] += covered_segments_length[vert * 2];
                }
                if (vert * 2 + 1 < size) {
                    covered_segments_length[vert] += covered_segments_length[vert * 2 + 1];
                }
            }
            return;
        }
        int tmiddle = (tleft + trigth) / 2;
        remove(vert * 2, tleft, tmiddle, left, std::min(right, tmiddle));
        remove(vert * 2 + 1, tmiddle + 1, trigth, std::max(left, tmiddle + 1), right);
        if (covering_segments_count[vert] == 0) {
            covered_segments_length[vert] = covered_segments_length[vert * 2] +
                                            covered_segments_length[vert * 2 + 1];
        }
    }

    int64_t get_length() {
        return covered_segments_length[1];
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n_value;
    std::cin >> n_value;
    std::vector<int> begins(static_cast<size_t>(n_value));
    std::vector<int> ends(static_cast<size_t>(n_value));
    std::vector<char> operations(static_cast<size_t>(n_value));
    std::vector<int> elem(static_cast<size_t>(n_value * 2));
    int xf, xs;
    char op;
    for (int i = 0; i < n_value; ++i) {
        std::cin >> op >> xf >> xs;
        begins[i] = xf;
        ends[i] = xs;
        operations[i] = op;
        elem[2 * i] = xf;
        elem[2 * i + 1] = xs;
    }
    std::sort(elem.begin(), elem.end());
    std::map<int, int> dict;
    int kk = 0;
    dict[elem[0]] = 0;
    ++kk;
    for (int i = 1; i < 2 * n_value; ++i) {
        if (elem[i] != elem[i - 1]) {
            dict[elem[i]] = kk;
            ++kk;
        }
    }

    int aa = 1;
    while (aa < kk) {
        aa *= 2;
    }

    std::vector<int> dop;
    dop.resize(static_cast<unsigned int>(aa + 1), elem[2 * n_value - 1]);

    kk = 0;
    dop[0] = elem[0];
    ++kk;
    for (int i = 1; i < 2 * n_value; ++i) {
        if (elem[i] != elem[i - 1]) {
            dop[kk] = elem[i];
            ++kk;
        }
    }
    elem.clear();

    Tree my_tree(static_cast<size_t>(aa), dop);
    dop.clear();

    my_tree.build(1, 0, aa - 1);
    for (int i = 0; i < n_value; ++i) {
        if (operations[i] == '+') {
            my_tree.add(1, 0, aa - 1, dict[begins[i]], dict[ends[i]] - 1);
        } else {
            my_tree.remove(1, 0, aa - 1, dict[begins[i]], dict[ends[i]] - 1);
        }
        std::cout << my_tree.get_length();
        if (i != n_value - 1) {
            std::cout << '\n';
        }
    }
    return 0;
}
