#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>

class Graph {

    struct Node {
        int component = -1;
        std::vector<int> children;
        std::vector<int> weights;

        void AddEdge(int child, int weight) {
            children.push_back(child);
            weights.push_back(weight);
        }
    };

    std::vector<Node> graph_;
    std::vector<Node> transposed_graph;
    std::vector<bool> used;
    std::vector<int> order;

    int timer = 0;
    std::vector<int> time_in, fup;
    int size_ = 0;

    void ReadGraph() {
        int edges;
        std::cin >> size_ >> edges;
        int first, second, result;
        graph_.resize(static_cast<size_t>(size_));
        transposed_graph.resize(static_cast<size_t>(size_));
        for (int ind = 0; ind < edges; ++ind) {
            std::cin >> first >> second >> result;
            --first;
            --second;
            graph_[first].AddEdge(second, result);
            transposed_graph[second].AddEdge(first, result);
        }
    }

    void DfsFirst(int v_index) {
        used[v_index] = true;
        for (size_t i = 0; i < graph_[v_index].children.size(); ++i) {
            if (not used[graph_[v_index].children[i]]) {
                DfsFirst(graph_[v_index].children[i]);
            }
        }
        order.push_back(v_index);
    }

    void DfsSecond(int v_index, int component) {
        used[v_index] = true;
        graph_[v_index].component = component;
        for (size_t i = 0; i < transposed_graph[v_index].children.size(); ++i) {
            if (not used[transposed_graph[v_index].children[i]]) {
                DfsSecond(transposed_graph[v_index].children[i], component);
            }
        }
    }

    void GetCSS() {
        timer = 0;
        order.clear();
        used.resize(static_cast<size_t>(size_), false);
        time_in.resize(static_cast<size_t>(size_));
        fup.resize(static_cast<size_t>(size_));
        for (int i = 0; i < size_; ++i) {
            if (not used[i]) {
                DfsFirst(i);
            }
        }
        std::fill(used.begin(), used.end(), false);
        int comp = 0;
        for (int i = size_ - 1; i >= 0; --i) {
            if (not used[order[i]]) {
                DfsSecond(order[i], comp);
                ++comp;
            }
        }
        used.clear();
        order.clear();
        transposed_graph.clear();
        time_in.clear();
        fup.clear();
    }

    void clear_graph() {
        std::vector<Node> new_graph(static_cast<unsigned int>(size_));
        for (int i = 0; i < size_; ++i) {
            new_graph[i].component = graph_[i].component;
            int mm = static_cast<int>(graph_[i].children.size());
            for (int j = 0; j < mm; ++j) {
                if (graph_[i].component == graph_[graph_[i].children[j]].component) {
                    new_graph[i].AddEdge(graph_[i].children[j], graph_[i].weights[j]);
                }
            }
        }
        graph_.clear();
        graph_ = new_graph;
    }

public:
    explicit Graph() = default;

    ~Graph() = default;

    void Solve() {
        ReadGraph();
        int xx;
        GetCSS();
        clear_graph();

        std::vector<int> dynamic(static_cast<unsigned int>(size_));
        for (int kk = 0; kk < size_; ++kk) {
            for (int ii = 0; ii < size_; ++ii) {
                int mm = static_cast<int>(graph_[ii].children.size());
                for (int j = 0; j < mm; ++j) {
                    int child = graph_[ii].children[j];
                    if (dynamic[child] > dynamic[ii] + graph_[ii].weights[j]) {
                        dynamic[child] = std::max(-2000000000, dynamic[ii] + graph_[ii].weights[j]);
                    }
                }
            }
        }

        std::vector<bool> otr_comp(static_cast<unsigned int>(size_), false);
        xx = -1;
        for (int ii = 0; ii < size_; ++ii) {
            int mm = static_cast<int>(graph_[ii].children.size());
            for (int j = 0; j < mm; ++j) {
                int child = graph_[ii].children[j];
                if (dynamic[child] > dynamic[ii] + graph_[ii].weights[j]) {
                    dynamic[child] = std::max(-2000000000, dynamic[ii] + graph_[ii].weights[j]);
                    otr_comp[graph_[child].component] = true;
                    ++xx;
                }
            }
        }

        if (xx < 0) {
            std::cout << "NO";
        } else {
            std::cout << "YES\n";
            int ind = 0;
            while (otr_comp[graph_[ind].component] != true) {
                ++ind;
            }
            std::cout << ind + 1;
        }
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Graph solver;
    solver.Solve();
    return 0;
}
