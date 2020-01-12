#include <iostream>
#include <vector>
#include <algorithm>

int argmax(std::vector<int> array, const size_t length) {
    int argmax = 0;
    for (int i = 0; i < static_cast<int>(length); ++i) {
        if (array[i] >= array[argmax]) {
            argmax = i;
        }
    }
    return argmax;
}

std::vector<int> read_sequence(size_t &number) {

    std::cin >> number;
    std::vector<int> sequence(number);
    int value;
    for (int i = 0; i < static_cast<int>(number); ++i) {
        std::cin >> value;
        sequence[i] = value;
    }
    return sequence;
}

void write_sequence(const std::vector<int> &sequence, const std::vector<int> &subsequence,
                    const int length) {
    for (int i = 0; i < length - 1; ++i) {
        std::cout << sequence[subsequence[i]] << " ";
    }
    std::cout << sequence[subsequence[length - 1]];
}

std::vector<int>
get_subsequence(const std::vector<int> &sequence_a, const std::vector<int> &sequence_b, int index,
                const size_t length) {

    std::vector<int> cur_sequence = sequence_a;
    std::vector<int> subsequence(length);

    for (int i = 0; i < static_cast<int>(length) - 1; ++i) {
        subsequence[static_cast<int>(length) - 1 - i] = index;
        index = cur_sequence[index];
        cur_sequence = (i % 2 == 0) ? sequence_b : sequence_a;
    }
    subsequence[0] = index;

    return subsequence;
}

std::vector<int> find_sequence(const std::vector<int> &sequence, const size_t number,
                               int &length) {

    std::vector<int> odd_seq(number);
    std::vector<int> even_seq(number);
    std::vector<int> odd_prev_number(number);
    std::vector<int> even_prev_number(number);

    odd_seq[0] = 1;
    even_seq[0] = 1;

    for (int index = 1; index < static_cast<int>(number); ++index) {
        odd_seq[index] = 1;
        even_seq[index] = 1;
        odd_prev_number[index] = index;
        even_prev_number[index] = index;
        for (int j = 0; j < index; ++j) {
            if ((sequence[j] > sequence[index]) and (even_seq[j] + 1) >= odd_seq[index]) {
                odd_seq[index] = even_seq[j] + 1;
                odd_prev_number[index] = j;
            }
            if ((sequence[j] < sequence[index]) and (odd_seq[j] + 1) >= even_seq[index]) {
                even_seq[index] = odd_seq[j] + 1;
                even_prev_number[index] = j;
            }
        }
    }

    int odd_max = argmax(odd_seq, number);
    int even_max = argmax(even_seq, number);
    int length_odd, length_even;
    std::vector<int> subseq_odd;
    std::vector<int> subseq_even;
    subseq_odd = get_subsequence(odd_prev_number, even_prev_number, odd_max,
                                 static_cast<size_t>(odd_seq[odd_max]));
    length_odd = odd_seq[odd_max];

    subseq_even = get_subsequence(even_prev_number, odd_prev_number, even_max,
                                  static_cast<size_t>(even_seq[even_max]));
    length_even = even_seq[even_max];

    if (length_odd > length_even) {
        length = length_odd;
        return subseq_odd;
    } else {
        if (length_odd < length_even) {
            length = length_even;
            return subseq_even;
        } else {
            length = length_odd;
            if (subseq_odd[0] < subseq_even[0]) {
                return subseq_odd;
            } else {
                if (subseq_odd[0] > subseq_even[0]) {
                    return subseq_odd;
                } else {
                    if (length > 1) {
                        if (subseq_odd[1] > subseq_even[1]) {
                            return subseq_odd;
                        } else {
                            return subseq_even;
                        }
                    } else {
                        return subseq_odd;
                    }
                }
            }
        }
    }
}


int main() {

    size_t number;
    int length;
    std::vector<int> sequence = read_sequence(number);
    std::reverse(std::begin(sequence), std::end(sequence));

    std::vector<int> subsequence = find_sequence(sequence, number, length);
    std::reverse(std::begin(subsequence), std::end(subsequence));

    write_sequence(sequence, subsequence, length);
    return 0;
}
