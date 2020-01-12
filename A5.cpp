#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

class Sets {
    std::vector<int> parents;

public:
    explicit Sets() = default;

    ~Sets() = default;

    void make_sets(int size) {
        parents.resize(static_cast<unsigned int>(size));
        for (int i = 0; i < size; ++i) {
            parents[i] = i;
        }
    }

    int find_set(int vertex) {
        if (vertex == parents[vertex]) {
            return vertex;
        } else {
            parents[vertex] = find_set(parents[vertex]);
            return parents[vertex];
        }
    }

    int union_sets(int first, int second) {
        int parent_f = find_set(first);
        int parent_s = find_set(second);
        if ((first != second) and (second == parent_s) and (parent_f != parent_s)) {
            parents[parent_s] = parent_f;
            return 1;
        } else {
            return 0;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Sets my_set;
    int n_value, m_value;
    std::cin >> n_value >> m_value;
    my_set.make_sets(n_value);
    std::string str;
    std::getline(std::cin, str);
    for (int ind = 0; ind < m_value; ++ind) {
        std::stringstream ss;
        std::getline(std::cin, str);
        ss << str;
        int first = 0, second = -1;
        ss >> first;
        --first;
        if (!ss.eof()) {
            ss >> second;
            --second;
        }
        if (second == -1) {
            std::cout << my_set.find_set(first) + 1;
        } else {
            std::cout << my_set.union_sets(first, second);
        }
        if (ind != m_value - 1) {
            std::cout << '\n';
        }
    }
    return 0;
}
