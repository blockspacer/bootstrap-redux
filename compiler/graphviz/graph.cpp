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

#include "graph.h"

namespace basecode::compiler::graphviz {

    graph_t::graph_t(
            memory::allocator_t* allocator,
            graph_type_t type,
            std::string_view name) : _type(type),
                                     _edges(allocator),
                                     _nodes(allocator),
                                     _name(name),
                                     _subgraphs(allocator),
                                     _storage(allocator) {
    }

    graph_type_t graph_t::type() const {
        return _type;
    }

}