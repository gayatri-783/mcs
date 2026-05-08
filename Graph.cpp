#include "../include/Graph.h"
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
#include <limits>
#include <iostream>
#include <unordered_set>

Graph::Graph(uint32_t n) : adj(n) {}

void Graph::add_edge(uint32_t u, uint32_t v, uint32_t w) {
    if (u >= adj.size() || v >= adj.size()) return;

    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
}

Graph Graph::from_gr(const std::string& path, bool /*directed*/) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Cannot open .gr file");

    std::string line;
    uint32_t n = 0, m = 0;

    // read header
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == 'c') continue;

        std::istringstream iss(line);
        char type;
        iss >> type;

        if (type == 'p') {
            std::string sp;
            iss >> sp >> n >> m;
            break;
        }
    }

    Graph g(n);

    while (std::getline(in, line)) {
        if (line.empty() || line[0] == 'c') continue;

        std::istringstream iss(line);
        char type;
        iss >> type;

        if (type == 'a') {
            uint32_t u, v, w;
            iss >> u >> v >> w;


            --u;
            --v;

            g.add_edge(u, v, w);
        }
    }

    return g;
}

std::vector<uint32_t> Graph::mcs_ordering() const {
    const uint32_t n = adj.size();

    std::vector<uint32_t> label(n, 0);
    std::vector<bool> used(n, false);
    std::vector<uint32_t> order;
    order.reserve(n);

    for (uint32_t i = 0; i < n; i++) {
        uint32_t best = std::numeric_limits<uint32_t>::max();
        int best_label = -1;

        for (uint32_t v = 0; v < n; v++) {
            if (!used[v] && (int)label[v] > best_label) {
                best_label = label[v];
                best = v;
            }
        }

        used[best] = true;
        order.push_back(best);

        for (const auto& e : adj[best]) {
            if (!used[e.to]) {
                label[e.to]++;
            }
        }
    }


    std::reverse(order.begin(), order.end());
    return order;
}


Graph::TreeDecomposition Graph::build_tree_decomposition_from_mcs() const {
    auto order = mcs_ordering();
    const uint32_t n = adj.size();


    std::vector<int> pos(n);
    for (uint32_t i = 0; i < n; i++) {
        pos[order[i]] = i;
    }


    std::vector<std::unordered_set<uint32_t>> work_adj(n);
    for (uint32_t u = 0; u < n; u++) {
        for (auto& e : adj[u]) {
            work_adj[u].insert(e.to);
        }
    }

    std::vector<std::vector<uint32_t>> bags(n);
    std::vector<std::vector<uint32_t>> tree_adj(n);
    std::vector<uint32_t> parent(n, n);


    for (uint32_t idx = 0; idx < n; idx++) {
        uint32_t v = order[idx];

        std::vector<uint32_t> active_nbrs;


        for (uint32_t u : work_adj[v]) {
            if (pos[u] > (int)idx) {
                active_nbrs.push_back(u);
            }
        }


        bags[v] = active_nbrs;
        bags[v].push_back(v);


        uint32_t best_parent = n;
        int best_pos = n;

        for (uint32_t u : active_nbrs) {
            if (pos[u] < best_pos) {
                best_pos = pos[u];
                best_parent = u;
            }
        }

        if (best_parent != n) {
            parent[v] = best_parent;
            tree_adj[v].push_back(best_parent);
            tree_adj[best_parent].push_back(v);
        }


        for (size_t i = 0; i < active_nbrs.size(); i++) {
            for (size_t j = i + 1; j < active_nbrs.size(); j++) {
                uint32_t a = active_nbrs[i];
                uint32_t b = active_nbrs[j];

                work_adj[a].insert(b);
                work_adj[b].insert(a);
            }
        }
    }

    uint32_t root = order[0];

    return {bags, tree_adj, root};
}