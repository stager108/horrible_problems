#include <iostream>
#include <memory>
#include <vector>

struct Graph {

    void Resize(const size_t size) {
        childrens.resize(size);
        component.resize(size);
    }

    void AddEdge(const unsigned int first, const unsigned int second) {
        childrens[first].push_back(second);
    }

    std::vector<std::vector<unsigned int>> childrens;
    std::vector<unsigned int> component;
};

size_t ReadGraph(Graph &graph, Graph &transposed_graph) {
    size_t edges, size;
    std::cin >> size >> edges;
    graph.Resize(size);
    transposed_graph.Resize(size);
    for (size_t ind = 0; ind < edges; ++ind) {
        unsigned int first, second, result;
        std::cin >> first >> second >> result;
        --first;
        --second;
        if (result != 3) {
            if (result == 1) {
                graph.AddEdge(first, second);
                transposed_graph.AddEdge(second, first);
            } else {
                graph.AddEdge(second, first);
                transposed_graph.AddEdge(first, second);
            }
        }
    }
    graph.childrens.shrink_to_fit();
    transposed_graph.childrens.shrink_to_fit();
    return size;
}

class Solver {
private:
    void DfsFirst(const unsigned int v_index) {
        used_[v_index] = true;
        for (auto child = graph_.childrens[v_index].begin();
             child != graph_.childrens[v_index].end(); ++child) {
            if (not used_[*child]) {
                DfsFirst(*child);
            }
        }
        order_.push_back(v_index);
    }

    void DfsSecond(const unsigned int v_index, const unsigned int component) {
        used_[v_index] = true;
        graph_.component[v_index] = component;
        for (auto child = transposed_graph_.childrens[v_index].begin();
             child != transposed_graph_.childrens[v_index].end(); ++child) {
            if (not used_[*child]) {
                DfsSecond(*child, component);
            }
        }
    }

    unsigned int GetCSS() {

        used_.resize(size_, false);
        order_.reserve(size_);
        for (unsigned int i = 0; i < size_; ++i) {
            if (not used_[i]) {
                DfsFirst(i);
            }
        }
        std::fill(used_.begin(), used_.end(), false);
        unsigned int comp = 0;
        for (size_t i = size_; i > 0; --i) {
            if (not used_[order_[i - 1]]) {
                DfsSecond(order_[i - 1], comp);
                ++comp;
            }
        }
        used_.clear();
        order_.clear();
        return comp;
    }

    size_t FindBiggestSourceComponents(const unsigned int comp_count) {

        std::vector<bool> is_source;
        std::vector<int> comp_size;
        is_source.resize(static_cast<size_t>(comp_count), true);
        comp_size.resize(static_cast<size_t>(comp_count), 0);

        for (size_t i = 0; i < size_; ++i) {
            for (auto child = graph_.childrens[i].begin();
                 child != graph_.childrens[i].end(); ++child) {
                if (graph_.component[*child] != graph_.component[i]) {
                    is_source[graph_.component[*child]] = false;
                }
            }
            ++comp_size[graph_.component[i]];
        }

        size_t answer = 1;
        for (size_t i = 0; i < comp_count; ++i) {
            if (is_source[i]) {
                if (size_ + 1 - comp_size[i] > answer) {
                    answer = size_ + 1 - comp_size[i];
                }
            }
        }
        return answer;
    }

    Graph graph_, transposed_graph_;
    std::vector<bool> used_;
    std::vector<unsigned int> order_;
    size_t size_ = 0;

public:
    explicit Solver() = default;

    ~Solver() = default;

    void Solve() {
        size_ = ReadGraph(graph_, transposed_graph_);
        unsigned int comp_count = GetCSS();
        size_t answer = FindBiggestSourceComponents(comp_count);
        std::cout << answer;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Solver solver;
    solver.Solve();
    return 0;
}
