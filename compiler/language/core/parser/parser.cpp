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

#include <utility>
#include <compiler/errors/errors.h>
#include "parser.h"

namespace basecode::compiler::language::core::parser {

    parser_t::parser_t(
            workspace::session_t& session,
            utf8::source_buffer_t& buffer,
            entity_list_t tokens) : _tokens(std::move(tokens)),
                                    _session(session),
                                    _buffer(buffer) {
    }

    bool parser_t::advance(std::string_view expected) {
        if (_token_index < _tokens.size()) {
            ++_token_index;

            if (!expected.empty()) {
                // XXX: compare
            }

            return true;
        } else {
            return false;
        }
    }

    bool parser_t::apply(result_t& r) {
        for (auto entity : _tokens) {
        }
        return true;
    }

    bool parser_t::initialize(result_t& r) {
        constant("true"sv, 1);
        constant("false"sv, 0);
        constant("nil"sv, nullptr);

        infix("+"sv, 10);
        infix("-"sv, 10);
        infix("*"sv, 20);
        infix("/"sv, 20);
        infix("%"sv, 20);

        infix("["sv, 80, [&](auto r, auto lhs) {
            auto& registry = _session.registry();
            auto ast_node = registry.create();
            auto& op = registry.assign<ast::binary_operator_t>(ast_node);
            op.lhs = lhs;
            op.rhs = expression(r, 0);
            advance("]"sv);
            return ast_node;
        });

        infix_right("&&"sv, 30);
        infix_right("||"sv, 30);

        prefix("!");
        prefix("~");
        prefix("-");
        prefix("@");
        prefix("#");
        prefix("(", [&](auto r) {
            auto expr = expression(r);
            advance(")"sv);
            return expr;
        });

        return apply(r);
    }

    void parser_t::symbol(std::string_view token, int32_t bp) {
    }

    bool parser_t::token(result_t& r, token_info_t& info) const {
        auto& registry = _session.registry();
        info.token = _tokens[_token_index];
        info.loc = registry.get<source_location_t>(info.token);
        info.atom = registry.try_get<symbol_t>(info.token);
        if (!info.atom) {
            errors::add_source_highlighted_error(
                r,
                _session.intern_pool(),
                errors::parser::invalid_token,
                _buffer,
                info.loc);
            return false;
        }
        return true;
    }

    entt::entity parser_t::expression(result_t& r, int32_t rbp) {
        token_info_t t{};
        if (!token(r, t)) return entt::null;

        advance();
        auto lhs = t.atom->nud(r);

        if (!token(r, t)) return entt::null;
        while (rbp < t.atom->lbp) {
            advance();
            lhs = t.atom->led(r, lhs);
            if (!token(r, t)) return entt::null;
        }

        return lhs;
    }

    void parser_t::constant(std::string_view token, boost::any value) {
    }

    void parser_t::prefix(std::string_view token, const nud_callback_t& nud) {
//        [&](auto r) {
//            auto ast_node = _workspace.registry.create();
//            auto& unary_op = _workspace.registry.assign<unary_op_t>(ast_node);
//            unary_op_t.lhs = lhs;
//            return ast_node;
//        }
    }

    void parser_t::infix(std::string_view token, int32_t bp, const led_callback_t& led) {
//        [&](auto r, auto lhs) {
//            auto ast_node = _workspace.registry.create();
//            auto& binary_op = _workspace.registry.assign<binary_op_t>(ast_node);
//            binary_op.lhs = lhs;
//            binary_op.rhs = expression(r, bp);
//            return ast_node;
//        }
    }

    void parser_t::infix_right(std::string_view token, int32_t bp, const led_callback_t& led) {
//        [&](auto r, auto lhs) {
//            auto ast_node = _workspace.registry.create();
//            auto& binary_op = _workspace.registry.assign<binary_op_t>(ast_node);
//            binary_op.lhs = lhs;
//            binary_op.rhs = expression(r, bp - 1);
//            return ast_node;
//        }
    }

}
