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

#include <basecode/errors/errors.h>
#include <basecode/formatters/formatters.h>
#include "graph.h"
#include "dot_model.h"
#include "attribute.h"

namespace basecode::graphviz {

    dot_model_t::dot_model_t(
            memory::allocator_t* allocator,
            strings::pool_t& intern_pool) : _attributes(allocator),
                                            _intern_pool(intern_pool),
                                            _allocator(allocator) {
        assert(_allocator);
    }

    bool dot_model_t::serialize(
            result_t& r,
            graph_t& graph,
            fmt::memory_buffer& buffer) {
        const auto node_connector = graph.type() == graph_type_t::directed ? "->" : "--";

        if (graph.type() == graph_type_t::directed) {
            fmt::format_to(buffer, "digraph {} {{\n", graph.name());
        } else {
            fmt::format_to(buffer, "graph {} {{\n", graph.name());
        }

        for (auto attr : graph.attributes().values()) {
            fmt::format_to(buffer, "\t");
            if (!serialize_attribute(r, attr, buffer))
                return false;
            fmt::format_to(buffer, ";\n");
        }

        fmt::format_to(buffer, "\n");

        for (auto node : graph.nodes()) {
            fmt::format_to(buffer, "\t{}", node->name());
            const auto& node_attrs = node->attributes().values();
            if (!node_attrs.empty()) {
                fmt::format_to(buffer, " [ ");
                if (!serialize_attributes(r, node_attrs, ", "sv, buffer))
                    return false;
                fmt::format_to(buffer, " ]");
            }
            fmt::format_to(buffer, ";\n");
        }

        fmt::format_to(buffer, "\n");

        for (auto edge : graph.edges()) {
            fmt::format_to(
                buffer,
                "\t{} {} {}",
                edge->first()->name(),
                node_connector,
                edge->second()->name());
            const auto& edge_attrs = edge->attributes().values();
            if (!edge_attrs.empty()) {
                fmt::format_to(buffer, " [ ");
                if (!serialize_attributes(r, edge_attrs, ", "sv, buffer))
                    return false;
                fmt::format_to(buffer, " ]");
            }
            fmt::format_to(buffer, ";\n");
        }

        fmt::format_to(buffer, "\n}}");

        return true;
    }

    bool dot_model_t::is_attribute_valid(
            result_t& r,
            component_type_t component,
            attribute_type_t type) {
        auto metadata = _attributes.find(type);
        if (!metadata) {
            errors::add_error(
                r,
                _intern_pool,
                errors::graphviz::attribute_type_not_found);
            return false;
        }
        const auto component_flag = static_cast<uint8_t>(component);
        auto valid = (metadata->component_flags & component_flag) == component_flag;
        if (!valid) {
            errors::add_error(
                r,
                _intern_pool,
                errors::graphviz::invalid_attribute_for_component,
                metadata->name,
                component_type_to_name(component));
        }
        return valid;
    }

    bool dot_model_t::initialize(result_t& r) {
        _attributes = metadata_table_t {
            {
                {
                    attribute_type_t::rankdir,
                    metadata_t{
                        "rankdir",
                        graph_flag
                    }
                },
                {
                    attribute_type_t::fontsize,
                    metadata_t{
                        "fontsize",
                        graph_flag | node_flag | edge_flag | cluster_subgraph_flag
                    }
                },
                {
                    attribute_type_t::label,
                    metadata_t{
                        "label",
                        graph_flag | node_flag | edge_flag | cluster_subgraph_flag
                    }
                },
                {
                    attribute_type_t::fillcolor,
                    metadata_t{
                        "fillcolor",
                        node_flag | edge_flag | cluster_subgraph_flag
                    }
                },
                {
                    attribute_type_t::labelloc,
                    metadata_t{
                        "labelloc",
                        graph_flag | node_flag | cluster_subgraph_flag
                    }
                },
                {
                    attribute_type_t::shape,
                    metadata_t{
                        "shape",
                        node_flag
                    }
                },
                {
                    attribute_type_t::style,
                    metadata_t{
                        "style",
                        graph_flag | node_flag | edge_flag | cluster_subgraph_flag
                    }
                },
            },
            _allocator
        };
        return true;
    }

    bool dot_model_t::serialize_attributes(
            result_t& r,
            const attribute_value_list_t& attrs,
            std::string_view separator,
            fmt::memory_buffer& buffer) {
        for (size_t i = 0; i < attrs.size(); i++) {
            if (!serialize_attribute(r, attrs[i], buffer))
                return false;
            if (i < attrs.size() - 1)
                fmt::format_to(buffer, "{}", separator);
        }
        return true;
    }

    bool dot_model_t::serialize_attribute(
            result_t& r,
            attribute_value_t* attr,
            fmt::memory_buffer& buffer) {
        switch (attr->value_type) {
            case attribute_value_type_t::string: {
                fmt::format_to(
                    buffer,
                    "{}=\"{}\"",
                    attribute_type_to_name(attr->type),
                    escape_chars(*attr->value.string));
                break;
            }
            case attribute_value_type_t::boolean: {
                fmt::format_to(
                    buffer,
                    "{}={}",
                    attribute_type_to_name(attr->type),
                    attr->value.flag);
                break;
            }
            case attribute_value_type_t::integer: {
                fmt::format_to(
                    buffer,
                    "{}={}",
                    attribute_type_to_name(attr->type),
                    attr->value.integer);
                break;
            }
            case attribute_value_type_t::enumeration: {
                fmt::format_to(
                    buffer,
                    "{}={}",
                    attribute_type_to_name(attr->type),
                    *attr->value.string);
                break;
            }
            case attribute_value_type_t::floating_point: {
                fmt::format_to(
                    buffer,
                    "{}={}",
                    attribute_type_to_name(attr->type),
                    attr->value.floating_point);
                break;
            }
        }
        return true;
    }

    data::string_t dot_model_t::escape_chars(const data::string_t& value) {
        data::string_t buffer(value.allocator());
        buffer.reserve(value.size() * 2);
        for (size_t i = 0 ; i < value.size(); i++) {
            const auto& c = value[i];
            if (c == '\\') {
                ++i;
                if (value[i] == '|') {
                    buffer.append('|');
                } else if (value[i] == '{') {
                    buffer.append('{');
                } else if (value[i] == '}') {
                    buffer.append('}');
                }
            } else {
                if (c == '\"') {
                    buffer.append("\\\"");
                } else if (c == '{') {
                    buffer.append("\\{");
                } else if (c == '}') {
                    buffer.append("\\}");
                } else if (c == '.') {
                    buffer.append("\\.");
                } else if (c == ',') {
                    buffer.append("\\,");
                } else if (c == '|') {
                    buffer.append("\\|");
                } else if (c == '<') {
                    buffer.append("\\<");
                } else if (c == '>') {
                    buffer.append("\\>");
                } else if (c == '=') {
                    buffer.append("\\=");
                } else {
                    buffer.append(c);
                }
            }
        }
        return buffer;
    }

    std::string_view dot_model_t::attribute_type_to_name(attribute_type_t type) {
        auto metadata = _attributes.find(type);
        if (!metadata)
            return "unknown"sv;
        return metadata->name;
    }

}