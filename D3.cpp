#include <iostream>
#include "stdlib.h"
#include "algorithm"
#include <random>
#include <memory>
#include <vector>

class CartesianTree {
    struct Node {
        int value = 0;
        int key = 0;
        int subTreeSize = 0;
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
    };

private:
    std::shared_ptr<Node> TreeRoot = nullptr;
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;

    struct dpair {
        std::shared_ptr<Node> first = nullptr;
        std::shared_ptr<Node> second = nullptr;
    };

public:
    explicit CartesianTree() = default;

    ~CartesianTree() = default;

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

    dpair SplitByKey(std::shared_ptr<Node> &root, int key) {
        dpair answer;
        if (root == nullptr) {
            return answer;
        }
        if (root->key < key) {
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

    void Insert(int key) {
        std::shared_ptr<Node> NewNode(new Node);
        NewNode->value = distribution(generator);
        NewNode->key = key;
        NewNode->subTreeSize = 1;
        auto splitted = SplitByKey(TreeRoot, key);
        splitted.first = Merge(splitted.first, NewNode);
        TreeRoot = Merge(splitted.first, splitted.second);
    }

    void Delete(int key) {
        auto splt_f = SplitByKey(TreeRoot, key);
        auto splt_s = SplitBySize(splt_f.second, 1);
        TreeRoot = Merge(splt_f.first, splt_s.second);
    }

    int SearchBySize(int pos) {
        auto splt_f = SplitBySize(TreeRoot, pos - 1);
        auto splt_s = SplitBySize(splt_f.second, 1);
        int ans;
        if (splt_s.first != nullptr) {
            ans = splt_s.first->key;
        } else {
            ans = -1;
        }
        TreeRoot = Merge(splt_f.first, splt_s.first);
        TreeRoot = Merge(TreeRoot, splt_s.second);
        return ans;
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    CartesianTree tree;
    int n_value, m_value, k_value;
    std::cin >> n_value >> m_value >> k_value;
    std::vector<int> values(static_cast<size_t>(n_value));
    std::vector<int> answers(static_cast<size_t>(m_value));
    char command;
    for (int i = 0; i < n_value; ++i) {
        std::cin >> values[i];
    }
    int left = 0, right = 0;
    tree.Insert(values[right]);
    ++right;
    for (int ind = 0; ind < m_value; ++ind) {
        std::cin >> command;
        if (command == 'R') {
            tree.Insert(values[right]);
            ++right;
        } else {
            tree.Delete(values[left]);
            ++left;
        }
        if (right - left >= k_value) {
            answers[ind] = tree.SearchBySize(k_value);
        } else {
            answers[ind] = -1;
        }
    }
    for (int i = 0; i < m_value - 1; ++i) {
        std::cout << answers[i] << '\n';
    }
    if (m_value > 0) {
        std::cout << answers[m_value - 1];
    }
    return 0;
}
