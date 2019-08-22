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

#include <compiler/io/text.h>
#include <compiler/graphviz/graph.h>
#include <compiler/graphviz/dot_model.h>
#include <compiler/formatters/formatters.h>
#include <compiler/language/core/lexer/token.h>
#include "ast.h"

namespace basecode::language::core::ast {

    std::string_view node_type_to_name(node_type_t type) {
        switch (type) {
            case node_type_t::label:                    return "label"sv;
            case node_type_t::scope:                    return "scope"sv;
            case node_type_t::block:                    return "block"sv;
            case node_type_t::module:                   return "module"sv;
            case node_type_t::statement:                return "statement"sv;
            case node_type_t::directive:                return "directive"sv;
            case node_type_t::annotation:               return "annotation"sv;
            case node_type_t::identifier:               return "identifier"sv;
            case node_type_t::expression:               return "expression"sv;
            case node_type_t::nil_literal:              return "nil_literal"sv;
            case node_type_t::line_comment:             return "line_comment"sv;
            case node_type_t::block_comment:            return "block_comment"sv;
            case node_type_t::block_literal:            return "block_literal"sv;
            case node_type_t::ns_expression:            return "ns_expression"sv;
            case node_type_t::if_expression:            return "if_expression"sv;
            case node_type_t::in_expression:            return "in_expression"sv;
            case node_type_t::string_literal:           return "string_literal"sv;
            case node_type_t::number_literal:           return "number_literal"sv;
            case node_type_t::type_parameter:           return "type_parameter"sv;
            case node_type_t::unary_operator:           return "unary_operator"sv;
            case node_type_t::for_expression:           return "for_expression"sv;
            case node_type_t::use_expression:           return "use_expression"sv;
            case node_type_t::binary_operator:          return "binary_operator"sv;
            case node_type_t::cast_expression:          return "cast_expression"sv;
            case node_type_t::case_expression:          return "case_expression"sv;
            case node_type_t::proc_expression:          return "proc_expression"sv;
            case node_type_t::enum_expression:          return "enum_expression"sv;
            case node_type_t::with_expression:          return "with_expression"sv;
            case node_type_t::goto_expression:          return "goto_expression"sv;
            case node_type_t::boolean_literal:          return "boolean_literal"sv;
            case node_type_t::while_expression:         return "while_expression"sv;
            case node_type_t::defer_expression:         return "defer_expression"sv;
            case node_type_t::break_expression:         return "break_expression"sv;
            case node_type_t::union_expression:         return "union_expression"sv;
            case node_type_t::yield_expression:         return "yield_expression"sv;
            case node_type_t::struct_expression:        return "struct_expression"sv;
            case node_type_t::module_expression:        return "module_expression"sv;
            case node_type_t::import_expression:        return "import_expression"sv;
            case node_type_t::return_expression:        return "return_expression"sv;
            case node_type_t::family_expression:        return "family_expression"sv;
            case node_type_t::switch_expression:        return "switch_expression"sv;
            case node_type_t::bitcast_expression:       return "bitcast_expression"sv;
            case node_type_t::type_decl_operator:       return "type_decl_operator"sv;
            case node_type_t::value_sink_operator:      return "value_sink_operator"sv;
            case node_type_t::assignment_operator:      return "assignment_operator"sv;
            case node_type_t::continue_expression:      return "continue_expression"sv;
            case node_type_t::variable_declaration:     return "variable_declaration"sv;
            case node_type_t::fallthrough_expression:   return "fallthrough_expression"sv;
            case node_type_t::initializer_expression:   return "initializer_expression"sv;
            default: return "unknown"sv;
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    static graphviz::node_t* create_dot_node(
            result_t& r,
            workspace::session_t& session,
            graphviz::graph_t& graph,
            entity_t entity) {
        auto& registry = session.registry();
        auto& intern_pool = session.intern_pool();

        auto node_name = intern_pool.intern(fmt::format("id_{}", entity));
        auto node = graph.make_node(node_name);

        auto& node_attrs = node->attributes();
        node_attrs.set_value(
            r,
            graphviz::attribute_type_t::shape,
            graphviz::enumeration_value_t("record"));
        node_attrs.set_value(
            r,
            graphviz::attribute_type_t::style,
            graphviz::enumeration_value_t("filled"));

        data::array_t<data::string_t> ports(session.allocator());

        auto& ast_node = registry.get<node_t>(entity);
        ports.emplace(node_type_to_name(ast_node.type), session.allocator());

        if (ast_node.token != entt::null) {
            auto token = registry.get<lexer::token_t>(ast_node.token);
            ports.emplace(token.value, session.allocator());
        }

        switch (ast_node.type) {
            case node_type_t::block: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("azure3"));
                const auto& block_node = registry.get<block_t>(entity);
                auto scope_node = create_dot_node(r, session, graph, block_node.scope);
                graph.make_edge(node, scope_node);
                for (auto stmt : block_node.children) {
                    auto stmt_node = create_dot_node(r, session, graph, stmt);
                    graph.make_edge(node, stmt_node);
                }
                break;
            }
            case node_type_t::scope: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("cadetblue3"));
                break;
            }
            case node_type_t::module: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("deepskyblue3"));
                const auto& module_node = registry.get<module_t>(entity);
                auto block_node = create_dot_node(r, session, graph, module_node.block);
                graph.make_edge(node, block_node);
                break;
            }
            case node_type_t::statement: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("darkolivegreen1"));
                const auto& stmt_node = registry.get<statement_t>(entity);
                auto expr_node = create_dot_node(r, session, graph, stmt_node.expr);
                graph.make_edge(node, expr_node);
                break;
            }
            case node_type_t::directive: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("gold"));
                const auto& directive_node = registry.get<directive_t>(entity);
                auto lhs_expr = create_dot_node(r, session, graph, directive_node.lhs);
                auto rhs_expr = create_dot_node(r, session, graph, directive_node.rhs);
                auto lhs_edge = graph.make_edge(node, lhs_expr);
                lhs_edge->attributes().set_value(
                    r,
                    graphviz::attribute_type_t::label,
                    data::string_t("lhs"sv, session.allocator()));
                auto rhs_edge = graph.make_edge(node, rhs_expr);
                rhs_edge->attributes().set_value(
                    r,
                    graphviz::attribute_type_t::label,
                    data::string_t("rhs"sv, session.allocator()));
                break;
            }
            case node_type_t::annotation: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("goldenrod"));
                const auto& annotation_node = registry.get<annotation_t>(entity);
                auto lhs_expr = create_dot_node(r, session, graph, annotation_node.lhs);
                auto rhs_expr = create_dot_node(r, session, graph, annotation_node.rhs);
                auto lhs_edge = graph.make_edge(node, lhs_expr);
                lhs_edge->attributes().set_value(
                    r,
                    graphviz::attribute_type_t::label,
                    data::string_t("lhs"sv, session.allocator()));
                auto rhs_edge = graph.make_edge(node, rhs_expr);
                rhs_edge->attributes().set_value(
                    r,
                    graphviz::attribute_type_t::label,
                    data::string_t("rhs"sv, session.allocator()));
                break;
            }
            case node_type_t::unary_operator: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("cyan"));
                const auto& unary_op_node = registry.get<unary_operator_t>(entity);
                auto lhs_expr = create_dot_node(r, session, graph, unary_op_node.lhs);
                auto lhs_edge = graph.make_edge(node, lhs_expr);
                lhs_edge->attributes().set_value(
                    r,
                    graphviz::attribute_type_t::label,
                    data::string_t("lhs"sv, session.allocator()));
                break;
            }
            case node_type_t::binary_operator: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("cornsilk3"));
                const auto& bin_op_node = registry.get<binary_operator_t>(entity);
                auto lhs_expr = create_dot_node(r, session, graph, bin_op_node.lhs);
                auto rhs_expr = create_dot_node(r, session, graph, bin_op_node.rhs);
                auto lhs_edge = graph.make_edge(node, lhs_expr);
                lhs_edge->attributes().set_value(
                    r,
                    graphviz::attribute_type_t::label,
                    data::string_t("lhs"sv, session.allocator()));
                auto rhs_edge = graph.make_edge(node, rhs_expr);
                rhs_edge->attributes().set_value(
                    r,
                    graphviz::attribute_type_t::label,
                    data::string_t("rhs"sv, session.allocator()));
                break;
            }
            case node_type_t::identifier: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("aliceblue"));
                break;
            }
            case node_type_t::nil_literal: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("deeppink"));
                break;
            }
            case node_type_t::string_literal: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("aquamarine"));
                break;
            }
            case node_type_t::number_literal: {
                const auto& number_token = registry.get<number_token_t>(ast_node.token);
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("bisque"));
                ports.emplace(
                    fmt::format("\\{{ type: {}", number_type_to_name(number_token.type)).c_str(),
                    session.allocator());

                ports.emplace(
                    fmt::format("radix: {}", number_token.radix).c_str(),
                    session.allocator());

                ports.emplace(
                    fmt::format("size: {}", number_size_to_name(number_token.size)).c_str(),
                    session.allocator());

                if (number_token.type == number_type_t::floating_point) {
                    ports.emplace(
                        fmt::format("imaginary: {}", number_token.imaginary).c_str(),
                        session.allocator());
                }

                ports.emplace(
                    fmt::format("signed: {} \\}}", number_token.is_signed).c_str(),
                    session.allocator());
                break;
            }
            case node_type_t::boolean_literal: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("cornflowerblue"));
                break;
            }
            case node_type_t::assignment_operator: {
                node_attrs.set_value(
                    r,
                    graphviz::attribute_type_t::fillcolor,
                    graphviz::enumeration_value_t("darkseagreen1"));
                const auto& assignment_op_node = registry.get<assignment_operator_t>(entity);
                auto lhs_expr = create_dot_node(r, session, graph, assignment_op_node.lhs);
                auto rhs_expr = create_dot_node(r, session, graph, assignment_op_node.rhs);
                auto lhs_edge = graph.make_edge(node, lhs_expr);
                lhs_edge->attributes().set_value(
                    r,
                    graphviz::attribute_type_t::label,
                    data::string_t("lhs"sv, session.allocator()));
                auto rhs_edge = graph.make_edge(node, rhs_expr);
                rhs_edge->attributes().set_value(
                    r,
                    graphviz::attribute_type_t::label,
                    data::string_t("rhs"sv, session.allocator()));
                break;
            }
            case node_type_t::label:
            case node_type_t::expression:
            case node_type_t::line_comment:
            case node_type_t::block_comment:
            case node_type_t::block_literal:
            case node_type_t::ns_expression:
            case node_type_t::if_expression:
            case node_type_t::in_expression:
            case node_type_t::type_parameter:
            case node_type_t::for_expression:
            case node_type_t::use_expression:
            case node_type_t::cast_expression:
            case node_type_t::case_expression:
            case node_type_t::proc_expression:
            case node_type_t::enum_expression:
            case node_type_t::with_expression:
            case node_type_t::goto_expression:
            case node_type_t::while_expression:
            case node_type_t::defer_expression:
            case node_type_t::break_expression:
            case node_type_t::union_expression:
            case node_type_t::yield_expression:
            case node_type_t::struct_expression:
            case node_type_t::module_expression:
            case node_type_t::import_expression:
            case node_type_t::return_expression:
            case node_type_t::family_expression:
            case node_type_t::switch_expression:
            case node_type_t::bitcast_expression:
            case node_type_t::type_decl_operator:
            case node_type_t::value_sink_operator:
            case node_type_t::continue_expression:
            case node_type_t::variable_declaration:
            case node_type_t::fallthrough_expression:
            case node_type_t::initializer_expression:
                break;
        }

        fmt::memory_buffer label{};
        for (size_t i = 0; i < ports.size(); i++) {
            const auto& port = ports[i];
            fmt::format_to(label, "{}", port);
            if (i < ports.size() - 1)
                fmt::format_to(label, "\\|");
        }

        auto label_str = fmt::to_string(label);
        node_attrs.set_value(
            r,
            graphviz::attribute_type_t::label,
            data::string_t(label_str.c_str(), session.allocator()));

        return node;
    }

    bool write_dot_graph(
            result_t& r,
            workspace::session_t& session,
            const path_t& path,
            entity_t root) {
        graphviz::dot_model_t model(
            session.allocator(),
            session.intern_pool());

        if (!model.initialize(r))
            return false;

        graphviz::graph_t graph(
            session.allocator(),
            &model,
            graphviz::graph_type_t::directed,
            "test");

        auto& graph_attrs = graph.attributes();
        graph_attrs.set_value(
            r,
            graphviz::attribute_type_t::rankdir,
            graphviz::enumeration_value_t("TB"));
        graph_attrs.set_value(
            r,
            graphviz::attribute_type_t::fontsize,
            16.0);

        create_dot_node(r, session, graph, root);

        fmt::memory_buffer buffer{};
        if (!model.serialize(r, graph, buffer))
            return false;

        return io::text::write(r, path, fmt::to_string(buffer));
    }

}