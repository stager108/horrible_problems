#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits.h>


class FixedSet {
private:
    struct HashParameters {
        unsigned int a_par = 0, b_par = 0;
        size_t m_par = 1;

        void Initialize(const unsigned int new_a,
                        const unsigned int new_b,
                        size_t new_m) {
            a_par = new_a;
            b_par = new_b;
            m_par = new_m;
        }

        void Initialize(std::uniform_int_distribution<unsigned> &distribution,
                        std::mt19937 &generator, size_t new_m) {
            a_par = distribution(generator);
            b_par = distribution(generator);
            m_par = new_m;
        }

        void Reset(std::uniform_int_distribution<unsigned> &distribution,
                   std::mt19937 &generator) {
            a_par = distribution(generator);
            b_par = distribution(generator);
        }
    };

    std::vector<HashParameters> second_level_parameters_;
    HashParameters first_level_parameters_;
    std::mt19937 generator_;
    std::uniform_int_distribution<unsigned> distribution_;
    std::vector<std::vector<int>> hash_table_;

    size_t CalculateHash(const HashParameters parameters, const int key) const {
        const int p_par = 1000000321; // big prime number
        return ((parameters.a_par * key + parameters.b_par)
                                % p_par % parameters.m_par);
    }


public:
    explicit FixedSet() = default;

    size_t CalculateSquaredSum(const std::vector<std::vector<int>> &table) {
        size_t size = table.size();
        size_t sum = 0;
        for (size_t i = 0; i < size; ++i) {
            size_t subsize = table[i].size();
            sum += subsize * subsize;
        }
        return sum;
    }

    void InitializeFirstLevel(const std::vector<int> &numbers,
                              std::vector<std::vector<int>> &table) {
        size_t size = numbers.size();
        if (size == 0) {
            first_level_parameters_.Initialize(0, 0, 1);
        } else {
            first_level_parameters_.m_par = static_cast<int>(size);
            do {
                table.clear();
                table.resize(numbers.size());
                first_level_parameters_.Reset(distribution_, generator_);
                for (size_t j = 0; j < size; ++j) {
                    size_t cur_hash = CalculateHash(first_level_parameters_, numbers[j]);
                    table[cur_hash].push_back(numbers[j]);
                }
                table.shrink_to_fit();
            } while (CalculateSquaredSum(table) > 2 * size);
        }
    }

    HashParameters ResetSecondLevelHashParameters(const size_t index, const size_t size_index,
            const std::vector<std::vector<int>> &first_level_table) {
        HashParameters new_hash;
        new_hash.Initialize(distribution_, generator_, size_index * size_index);
        size_t count = 0;
        while (count != size_index) {
            for (size_t j_ind = 0; j_ind < size_index; ++j_ind) {
                size_t cur_hash = CalculateHash(new_hash,
                                         first_level_table[index][j_ind]);
                if (hash_table_[index][cur_hash] == INT_MAX) {
                    hash_table_[index][cur_hash] = first_level_table[index][j_ind];
                    ++count;
                } else {
                    hash_table_[index].clear();
                    hash_table_[index].resize(size_index * size_index, INT_MAX);
                    new_hash.Reset(distribution_, generator_);
                    count = 0;
                    break;
                }
            }
        }
        return new_hash;
    }

    void InitializeSecondLevel(const size_t index,
            const std::vector<std::vector<int>> &first_level_table) {
        HashParameters new_hash;
        size_t size_index = first_level_table[index].size();
        if (size_index == 0) {
            new_hash.Initialize(distribution_, generator_, 1);
        } else {
            new_hash.Initialize(distribution_, generator_, size_index * size_index);
            hash_table_[index].resize(size_index * size_index, INT_MAX);
            if (size_index == 1) {
                hash_table_[index][0] = first_level_table[index][0];
            } else {
                new_hash = ResetSecondLevelHashParameters(index, size_index,
                                                          first_level_table);
            }
        }
        second_level_parameters_[index] = new_hash;
    }

    void Initialize(const std::vector<int> &numbers) {
        hash_table_.clear();
        second_level_parameters_.clear();
        size_t size = numbers.size();
        std::vector<std::vector<int>> first_level_table(numbers.size());

        InitializeFirstLevel(numbers, first_level_table);

        hash_table_.resize(numbers.size());
        second_level_parameters_.resize(numbers.size());

        for (size_t index = 0; index < size; ++index) {
            InitializeSecondLevel(index, first_level_table);
        }
    };

    bool Contains(const int number) const {
        if (not hash_table_.empty()) {
            size_t first_hash = CalculateHash(first_level_parameters_, number);
            if (not hash_table_[first_hash].empty()) {
                size_t second_hash = CalculateHash(second_level_parameters_[first_hash], number);
                if (hash_table_[first_hash][second_hash] == number) {
                    return true;
                }
            }
        }
        return false;
    };
};
