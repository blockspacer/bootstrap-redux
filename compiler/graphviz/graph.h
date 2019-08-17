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

#include <compiler/data/array.h>
#include <compiler/memory/object_pool.h>
#include "edge.h"
#include "node.h"

namespace basecode::compiler::graphviz {

    enum class graph_type_t {
        undirected,
        directed
    };

    class graph_t;

    using node_list_t  = data::array_t<node_t*>;
    using edge_list_t  = data::array_t<edge_t*>;
    using graph_list_t = data::array_t<graph_t*>;

    class graph_t final {
    public:
        graph_t(
            memory::allocator_t* allocator,
            graph_type_t type,
            std::string_view name);

        [[nodiscard]] graph_type_t type() const;

    private:
        graph_type_t _type;
        edge_list_t _edges;
        node_list_t _nodes;
        std::string_view _name;
        graph_list_t _subgraphs;
        memory::object_pool_t _storage;
    };

}