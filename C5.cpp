#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <set>

struct Pair {
    int first = 0;
    int second = 0;

    bool operator<(const Pair &second) const {
        if (this->first == second.first) {
            return this->second < second.second;
        } else {
            return this->first < second.first;
        }
    }
};

class Graph {

    struct Node {
        int component = -1;
        std::vector<int> children;

        void AddEdge(int child) {
            children.push_back(child);
        }
    };

    std::vector<Node> graph, cgraph;
    std::vector<bool> used;
    std::vector<int> time_in, fup;
    std::vector<int> lca_h, lca_dfs_list, lca_first, lca_tree;
    int size = 0;
    int timer = 0;
    int ans = 0;
    int maxcolor = -1;
    int capital = -1;
    const int MAXVALUE = 1000000007;

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
            }
        }
    }

    void Paint(int v_index, int color) {
        graph[v_index].component = color;
        int size = static_cast<unsigned int>(graph[v_index].children.size());
        for (int i = 0; i < size; ++i) {
            int child = graph[v_index].children[i];
            if (graph[child].component == -1) {
                if (fup[child] > time_in[v_index]) {
                    ++maxcolor;
                    Paint(child, maxcolor);
                } else {
                    Paint(child, color);
                }
            }
        }
    }

    void ReadGraph() {
        int edges;
        std::cin >> size >> edges >> capital;
        int first, second;
        graph.resize(static_cast<size_t>(size));
        for (int i = 0; i < edges; ++i) {
            std::cin >> first >> second;
            --first;
            --second;
            graph[first].AddEdge(second);
            graph[second].AddEdge(first);
        }
    }

    void CondenseGraph() {
        std::set<Pair> edges_set;
        edges_set.clear();
        cgraph.resize(static_cast<unsigned int>(maxcolor + 1));
        for (int ind = 0; ind < size; ++ind) {
            auto children_size = graph[ind].children.size();
            for (size_t j = 0; j < children_size; ++j) {
                auto child = graph[ind].children[j];
                if (graph[child].component != graph[ind].component) {
                    Pair edge;
                    edge.first = graph[child].component;
                    edge.second = graph[ind].component;
                    if (edges_set.find(edge) == edges_set.end()) {
                        edges_set.insert(edge);
                    }
                }
            }
        }
        for (auto edg_it = edges_set.begin(); edg_it != edges_set.end(); ++edg_it) {
            cgraph[edg_it->first].AddEdge(edg_it->second);
        }
    }

    void lca_dfs(int v_index, int height = 1) {
        used[v_index] = true;
        lca_h[v_index] = height;
        lca_dfs_list.push_back(v_index);
        int size = static_cast<int>(cgraph[v_index].children.size());
        for (int i = 0; i < size; ++i) {
            int child = cgraph[v_index].children[i];
            if (not used[child]) {
                lca_dfs(child, height + 1);
                lca_dfs_list.push_back(v_index);
            }
        }
    }

    void lca_build_tree(int i_value, int l_value, int r_value) {
        if (l_value == r_value) {
            lca_tree[i_value] = lca_dfs_list[l_value];
        } else {
            int m_value = (l_value + r_value) / 2;
            lca_build_tree(i_value + i_value, l_value, m_value);
            lca_build_tree(i_value + i_value + 1, m_value + 1, r_value);
            if (lca_h[lca_tree[i_value + i_value]] < lca_h[lca_tree[i_value + i_value + 1]]) {
                lca_tree[i_value] = lca_tree[i_value + i_value];
            } else {
                lca_tree[i_value] = lca_tree[i_value + i_value + 1];
            }
        }
    }

    void lca_prepare(int root) {
        int nn = static_cast<int>(cgraph.size());
        lca_h.resize(cgraph.size());
        lca_dfs_list.reserve(2 * cgraph.size());
        lca_dfs(root, 0);
        int mm = static_cast<int>(lca_dfs_list.size());
        lca_tree.assign(lca_dfs_list.size() * 4 + 1, -1);
        lca_build_tree(1, 0, mm - 1);
        lca_first.assign(nn, -1);
        for (int i = 0; i < mm; ++i) {
            int vv = lca_dfs_list[i];
            if (lca_first[vv] == -1) {
                lca_first[vv] = i;
            }
        }
    }

    int lca_tree_min(int ii, int sl, int sr, int ll, int rr) {
        if ((sl == ll) and (sr == rr)) {
            return lca_tree[ii];
        }
        int sm = (sl + sr) / 2;
        if (rr <= sm) {
            return lca_tree_min(ii + ii, sl, sm, ll, rr);
        }
        if (ll > sm) {
            return lca_tree_min(ii + ii + 1, sm + 1, sr, ll, rr);
        }
        int ans_f = lca_tree_min(ii + ii, sl, sm, ll, sm);
        int ans_s = lca_tree_min(ii + ii + 1, sm + 1, sr, sm + 1, rr);
        return lca_h[ans_f] < lca_h[ans_s] ? ans_f : ans_s;
    }

    int lca(int aa, int bb) {
        int left = lca_first[aa];
        int right = lca_first[bb];
        if (left > right) {
            std::swap(left, right);
        }
        return lca_tree_min(1, 0, static_cast<int>(lca_dfs_list.size() - 1), left, right);
    }

public:
    explicit Graph() = default;

    ~Graph() = default;

    void Solve() {
        ReadGraph();
        ans = MAXVALUE;
        used.resize(static_cast<size_t>(size), false);
        time_in.resize(static_cast<size_t>(size));
        fup.resize(static_cast<size_t>(size));
        timer = 0;
        for (int i = 0; i < size; ++i) {
            if (not used[i]) {
                Dfs(i);
            }
        }

        maxcolor = -1;
        for (int i = 0; i < size; ++i) {
            if (graph[i].component == -1) {
                ++maxcolor;
                Paint(i, maxcolor);
            }
        }
        used.assign(static_cast<size_t>(size), false);

        CondenseGraph();

        lca_prepare(graph[capital - 1].component);

        int req = 0, fr, sc;
        std::cin >> req;
        for (int i = 0; i < req; ++i) {
            std::cin >> fr >> sc;
            int ans = lca(graph[fr - 1].component,
                          graph[sc - 1].component);
            std::cout << lca_h[ans];
            if (i != req - 1) {
                std::cout << '\n';
            }
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
