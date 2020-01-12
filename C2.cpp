#include <iostream>
#include <vector>
#include <algorithm>

class Heap {
public:
    explicit Heap(int max_size) {
        heap_size = 0;
        elements.resize(static_cast<size_t>(max_size));
        numbers.resize(static_cast<size_t>(max_size));
    }

    int min(int &value) const {
        value = numbers[0];
        return elements[0];
    }

    void extractMin() {
        --heap_size;
        if (heap_size > 0) {
            std::swap(elements[0], elements[heap_size]);
            std::swap(numbers[0], numbers[heap_size]);
            siftDown(0);
        }
    }

    void push(int value, int seq_number) {
        ++heap_size;
        elements[heap_size - 1] = value;
        numbers[heap_size - 1] = seq_number;
        siftUp(heap_size - 1);
    }

    ~Heap() = default;

private:
    int heap_size;
    std::vector<int> elements;
    std::vector<int> numbers;

    int parent(int value) const {
        return (value - 1) / 2;
    }

    int leftSon(int value) const {
        return 2 * value + 1;
    }

    int rightSon(int value) const {
        return 2 * value + 2;
    }

    void siftDown(int value) {
        int next_value = value;
        if (rightSon(value) < heap_size) {
            if ((elements[value] > elements[leftSon(value)]) ||
                (elements[value] > elements[rightSon(value)])) {

                if ((elements[value] > elements[leftSon(value)]) &&
                    (elements[rightSon(value)] >= elements[leftSon(value)])) {
                    std::swap(elements[value], elements[leftSon(value)]);
                    std::swap(numbers[value], numbers[leftSon(value)]);
                    value = leftSon(value);
                } else if ((elements[value] > elements[rightSon(value)]) &&
                           (elements[rightSon(value)] <= elements[leftSon(value)])) {
                    std::swap(elements[value], elements[rightSon(value)]);
                    std::swap(numbers[value], numbers[rightSon(value)]);
                    value = rightSon(value);
                }
            }
        } else {
            if (leftSon(value) < heap_size)
                if (elements[value] > elements[leftSon(value)]) {
                    std::swap(elements[value], elements[leftSon(value)]);
                    std::swap(numbers[value], numbers[leftSon(value)]);
                    value = leftSon(value);
                }
        }
        if (next_value < value) {
            siftDown(value);
        }
    }

    void siftUp(int value) {
        while ((value > 0) && (elements[value] < elements[parent(value)])) {
            std::swap(elements[value], elements[parent(value)]);
            std::swap(numbers[value], numbers[parent(value)]);
            value = parent(value);
        }
    }
};

void merge_sequences() {

    int nn, mm;
    std::cin >> nn >> mm;
    std::vector<std::vector<int>> sequences;

    for (int i = 0; i < nn; ++i) {
        std::vector<int> seq(static_cast<size_t >(mm));
        for (int j = 0; j < mm; ++j) {
            std::cin >> seq[j];
        }
        sequences.emplace_back(seq);
    }

    std::vector<int> begins(static_cast<size_t>(nn), 1);
    Heap my_heap(nn);
    for (int i = 0; i < nn; ++i) {
        my_heap.push(sequences[i][0], i);
    }
    int final_size = nn * mm;
    std::vector<int> answer(static_cast<size_t>(final_size));
    int next_element;
    int next_sequence;
    for (int ind = 0; ind < final_size; ++ind) {
        next_element = my_heap.min(next_sequence);
        answer[ind] = next_element;
        my_heap.extractMin();
        if (begins[next_sequence] < mm) {
            my_heap.push(sequences[next_sequence][begins[next_sequence]],
                         next_sequence);
            ++begins[next_sequence];
        }
    }

    for (int i = 0; i < final_size - 1; ++i) {
        std::cout << answer[i] << ' ';
    }
    std::cout << answer[final_size - 1];
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    merge_sequences();
    return 0;
}
