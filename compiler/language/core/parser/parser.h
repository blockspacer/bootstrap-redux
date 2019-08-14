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

#include <boost/any.hpp>
#include <compiler/types.h>
#include <compiler/errors/errors.h>
#include <compiler/workspace/session.h>
#include <compiler/language/core/ast/ast.h>
#include <compiler/memory/frame_allocator.h>
#include <compiler/language/core/lexer/token.h>

namespace basecode::compiler::language::core::parser {

    using namespace std::literals;

    struct context_t;

    using nud_callback_t = std::function<entity_t (context_t&)>;
    using led_callback_t = std::function<entity_t (context_t&, entity_t)>;

    struct token_selector_t final {
        std::string_view value{};
        lexer::token_type_t type{};

        bool operator == (const token_selector_t& other) const {
            if (value.empty()) {
                return type == other.type;
            } else {
                return type == other.type && value == other.value;
            }
        }
    };

    struct symbol_t final {
        int32_t lbp{};
        nud_callback_t nud{};
        led_callback_t led{};
        token_selector_t id{};

        // XXX: need to think of a better way to do this
        ast::node_type_t node_type{};
        // XXX: fix this shit
    };

    struct context_t final {
        result_t* r{};
        entity_t block{};
        entity_t scope{};
        entity_t token{};
        entity_t parent{};
        symbol_t* symbol{};
        workspace::session_t* session{};
        utf8::source_buffer_t* buffer{};
    };

    ///////////////////////////////////////////////////////////////////////////

    class parser_t final {
    public:
        parser_t(
            workspace::session_t& session,
            utf8::source_buffer_t& buffer,
            entity_list_t tokens);

        bool initialize(result_t& r);

        entity_t expression(result_t& r, int32_t rbp = 0);

    private:
        bool apply(result_t& r);

        [[nodiscard]] bool has_more() const;

        bool expect(result_t& r, std::string_view token = {});

        bool advance(result_t& r, std::string_view token = {});

    private:
        symbol_t* infix(
            token_selector_t selector,
            int32_t bp,
            const led_callback_t& led = {});

        symbol_t* prefix(
            lexer::token_type_t type,
            std::string_view token);

        symbol_t* prefix(
            lexer::token_type_t type,
            const nud_callback_t& nud,
            std::string_view token = {});

        symbol_t* statement(
            token_selector_t selector,
            int32_t bp);

        symbol_t* literal(
            lexer::token_type_t token_type,
            ast::node_type_t node_type);

        symbol_t* symbol(
            lexer::token_type_t type,
            int32_t bp = 0,
            std::string_view token = {});

        symbol_t* constant(
            lexer::token_type_t token_type,
            ast::node_type_t node_type);

        symbol_t* find_symbol(lexer::token_type_t type, std::string_view token = {});

        void infix_right(std::string_view token, int32_t bp, const led_callback_t& led = {});

    private:
        size_t _token_index{};
        entity_list_t _tokens;
        workspace::session_t& _session;
        utf8::source_buffer_t& _buffer;
        data::array_t<symbol_t*> _symbols;
        memory::frame_allocator_t<4096> _frame_allocator;
        data::hash_table_t<token_selector_t, symbol_t*> _symbol_table;
    };

}
