#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <string>

class Solver {

    struct my_pair {
        int number = 0;
        std::string hash = "";

        static bool comparator(const my_pair &first, const my_pair &second) {
            return first.hash < second.hash;
        }
    };

    struct TreeNode {
        std::string hash = "";
        std::vector<my_pair> children;

        void add(int &number) {
            my_pair next;
            next.hash = "";
            next.number = number;
            children.push_back(next);
        }
    };

    void readTree(std::vector<TreeNode> &answer, int size) {
        int first, second;
        for (int i = 1; i < size; ++i) {
            std::cin >> first >> second;
            --first;
            --second;
            answer[first].add(second);
            answer[second].add(first);
        }
    }

    void dfs(std::vector<TreeNode> &node, int index, int prev, int dist) {
        int size = static_cast<int>(node[index].children.size());
        depth[index] = dist;
        for (int i = 0; i < size; ++i) {
            if (node[index].children[i].number != prev) {
                int next = node[index].children[i].number;
                dfs(node, next, index, dist + 1);
            }
        }
    }

    bool dfs_path(std::vector<TreeNode> &node, std::vector<int> &path,
                  int index, int prev, int target) {
        int size = static_cast<int>(node[index].children.size());
        path.push_back(index);
        bool ans = false;
        if (index == target) {
            return true;
        }
        for (int i = 0; i < size; ++i) {
            if (node[index].children[i].number != prev) {
                int next = node[index].children[i].number;
                if (dfs_path(node, path, next, index, target)) {
                    ans = true;
                    break;
                };
            }
        }
        if (not(ans)) {
            path.pop_back();
            return false;
        } else {
            return true;
        }
    }

    std::vector<int> getCenter(std::vector<TreeNode> &node, int size) {
        dfs(node, 0, -1, 0);
        int max_depth = 0;
        int vert = -1;
        for (int i = 0; i < size; ++i) {
            if (depth[i] > max_depth) {
                max_depth = depth[i];
                vert = i;
            }
        }
        max_depth = -1;
        dfs(node, vert, -1, 0);
        int farrest = 0;
        for (int i = 0; i < size; ++i) {
            if (depth[i] > max_depth) {
                max_depth = depth[i];
                farrest = i;
            }
        }
        std::vector<int> path;
        path.reserve(static_cast<size_t>(size));
        dfs_path(node, path, vert, -1, farrest);
        std::vector<int> answer;
        int length = static_cast<int>(path.size());
        answer.push_back(path[(length - 1) / 2]);
        if (length % 2 == 0) {
            answer.push_back(path[length / 2]);
        }
        return answer;
    }

    void getHash(std::vector<TreeNode> &node, int index, int prev) {
        int size = static_cast<int>(node[index].children.size());
        std::string curhash = "(";
        for (int i = 0; i < size; ++i) {
            if (node[index].children[i].number != prev) {
                int next = node[index].children[i].number;
                getHash(node, next, index);
            } else {
                node[index].children[i].hash = "";
            }
        }
        std::sort(node[index].children.begin(), node[index].children.end(), my_pair::comparator);
        for (int i = 0; i < size; ++i) {
            curhash += node[index].children[i].hash;
            node[index].children[i].hash = "";
        }
        curhash += ")";
        node[index].hash = curhash;
    }

    bool get_string(std::vector<TreeNode> &first, std::vector<TreeNode> &second,
                    int center_f, int center_s) {

        getHash(first, center_f, -1);
        getHash(second, center_s, -1);
        auto hashf = first[center_f].hash;
        auto hashs = second[center_s].hash;
        bool ans = true;
        if (hashf != hashs) {
            return false;
        }
        return ans;
    }

    void subprint(std::vector<int> &answer, std::vector<TreeNode> &ft,
                  std::vector<TreeNode> &st, int node_f, int node_s, int prev) {
        answer[node_f] = node_s;
        int subsize = static_cast<int>(ft[node_f].children.size());
        for (int i = 0; i < subsize; ++i) {
            if (ft[node_f].children[i].number != prev) {
                subprint(answer, ft, st, ft[node_f].children[i].number,
                         st[node_s].children[i].number, node_f);
            }
        }
    }

    void print_answer(std::vector<TreeNode> &ft, std::vector<TreeNode> &st,
                      int cf, int cs, int size) {
        std::vector<int> answer(static_cast<size_t>(size));
        answer[cf] = cs;
        int subsize = static_cast<int>(ft[cf].children.size());
        for (int i = 0; i < subsize; ++i) {
            subprint(answer, ft, st, ft[cf].children[i].number,
                     st[cs].children[i].number, cf);
        }

        for (int i = 0; i < size - 1; ++i) {
            std::cout << answer[i] + 1 << "\n";
        }
        if (size > 0) {
            std::cout << answer[size - 1] + 1;
        }
    }

    void init_depth(int size) {
        depth.resize(static_cast<size_t>(size), 0);
    }

    void clear_depth() {
        depth.clear();
    }

    std::vector<int> depth;

public:
    void solve() {
        int size;
        std::cin >> size;

        std::vector<TreeNode> first_tree(static_cast<size_t>(size)),
                second_tree(static_cast<size_t>(size));
        readTree(first_tree, size);
        readTree(second_tree, size);
        init_depth(size);

        auto center_f = getCenter(first_tree, size);
        auto center_s = getCenter(second_tree, size);
        clear_depth();

        if (center_f.size() != center_s.size()) {
            std::cout << "-1";
        } else {
            if (center_s.size() == 2) {

                if (get_string(first_tree, second_tree, center_f[0], center_s[0])) {
                    print_answer(first_tree, second_tree, center_f[0], center_s[0], size);
                } else {
                    if (get_string(first_tree, second_tree, center_f[0], center_s[1])) {
                        print_answer(first_tree, second_tree,
                                     center_f[0], center_s[1], size);
                    } else {
                        std::cout << -1;
                    }
                }
            } else {
                if (get_string(first_tree, second_tree, center_f[0], center_s[0])) {
                    print_answer(first_tree, second_tree, center_f[0], center_s[0], size);
                } else {
                    std::cout << -1;
                }
            }
        }
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Solver solver;
    solver.solve();
    return 0;
}
