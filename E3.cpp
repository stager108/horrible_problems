#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

int division(int a_value, int b_value) {
    while (b_value) {
        a_value %= b_value;
        std::swap(a_value, b_value);
    }
    return a_value;
}

struct Triangle {
    std::vector<int> values;

    void Init(int one, int two, int three) {
        values.resize(3);
        int dgc = division(division(one, two), three);
        values[0] = one / dgc;
        values[1] = two / dgc;
        values[2] = three / dgc;
        std::sort(values.begin(), values.end());
    }

    int GetHash(const size_t size) const {
        const int HashParameter = 91;
        int hash = 0;
        for (int i = 0; i < 3; ++i) {
            hash = static_cast<int>((hash * HashParameter + values[i]) % size);
        }
        return hash;
    }

    bool IsEqual(Triangle &another) const {
        return (another.values[0] == values[0]) and
               (another.values[1] == values[1]) and
               (another.values[2] == values[2]);
    }
};


class CChainHashTable {
private:
    struct CListNode {
        Triangle Key;
        std::shared_ptr<CListNode> Next;

        explicit CListNode(const Triangle &key) {
            Key = key;
            Next = nullptr;
        }

        ~CListNode() = default;
    };

    std::vector<std::shared_ptr<CListNode>> table;
    size_t table_size;
    int answer;

public:
    explicit CChainHashTable(size_t initialSize) {
        table.resize(initialSize, nullptr);
        table_size = initialSize;
        answer = 0;
    }

    ~CChainHashTable() = default;

    bool Has(const Triangle &value) const {
        const int hash = value.GetHash(table_size);
        for (std::shared_ptr<CListNode> current = table[hash];
             current != nullptr; current = current->Next) {
            if (value.IsEqual(current->Key)) {
                return true;
            }
        }
        return false;
    }

    bool Add(const Triangle &key) {
        const int hash = key.GetHash(table_size);
        for (std::shared_ptr<CListNode> current = table[hash];
             current != nullptr; current = current->Next) {
            if (key.IsEqual(current->Key)) {
                return false;
            }
        }
        std::shared_ptr<CListNode> newNode(new CListNode(key));
        newNode->Next = table[hash];
        table[hash] = newNode;
        ++answer;
        return true;
    }

    void PrintAnswer() {
        std::cout << answer;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n_value;
    std::cin >> n_value;
    CChainHashTable table(static_cast<size_t>(5 * n_value));
    int av, bv, cv;
    for (int i = 0; i < n_value; ++i) {
        std::cin >> av >> bv >> cv;
        Triangle new_triangle;
        new_triangle.Init(av, bv, cv);
        if (not table.Has(new_triangle)) {
            table.Add(new_triangle);
        }
    }
    table.PrintAnswer();
    return 0;
}
