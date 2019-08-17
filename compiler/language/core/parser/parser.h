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

#include <compiler/types.h>
#include <compiler/data/stack.h>
#include <compiler/errors/errors.h>
#include <compiler/workspace/session.h>
#include <compiler/language/core/ast/ast.h>
#include <compiler/memory/frame_allocator.h>
#include <compiler/language/core/lexer/token.h>

namespace basecode::compiler::language::core::parser {

    using namespace std::literals;

    struct context_t;

    using nud_callback_t = entity_t (*) (context_t&);
    using led_callback_t = entity_t (*) (context_t&, entity_t);

    struct production_rule_t final {
        int32_t lbp{};
        nud_callback_t nud{};
        led_callback_t led{};
        lexer::token_type_t id{};

        // XXX: need to think of a better way to do this
        ast::node_type_t node_type{};
        // XXX: fix this shit
    };

    class parser_t;

    struct context_t final {
        result_t* r{};
        entity_t block{};
        entity_t scope{};
        entity_t token{};
        entity_t parent{};
        parser_t* parser{};
        production_rule_t* rule{};
        entt::registry* registry{};
        memory::allocator_t* allocator{};
    };

    ///////////////////////////////////////////////////////////////////////////

    class parser_t final {
    public:
        parser_t(
            workspace::session_t& session,
            utf8::source_buffer_t& buffer,
            entity_list_t tokens);

        entity_t parse(result_t& r);

        bool initialize(result_t& r);

        entity_t expression(result_t& r, int32_t rbp = 0);

    private:
        bool apply(result_t& r);

        [[nodiscard]] bool has_more() const;

        [[nodiscard]] entity_t token() const;

        [[nodiscard]] production_rule_t* rule() const;

        bool expect(result_t& r, lexer::token_type_t type = {});

        bool advance(result_t& r, lexer::token_type_t type = {});

    private:
        template <typename... Args>
        void add_source_highlighted_error(
                result_t& r,
                errors::error_code_t code,
                const source_location_t& loc,
                Args&&... args) {
            errors::add_source_highlighted_error(
                r,
                _session.intern_pool(),
                code,
                _buffer,
                loc,
                std::forward<Args>(args)...);
        }

        bool is_node_an_identifier(entity_t expr, source_location_t& loc);

    private:
        production_rule_t* infix(
            lexer::token_type_t token_type,
            int32_t bp,
            const led_callback_t& led = {});

        production_rule_t* prefix(
            lexer::token_type_t token_type,
            const nud_callback_t& nud = {});

        production_rule_t* literal(
            lexer::token_type_t token_type,
            ast::node_type_t node_type);

        production_rule_t* terminal(
            lexer::token_type_t token_type,
            int32_t bp = 0);

        production_rule_t* constant(
            lexer::token_type_t token_type,
            ast::node_type_t node_type);

        production_rule_t* infix_right(
            lexer::token_type_t token_type,
            int32_t bp,
            const led_callback_t& led = {});

    private:
        size_t _token_index{};
        entity_list_t _tokens;
        workspace::session_t& _session;
        utf8::source_buffer_t& _buffer;
        data::stack_t<entity_t> _scopes;
        data::stack_t<entity_t> _blocks;
        data::stack_t<entity_t> _parent;
        data::array_t<production_rule_t*> _rules;
        memory::frame_allocator_t<4096> _frame_allocator;
        data::hash_table_t<lexer::token_type_t, production_rule_t*> _rule_table;
    };

}
