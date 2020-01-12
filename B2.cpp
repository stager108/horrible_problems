#include <iostream>
#include <vector>
#include <algorithm>

struct FootballPair {
    size_t effectiveness = 0;
    size_t index = 0;

    static bool EffectivenessComparator(const FootballPair &first, const FootballPair &second) {
        return first.effectiveness < second.effectiveness;
    }

    static bool IndexComparator(const FootballPair &first, const FootballPair &second) {
        return first.index < second.index;
    }
};

template<typename T, typename Comparator>
class Heap {
public:
    explicit Heap(Comparator comparator) {
        heap_size_ = 0;
        comparator_ = comparator;
    }

    T Min() const {
        return elements_[0];
    }

    void ExtractMin() {
        --heap_size_;
        if (heap_size_ > 0) {
            std::swap(elements_[0], elements_[heap_size_]);
            SiftDown(0);
        }
        elements_.pop_back();
    }

    void Push(const T value) {
        ++heap_size_;
        elements_.push_back(value);
        SiftUp(heap_size_ - 1);
    }

    ~Heap() = default;

private:
    size_t heap_size_;
    std::vector<T> elements_;
    Comparator comparator_;

    inline size_t Parent(size_t value) const {
        return (value - 1) / 2;
    }

    inline size_t LeftSon(size_t value) const {
        return 2 * value + 1;
    }

    inline size_t RightSon(size_t value) const {
        return 2 * value + 2;
    }

    void SiftDown(size_t value) {
        size_t next_value = value;
        if (RightSon(value) < heap_size_) {
            if (comparator_(elements_[LeftSon(value)], elements_[value]) ||
                (comparator_(elements_[RightSon(value)], elements_[value]))) {

                if (not comparator_(elements_[value], elements_[LeftSon(value)]) &&
                    comparator_(elements_[LeftSon(value)], elements_[RightSon(value)])) {
                    std::swap(elements_[value], elements_[LeftSon(value)]);
                    value = LeftSon(value);
                } else if
                        (comparator_(elements_[RightSon(value)], elements_[value]) &&
                         (not comparator_(elements_[LeftSon(value)], elements_[RightSon(value)]))) {
                    std::swap(elements_[value], elements_[RightSon(value)]);
                    value = RightSon(value);
                }
            }
        } else {
            if (LeftSon(value) < heap_size_) {
                if (comparator_(elements_[LeftSon(value)], elements_[value])) {
                    std::swap(elements_[value], elements_[LeftSon(value)]);
                    value = LeftSon(value);
                }
            }
        }
        if (next_value < value) {
            SiftDown(value);
        }
    }

    void SiftUp(size_t value) {
        while ((value > 0) and (comparator_(elements_[value], elements_[Parent(value)]))) {
            std::swap(elements_[value], elements_[Parent(value)]);
            value = Parent(value);
        }
    }
};

template<typename T, typename Comparator>
void HeapSort(T begin,
              T end,
              Comparator comparator) {

    Heap<typename T::value_type, Comparator> my_heap(comparator);

    for (auto point = begin; point != end; ++point) {
        my_heap.Push(*point);
    }

    for (auto point = begin; point != end; ++point) {
        point->index = my_heap.Min().index;
        point->effectiveness = my_heap.Min().effectiveness;
        my_heap.ExtractMin();
    }
}


std::vector<FootballPair> GetMaximalTeam(std::vector<FootballPair> values) {

    size_t n_value = values.size();
    if (n_value == 1) {
        return values;
    }

    HeapSort(values.begin(), values.end(), FootballPair::EffectivenessComparator);

    size_t right = 0, best_left = 0, best_right = 0;
    int64_t best_sum = 0, sum;

    while ((best_right < n_value) and (static_cast<int64_t>(values[best_right].effectiveness) <=
                                       static_cast<int64_t>(values[best_left].effectiveness) +
                                       static_cast<int64_t>(values[best_left + 1].effectiveness))) {
        best_sum += values[best_right].effectiveness;
        ++best_right;
    }

    sum = best_sum;
    right = best_right;

    for (size_t left_ind = 1; left_ind < n_value - 1; ++left_ind) {
        sum = sum - static_cast<int64_t>(values[left_ind - 1].effectiveness);
        while ((right < n_value) and (static_cast<int64_t>(values[right].effectiveness) <=
                                      static_cast<int64_t>(values[left_ind].effectiveness) +
                                      static_cast<int64_t>(values[left_ind + 1].effectiveness))) {
            sum += static_cast<int64_t>(values[right].effectiveness);
            ++right;
        }
        if (sum > best_sum) {
            best_sum = sum;
            best_left = left_ind;
            best_right = right;
        }
    }

    HeapSort(values.begin() + best_left, values.begin() + best_right,
             FootballPair::IndexComparator);

    std::vector<FootballPair> answer;
    answer.reserve(static_cast<unsigned int>(best_right - best_left + 1));
    for (size_t i = best_left; i < best_right; ++i) {
        answer.push_back(values[i]);
    }
    return answer;
}

int64_t CountSum(std::vector<FootballPair> &team) {
    int64_t sum = 0;
    size_t size = team.size();
    for (size_t i = 0; i < size; ++i) {
        sum += team[i].effectiveness;
    }
    return sum;
}

std::vector<FootballPair> ReadData() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t players_number;

    std::cin >> players_number;
    std::vector<FootballPair> players;
    players.reserve(players_number);
    for (size_t i = 0; i < players_number; ++i) {
        FootballPair new_pair;
        std::cin >> new_pair.effectiveness;
        new_pair.index = i + 1;
        players.push_back(new_pair);
    }
    return players;
}

void PrintTeam(std::vector<FootballPair> team) {
    size_t size = team.size();
    for (size_t i = 0; i < size - 1; ++i) {
        std::cout << team[i].index << ' ';
    }
    std::cout << team[size - 1].index;
}

int main() {
    auto players = ReadData();
    auto team = GetMaximalTeam(players);
    std::cout << CountSum(team) << '\n';
    PrintTeam(team);
    return 0;
}
