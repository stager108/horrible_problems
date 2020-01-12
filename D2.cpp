#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

class GenerateSeq {
public:
    explicit GenerateSeq(int aa, int bb) {
        a_value = aa;
        b_value = bb;
    }

    std::vector<unsigned int> return_sequence(int size) {
        std::vector<unsigned int> answer(static_cast<size_t>(size));
        for (int i = 0; i < size; ++i) {
            answer[i] = nextRand32();
        }
        return answer;
    }

    ~GenerateSeq() = default;

private:
    unsigned int cur = 0;
    int a_value;
    int b_value;

    unsigned int nextRand24() {
        cur = cur * a_value + b_value;
        return cur >> 8;
    }

    unsigned int nextRand32() {
        unsigned int aa = nextRand24(), bb = nextRand24();
        return (aa << 8) ^ bb;
    }
};

class Answer {
private:
    std::mt19937 generator;
    std::uniform_int_distribution<unsigned int> distribution;

public:
    explicit Answer() {}

    unsigned int get_median(std::vector<unsigned int> &list, int left, int right, int kk) {
        int size = right - left + 1;
        if (size <= 5) {
            for (int j = 0; j < size; ++j) {
                for (int i = j; i < size - 1; ++i) {
                    if (list[left + i] > list[left + i + 1]) {
                        std::swap(list[left + i], list[left + i + 1]);
                    }
                }
            }
            return list[left + kk];
        } else {
            int ind = distribution(generator) % (right + 1 - left) + left;
            unsigned int pivot = list[ind];
            std::vector<unsigned int> left_part;
            std::vector<unsigned int> right_part;
            for (int i = left; i <= right; ++i) {
                if (list[i] < pivot) {
                    left_part.push_back(list[i]);
                } else {
                    if (list[i] > pivot) {
                        right_part.push_back(list[i]);
                    }
                }
            }
            int left_size = static_cast<int>(left_part.size());
            int right_size = static_cast<int>(right_part.size());
            if ((left_size <= kk) and (size - right_size >= kk)) {
                return pivot;
            } else {
                if (kk < left_size) {
                    right_part.clear();
                    return get_median(left_part, 0, left_size - 1, kk);
                } else {
                    left_part.clear();
                    return get_median(right_part, 0, right_size - 1,
                                      kk - size + right_size);
                }
            }
        }
    }
};


int64_t get_answer(std::vector<unsigned int> &sequence, int size) {
    Answer GetAnswer;

    int64_t cur_sum = 0;
    auto median = static_cast<uint64_t>(
            GetAnswer.get_median(sequence, 0, size - 1, size / 2));

    for (int i = 0; i < size; ++i) {
        if (median < sequence[i]) {
            cur_sum += static_cast<uint64_t>(sequence[i]) - median;
        } else {
            cur_sum += median - static_cast<uint64_t>(sequence[i]);
        }
    }
    return cur_sum;
}


int main() {
    int n_value, a_value, b_value;
    std::cin >> n_value >> a_value >> b_value;
    GenerateSeq generator(a_value, b_value);
    std::vector<unsigned int> seq = generator.return_sequence(n_value);
    std::cout << get_answer(seq, n_value);
    return 0;
}
