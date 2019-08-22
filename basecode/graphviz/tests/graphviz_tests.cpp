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

#include <catch2/catch.hpp>
#include <basecode/defer.h>
#include <basecode/graphviz/graph.h>
#include <basecode/graphviz/dot_model.h>
#include <basecode/formatters/formatters.h>

namespace basecode {

    using namespace std::literals;
    using namespace basecode;
    using namespace basecode::graphviz;

    TEST_CASE("graph_t") {
        auto allocator = memory::default_scratch_allocator();

        strings::pool_t intern_pool(allocator);

        result_t r(allocator);
        defer(fmt::print("{}", r));

        dot_model_t model(allocator, intern_pool);
        REQUIRE(model.initialize(r));

        graph_t graph(
            memory::default_scratch_allocator(),
            &model,
            graph_type_t::directed,
            "test");
        auto& attrs = graph.attributes();
        REQUIRE(attrs.set_value(r, attribute_type_t::rankdir, enumeration_value_t("LR")));
        REQUIRE(attrs.set_value(r, attribute_type_t::fontsize, 22.0));

        data::string_t temp;
        REQUIRE(attrs.get_value(r, attribute_type_t::rankdir, temp));
        REQUIRE(temp == "LR"sv);

        auto node1 = graph.make_node("node1"sv);
        REQUIRE(node1);
        REQUIRE(node1->attributes().set_value(r, attribute_type_t::style, enumeration_value_t("filled")));
        REQUIRE(node1->attributes().set_value(r, attribute_type_t::fillcolor, enumeration_value_t("grey")));

        auto node2 = graph.make_node("node2"sv);
        REQUIRE(node2);
        REQUIRE(node2->attributes().set_value(r, attribute_type_t::style, enumeration_value_t("filled")));
        REQUIRE(node2->attributes().set_value(r, attribute_type_t::fillcolor, enumeration_value_t("yellow")));

        auto node3 = graph.make_node("node3"sv);
        REQUIRE(node3);
        REQUIRE(node3->attributes().set_value(r, attribute_type_t::style, enumeration_value_t("filled")));
        REQUIRE(node3->attributes().set_value(r, attribute_type_t::fillcolor, enumeration_value_t("yellow")));

        auto edge1 = graph.make_edge(node1, node2);
        REQUIRE(edge1);
        auto edge2 = graph.make_edge(node1, node3);
        REQUIRE(edge2);

        fmt::memory_buffer buffer{};
        REQUIRE(model.serialize(r, graph, buffer));

        fmt::print("{}\n", fmt::to_string(buffer));
    }

}