#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

class Graph {

    struct Node {
        int component = 0;
        std::vector<int> children;
        std::vector<int> weigths;

        void AddEdge(int child, int weigth) {
            children.push_back(child);
            weigths.push_back(weigth);
        }
    };

    std::vector<Node> graph;
    std::vector<bool> used;
    std::vector<int> time_in, fup;
    int size = 0;
    int timer = 0;
    int ans = 0;

    void Dfs(int v_index, int p_value = -1) {
        used[v_index] = true;
        ++timer;
        time_in[v_index] = timer;
        fup[v_index] = timer;
        int cur_size = static_cast<int>(graph[v_index].children.size());
        for (int ind = 0; ind < cur_size; ++ind) {
            int next = graph[v_index].children[ind];
            if (next == p_value) {
                continue;
            }
            if (used[next]) {
                fup[v_index] = std::min(fup[v_index], fup[next]);
            } else {
                Dfs(next, v_index);
                fup[v_index] = std::min(fup[v_index], fup[next]);
                if (fup[next] > time_in[v_index]) {
                    if (ans > graph[v_index].weigths[ind]) {
                        ans = graph[v_index].weigths[ind];
                    }
                }
            }
        }
    }

    void ReadGraph() {
        int edges;
        std::cin >> size >> edges;
        int first, second, result;
        graph.resize(static_cast<size_t>(size));
        for (int i = 0; i < edges; ++i) {
            std::cin >> first >> second >> result;
            --first;
            --second;
            graph[first].AddEdge(second, result);
            graph[second].AddEdge(first, result);
        }
    }

public:
    explicit Graph() = default;

    ~Graph() = default;

    void Solve() {
        ReadGraph();
        ans = 1000000007;
        used.resize(static_cast<size_t>(size), false);
        time_in.resize(static_cast<size_t>(size));
        fup.resize(static_cast<size_t>(size));
        timer = 0;
        for (int i = 0; i < size; ++i) {
            if (not used[i]) {
                Dfs(i);
            }
        }
        if (ans == 1000000007) {
            ans = -1;
        }
        std::cout << ans;
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Graph solver;
    solver.Solve();
    return 0;
}
