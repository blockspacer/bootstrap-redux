// ----------------------------------------------------------------------------
// ____                               _
// |  _\                             | |
// | |_)| __ _ ___  ___  ___ ___   __| | ___ TM
// |  _< / _` / __|/ _ \/ __/ _ \ / _` |/ _ \
// | |_)| (_| \__ \  __/ (_| (_) | (_| |  __/
// |____/\__,_|___/\___|\___\___/ \__,_|\___|
//
//       C O M P I L E R  P R O J E C T
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// This software source file is licensed under the terms of MIT license.
// For details, please read the LICENSE file.
//
// ----------------------------------------------------------------------------

#pragma once

#include <set>
#include <map>
#include <vector>
#include <cstdint>
#include <functional>
#include <unordered_map>

namespace basecode::compiler::graph {

    template <typename V>
    struct directed_edge_t {
        V* dest = nullptr;
        V* source = nullptr;
    };

    template <typename V>
    class directed_graph_t {
    public:
        using vertex_t = V*;
        using vertex_set_t = std::set<vertex_t>;
        using directed_edge_t = directed_edge_t<V>;
        using vertex_list_t = std::vector<vertex_t>;
        using edge_list_t = std::vector<directed_edge_t>;
        using component_list_t = std::vector<vertex_set_t>;
        using edge_map_t = std::unordered_map<vertex_t, edge_list_t>;
        using transpose_callback_t = std::function<directed_edge_t (const directed_edge_t&)>;

        directed_graph_t() = default;

        vertex_set_t vertices() const {
            vertex_set_t set{};
            for (const auto& kvp : _graph)
                set.insert(kvp.first);
            return set;
        }

        bool add_vertex(vertex_t vertex) {
            auto it = _graph.find(vertex);
            if (it == std::end(_graph)) {
                auto result = _graph.insert(std::make_pair(
                    vertex,
                    edge_list_t{}));
                return result.second;
            }
            return false;
        }

        void add_edge(const directed_edge_t& edge) {
            edge_list_t* edges = nullptr;
            auto it = _graph.find(edge.source);
            if (it == std::end(_graph)) {
                auto result = _graph.insert(std::make_pair(
                    edge.source,
                    edge_list_t{}));
                edges = &result.first->second;
            } else {
                edges = &it->second;
            }
            edges->push_back(edge);
        }

        edge_list_t outgoing_edges(vertex_t source) const {
            auto it = _graph.find(source);
            if (it == std::end(_graph))
                return edge_list_t{};
            return it->second;
        }

        directed_graph_t<V> transpose(const transpose_callback_t& reverse_edge) {
            directed_graph_t<V> t{};
            for (const auto& kvp : _graph) {
                t.add_vertex(kvp.first);
                for (const auto& e : kvp.second)
                    t.add_edge(reverse_edge(e));
            }
            return t;
        }

        component_list_t strongly_connected_components(const transpose_callback_t& reverse_edge) const {
            component_list_t components{};

            uint32_t time = 0;
            vertex_set_t visited{};
            std::map<vertex_t, uint32_t> finishing_times{};

            std::function<void (vertex_t)> dfs = [&](vertex_t v) {
                ++time;
                visited.insert(v);
                const auto& outgoing = outgoing_edges(v);
                for (const auto& e : outgoing) {
                    if (!visited.count(e.dest) == 0)
                        dfs(e.dest);
                }
                finishing_times.insert(std::make_pair(v, ++time));
            };

            for (auto v : vertices()) {
                if (visited.count(v) == 0)
                    dfs(v);
            }

            const auto& transposed = transpose(reverse_edge);
            std::function<void (vertex_t, vertex_set_t&)> dfs2 = [&](vertex_t v, vertex_set_t& component) {
                visited.insert(v);
                component.push_back(v);
                const auto& outgoing = transposed.outgoing_edges(v);
                for (const auto& e : outgoing) {
                    if (visited.count(e.dest) == 0)
                        dfs2(e.dest, component);
                }
            };

            vertex_list_t ordered_vertices{};
            for (auto it = finishing_times.rbegin(); it != finishing_times.rend(); ++it) {
                ordered_vertices.push_back((*it).first);
            }

            visited.clear();

            for (auto v : ordered_vertices) {
                if (visited.count(v) == 0) {
                    components.emplace_back();
                    auto& component = components.back();
                    dfs2(v, component);
                }
            }

            return components;
        }

    private:
        edge_map_t _graph{};
    };

}