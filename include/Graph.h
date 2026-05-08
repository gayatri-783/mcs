#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <utility>

class Graph {
public:
    struct Edge {
        uint32_t to;
        uint32_t w;
    };

    using AdjList = std::vector<std::vector<Edge>>;

    Graph() = default;
    explicit Graph(uint32_t n);


    static Graph from_gr(const std::string& path, bool directed = false);


    void add_edge(uint32_t u, uint32_t v, uint32_t w = 1);

    uint32_t n() const { return adj.size(); }


    std::vector<uint32_t> mcs_ordering() const;


    struct TreeDecomposition {
        std::vector<std::vector<uint32_t>> bags;
        std::vector<std::vector<uint32_t>> tree_adj;
        uint32_t root = 0;
    };

    TreeDecomposition build_tree_decomposition_from_mcs() const;

private:
    AdjList adj;
};

