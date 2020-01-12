#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

struct part {
    int64_t left = -1;
    int64_t lenght = -1;
    bool free = true;
    part *next = nullptr;
    part *previous = nullptr;
    int heap_index = -1;

    static bool comparator(const part *first, const part *second) {
        if (first->lenght == second->lenght) {
            return first->left < second->left;
        } else {
            return first->lenght > second->lenght;
        }
    }
};

void reconnect(part *node) {
    part *prev = node->previous;
    part *next = node->next;

    if (prev != nullptr) {
        prev->next = next;
    }
    if (next != nullptr) {
        next->previous = prev;
    }
    node->previous = nullptr;
    node->next = nullptr;
    delete node;
}

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

    bool IsNotEmpty() {
        return (heap_size_ > 0);
    }

    void ExtractMin(int ind) {
        --heap_size_;
        if (heap_size_ > 0) {
            std::swap(elements_[ind], elements_[heap_size_]);
            elements_[ind]->heap_index = ind;
            SiftDown(ind);
            SiftUp(ind);
        }
        elements_[heap_size_]->heap_index = -1;
        elements_.pop_back();
    }

    void Push(T value) {
        ++heap_size_;
        value->heap_index = heap_size_ - 1;
        elements_.push_back(value);
        SiftUp(heap_size_ - 1);
    }

    ~Heap() {
        for (int i = 0; i < heap_size_; ++i) {
            delete elements_[i];
        }
        elements_.clear();
    };

private:
    int heap_size_;
    std::vector<T> elements_;
    Comparator comparator_;

    int Parent(int value) const {
        return (value - 1) / 2;
    }

    int LeftSon(int value) const {
        return 2 * value + 1;
    }

    int RightSon(int value) const {
        return 2 * value + 2;
    }

    void SiftDown(int value) {
        int next_value = value;
        if (RightSon(value) < heap_size_) {
            if (comparator_(elements_[LeftSon(value)], elements_[value]) ||
                (comparator_(elements_[RightSon(value)], elements_[value]))) {

                if (not comparator_(elements_[value], elements_[LeftSon(value)]) &&
                    comparator_(elements_[LeftSon(value)], elements_[RightSon(value)])) {
                    std::swap(elements_[value], elements_[LeftSon(value)]);
                    elements_[value]->heap_index = value;
                    elements_[LeftSon(value)]->heap_index = LeftSon(value);
                    value = LeftSon(value);
                } else if
                        (comparator_(elements_[RightSon(value)], elements_[value]) &&
                         (not comparator_(elements_[LeftSon(value)], elements_[RightSon(value)]))) {
                    std::swap(elements_[value], elements_[RightSon(value)]);
                    elements_[value]->heap_index = value;
                    elements_[RightSon(value)]->heap_index = RightSon(value);
                    value = RightSon(value);
                }
            }
        } else {
            if (LeftSon(value) < heap_size_) {
                if (comparator_(elements_[LeftSon(value)], elements_[value])) {
                    std::swap(elements_[value], elements_[LeftSon(value)]);
                    elements_[value]->heap_index = value;
                    elements_[LeftSon(value)]->heap_index = LeftSon(value);
                    value = LeftSon(value);
                }
            }
        }
        if (next_value < value) {
            SiftDown(value);
        }
    }

    void SiftUp(int value) {
        while ((value > 0) and (comparator_(elements_[value], elements_[Parent(value)]))) {
            std::swap(elements_[value], elements_[Parent(value)]);
            elements_[value]->heap_index = value;
            elements_[Parent(value)]->heap_index = Parent(value);
            value = Parent(value);
        }
    }
};

template<typename Comparator>
void solve(Comparator comparator) {
    Heap<part *, Comparator> my_heap(comparator);
    int64_t n_value;
    int m_value;
    std::cin >> n_value >> m_value;

    auto first = new part;
    first->left = 0;
    first->lenght = n_value;

    my_heap.Push(first);

    std::vector<part *> parts(static_cast<size_t>(m_value), nullptr);
    std::vector<int64_t> answers(static_cast<size_t>(m_value), -10);
    int64_t request;
    for (int ind = 0; ind < m_value; ++ind) {
        std::cin >> request;
        if (request > 0) {
            answers[ind] = -1;
            if (my_heap.IsNotEmpty()) {
                part *min = my_heap.Min();
                if (min->lenght >= request) {
                    answers[ind] = min->left + 1;
                    my_heap.ExtractMin(0);
                    if (min->lenght == request) {
                        min->free = false;
                        parts[ind] = min;
                    } else {
                        part *newp = new part;
                        newp->left = min->left;
                        newp->lenght = request;
                        newp->free = false;
                        newp->previous = min->previous;
                        newp->next = min;
                        if (min->previous != nullptr) {
                            min->previous->next = newp;
                        }
                        min->previous = newp;
                        min->left = min->left + request;
                        min->lenght = min->lenght - request;
                        my_heap.Push(min);
                        parts[ind] = newp;
                    }
                }
            }
        } else {
            request = -request - 1;
            part *cur = parts[request];
            parts[request] = nullptr;
            if (cur != nullptr) {
                if ((cur->previous == nullptr) and (cur->next == nullptr)) {
                    cur->free = true;
                    my_heap.Push(cur);
                } else {
                    if (cur->previous == nullptr) {
                        if (cur->next->free) {
                            part *righter = cur->next;
                            my_heap.ExtractMin(righter->heap_index);
                            righter->left = cur->left;
                            righter->lenght = righter->lenght + cur->lenght;
                            reconnect(cur);
                            my_heap.Push(righter);
                        } else {
                            cur->free = true;
                            my_heap.Push(cur);
                        }
                    } else {
                        if (cur->next == nullptr) {
                            if (cur->previous->free) {
                                part *lefter = cur->previous;
                                my_heap.ExtractMin(lefter->heap_index);
                                lefter->lenght += cur->lenght;
                                reconnect(cur);
                                my_heap.Push(lefter);
                            } else {
                                cur->free = true;
                                my_heap.Push(cur);
                            }
                        } else {
                            if ((cur->previous->free) and (cur->next->free)) {
                                my_heap.ExtractMin(cur->previous->heap_index);
                                my_heap.ExtractMin(cur->next->heap_index);
                                part *lefter = cur->previous;
                                lefter->lenght += cur->lenght + cur->next->lenght;
                                reconnect(cur->next);
                                reconnect(cur);
                                my_heap.Push(lefter);
                            } else {
                                if (cur->previous->free) {
                                    my_heap.ExtractMin(cur->previous->heap_index);
                                    part *lefter = cur->previous;
                                    lefter->lenght += cur->lenght;
                                    reconnect(cur);
                                    my_heap.Push(lefter);
                                } else {
                                    if (cur->next->free) {
                                        my_heap.ExtractMin(cur->next->heap_index);
                                        part *righter = cur->next;
                                        righter->lenght += cur->lenght;
                                        righter->left = cur->left;
                                        reconnect(cur);
                                        my_heap.Push(righter);
                                    } else {
                                        cur->free = true;
                                        my_heap.Push(cur);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < m_value - 1; ++i) {
        if (answers[i] != -10) {
            std::cout << answers[i] << '\n';
        }
    }
    if (m_value > 0) {
        if (answers[m_value - 1] != -10) {
            std::cout << answers[m_value - 1];
        }
    }
    for (int i = 0; i < m_value; ++i) {
        delete parts[i];
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    solve(part::comparator);
    return 0;
}
