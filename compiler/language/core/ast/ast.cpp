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

#include "ast.h"

namespace basecode::compiler::language::core::ast {

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

}