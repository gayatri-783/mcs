#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <limits>

#include "../include/Graph.h"

uint32_t compute_treewidth(const Graph::TreeDecomposition& td) {
    uint32_t width = 0;

    for (const auto& bag : td.bags) {
        width = std::max<uint32_t>(width, bag.size());
    }

    if (width == 0) return 0;
    return width - 1;
}

uint32_t compute_treeheight(const Graph::TreeDecomposition& td, uint32_t root) {
    const auto& adj = td.tree_adj;

    std::vector<int> parent(adj.size(), -1);
    std::vector<uint32_t> depth(adj.size(), 0);

    std::stack<uint32_t> st;
    st.push(root);
    parent[root] = root;
    depth[root] = 0;

    uint32_t max_depth = 0;

    while (!st.empty()) {
        uint32_t v = st.top();
        st.pop();

        for (uint32_t u : adj[v]) {
            if (u == parent[v]) continue;

            parent[u] = v;
            depth[u] = depth[v] + 1;
            max_depth = std::max(max_depth, depth[u]);

            st.push(u);
        }
    }

    return max_depth;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <graph.gr>\n";
        return 1;
    }

    std::string path = argv[1];

    try {
        std::cout << "Loading graph: " << path << "\n";
        Graph g = Graph::from_gr(path);

        std::cout << "Building tree decomposition (MCS)...\n";
        auto td = g.build_tree_decomposition_from_mcs();

        uint32_t treewidth = compute_treewidth(td);
        uint32_t treeheight = compute_treeheight(td, td.root);

        std::cout << "===========================\n";
        std::cout << "Treewidth : " << treewidth << "\n";
        std::cout << "Treeheight: " << treeheight << "\n";
        std::cout << "Root      : " << td.root << "\n";
        std::cout << "===========================\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
