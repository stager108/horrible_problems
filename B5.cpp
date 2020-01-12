#include <iostream>
#include "stdlib.h"
#include "algorithm"
#include <random>
#include <string>
#include <memory>
#include <vector>

class CartesianTree {
    struct Node {
        int value = 0;
        char key = 0;
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

    void Insert(char key) {
        std::shared_ptr<Node> NewNode(new Node);
        NewNode->value = distribution(generator);
        NewNode->key = key;
        NewNode->subTreeSize = 1;
        TreeRoot = Merge(TreeRoot, NewNode);
    }


    char Delete() {
        auto splt_f = SplitBySize(TreeRoot, 1);
        TreeRoot = splt_f.second;
        return splt_f.first->key;
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

    void ProcessRequest(int first, int last, int shift) {
        auto splt_f = SplitBySize(TreeRoot, last);
        auto splt_s = SplitBySize(splt_f.first, first);
        auto splt_t = SplitBySize(splt_s.second, shift);
        splt_s.second = Merge(splt_t.second, splt_t.first);
        splt_f.first = Merge(splt_s.first, splt_s.second);
        TreeRoot = Merge(splt_f.first, splt_f.second);
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    CartesianTree tree;

    std::string sequence;
    std::cin >> sequence;

    int size = static_cast<int>(sequence.size());
    for (int i = 0; i < size; ++i) {
        tree.Insert(sequence[i]);
    }
    int n_value;
    std::vector<int> start, finish, shift;
    std::cin >> n_value;
    start.resize(static_cast<unsigned int>(n_value));
    finish.resize(static_cast<unsigned int>(n_value));
    shift.resize(static_cast<unsigned int>(n_value));
    for (int ind = 0; ind < n_value; ++ind) {
        std::cin >> start[ind] >> finish[ind] >> shift[ind];
    }
    for (int ind = n_value - 1; ind >= 0; --ind) {
        tree.ProcessRequest(start[ind] - 1, finish[ind], shift[ind]);
    }

    for (int i = 0; i < size; ++i) {
        sequence[i] = tree.Delete();
    }

    std::cout << sequence;
    return 0;
}
