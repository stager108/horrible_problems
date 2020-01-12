#include <iostream>
#include "stdlib.h"
#include "algorithm"
#include <random>
#include <memory>
#include <vector>

struct my_pair {
    int x_f = 0, y_f = 0, index = 0;
    bool end = false;

    void init(int xf, int yf, int ind, bool is_end) {
        x_f = xf;
        y_f = yf;
        index = ind;
        end = is_end;
    }

    static bool comparator(const my_pair &first, const my_pair &second) {
        if (first.x_f == second.x_f) {
            return first.y_f < second.y_f;
        } else {
            return first.x_f < second.x_f;
        }
    }
};

struct Square {
    int x_f = 0, x_s = 0, y_f = 0, y_s = 0;

    void init(int xf, int yf, int xs, int ys) {
        x_f = std::min(xf, xs);
        x_s = std::max(xf, xs);
        y_f = std::min(yf, ys);
        y_s = std::max(yf, ys);
    }

    static bool comparator(const Square &first, const Square &second) {
        if (first.y_f == second.y_f) {
            return first.x_f < second.x_f;
        } else {
            return first.y_f < second.y_f;
        }
    }

    static bool contains(const Square &first, const Square &second) {
        return (first.x_f < second.x_f) and (first.x_s > second.x_s) and
               (first.y_f < second.y_f) and (first.y_s > second.y_s);
    }

    static bool equal(const Square &first, const Square &second) {
        return (first.x_f == second.x_f) and (first.x_s == second.x_s) and
               (first.y_f == second.y_f) and (first.y_s == second.y_s);
    }
};


class CartesianTree {

private:
    struct Node {
        int value = 0;
        Square key;
        int subTreeSize = 0;
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
    };

    int count = 0;
    std::shared_ptr<Node> TreeRoot = nullptr;
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;

    struct dpair {
        std::shared_ptr<Node> first = nullptr;
        std::shared_ptr<Node> second = nullptr;
    };

    int LeftSubTreeSize(std::shared_ptr<Node> &node) {
        if (node == nullptr) {
            return 0;
        } else {
            if (node->left != nullptr) {
                return node->left->subTreeSize;
            } else {
                return 0;
            }
        }
    }

    int RightSubTreeSize(std::shared_ptr<Node> &node) {
        if (node == nullptr) {
            return 0;
        } else {
            if (node->right != nullptr) {
                return node->right->subTreeSize;
            } else {
                return 0;
            }
        }
    }

    void NewInformation(std::shared_ptr<Node> &node) {
        if (node != nullptr) {
            node->subTreeSize = LeftSubTreeSize(node) +
                                RightSubTreeSize(node) + 1;
        }
    }

    dpair SplitByKey(std::shared_ptr<Node> &root, Square key) {
        dpair answer;
        if (root == nullptr) {
            return answer;
        }
        if (Square::comparator(root->key, key)) {
            answer = SplitByKey(root->right, key);
            NewInformation(answer.first);
            NewInformation(answer.second);
            root->right = answer.first;
            answer.first = root;

        } else {
            answer = SplitByKey(root->left, key);
            NewInformation(answer.first);
            NewInformation(answer.second);
            root->left = answer.second;
            answer.second = root;
        }
        NewInformation(root);
        return answer;
    }

    dpair SplitBySize(std::shared_ptr<Node> &root, int pos) {
        dpair answer;
        if (root == nullptr) {
            return answer;
        }
        int left_size;
        if (root->left == nullptr) {
            left_size = 0;
        } else {
            left_size = root->left->subTreeSize;
        }

        if (left_size >= pos) {
            answer = SplitBySize(root->left, pos);
            root->left = answer.second;
            answer.second = root;

        } else {
            answer = SplitBySize(root->right, pos - left_size - 1);
            root->right = answer.first;
            answer.first = root;
        }
        NewInformation(root);
        return answer;
    }

    std::shared_ptr<Node> Merge(std::shared_ptr<Node> First,
                                std::shared_ptr<Node> Second) {
        if (First == nullptr) return Second;
        if (Second == nullptr) return First;
        if (First->value > Second->value) {
            First->right = Merge(First->right, Second);
            NewInformation(First);
            return First;
        } else {
            Second->left = Merge(First, Second->left);
            NewInformation(Second);
            return Second;
        }
    }

public:
    explicit CartesianTree() = default;

    ~CartesianTree() = default;

    void PrintCount() {
        std::cout << count;
    }

    void Insert(Square key) {
        auto splt_f = SplitByKey(TreeRoot, key);
        dpair splt_s;
        if (splt_f.first != nullptr) {
            splt_s = SplitBySize(splt_f.first, splt_f.first->subTreeSize - 1);
        }

        if ((splt_s.second == nullptr) or (not Square::contains(splt_s.second->key, key))) {
            std::shared_ptr<Node> NewNode(new Node);
            NewNode->value = distribution(generator);
            NewNode->key = key;
            NewNode->subTreeSize = 1;
            splt_f.first = Merge(splt_s.first, splt_s.second);
            splt_f.second = Merge(NewNode, splt_f.second);
            TreeRoot = Merge(splt_f.first, splt_f.second);
        } else {
            splt_f.first = Merge(splt_s.first, splt_s.second);
            TreeRoot = Merge(splt_f.first, splt_f.second);
        }
    }

    void Delete(Square key) {

        auto splt_f = SplitByKey(TreeRoot, key);
        auto splt_s = SplitBySize(splt_f.second, 1);
        if ((splt_s.first == nullptr) or (not Square::equal(splt_s.first->key, key))) {
            splt_f.second = Merge(splt_s.first, splt_s.second);
            TreeRoot = Merge(splt_f.first, splt_f.second);
        } else {
            TreeRoot = Merge(splt_f.first, splt_s.second);
            ++count;
        }
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    CartesianTree tree;
    int n_value;
    std::cin >> n_value;
    std::vector<Square> values(static_cast<size_t>(n_value));
    std::vector<my_pair> pairs(static_cast<size_t>(2 * n_value));
    int xf, xs, yf, ys;
    for (int i = 0; i < n_value; ++i) {
        std::cin >> xf >> yf >> xs >> ys;
        values[i].init(xf, yf, xs, ys);
        pairs[2 * i].init(std::min(xf, xs), std::min(ys, yf), i, false);
        pairs[2 * i + 1].init(std::max(xf, xs), std::min(yf, ys), i, true);
    }
    std::sort(pairs.begin(), pairs.end(), my_pair::comparator);

    for (int i = 0; i < 2 * n_value; ++i) {
        if (pairs[i].end) {
            tree.Delete(values[pairs[i].index]);
        } else {
            tree.Insert(values[pairs[i].index]);
        }
    }
    tree.PrintCount();

    return 0;
}
