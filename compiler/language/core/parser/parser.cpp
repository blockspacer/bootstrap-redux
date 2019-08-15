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
#include <compiler/data/hashable.h>
#include "parser.h"

namespace basecode::compiler::data {

    template <> uint64_t hash_key(language::core::parser::token_selector_t key) {
        return hashing::murmur::hash64(
            &key.type,
            sizeof(language::core::lexer::token_type_t));
    }

}

namespace basecode::compiler::language::core::parser {

    parser_t::parser_t(
            workspace::session_t& session,
            utf8::source_buffer_t& buffer,
            entity_list_t tokens) : _tokens(std::move(tokens)),
                                    _session(session),
                                    _buffer(buffer),
                                    _symbols(session.allocator()),
                                    _frame_allocator(session.allocator()),
                                    _symbol_table(session.allocator()) {
    }

    symbol_t* parser_t::infix(
            token_selector_t selector,
            int32_t bp,
            const led_callback_t& led) {
        auto s = symbol(selector.type, bp, selector.value);
        if (led) {
            s->led = led;
        } else {
            s->led = [&](context_t& ctx, entity_t lhs) {
                auto ast_node = ctx.registry->create();

                auto& node = ctx.registry->assign<ast::node_t>(ast_node);
                node.type = ast::node_type_t::binary_operator;
                node.token = ctx.token;
                node.parent = ctx.parent;

                auto& binary_op = ctx.registry->assign<ast::binary_operator_t>(ast_node);
                binary_op.lhs = lhs;
                binary_op.rhs = expression(*ctx.r, ctx.symbol->lbp);

                return ast_node;
            };
        }
        return s;
    }

    symbol_t* parser_t::prefix(
            lexer::token_type_t type,
            std::string_view token) {
        return prefix(type, {}, token);
    }

    symbol_t* parser_t::prefix(
            lexer::token_type_t type,
            const nud_callback_t& nud,
            std::string_view token) {
        auto s = symbol(type, 0, token);
        if (nud) {
            s->nud = nud;
        } else {
            s->nud = [&](context_t& ctx) {
                auto ast_node = ctx.registry->create();

                auto& node = ctx.registry->assign<ast::node_t>(ast_node);
                node.type = ast::node_type_t::unary_operator;
                node.token = ctx.token;
                node.parent = ctx.parent;

                auto& unary_op = ctx.registry->assign<ast::unary_operator_t>(ast_node);
                unary_op.lhs = expression(*ctx.r, 70);

                return ast_node;
            };
        }
        return s;
    }

    symbol_t* parser_t::statement(
            token_selector_t selector,
            int32_t bp) {
        return infix(
            selector,
            bp,
            [&](context_t& ctx, entity_t lhs) {
                auto ast_node = ctx.registry->create();

                auto& node = ctx.registry->assign<ast::node_t>(ast_node);
                node.type = ast::node_type_t::statement;
                node.token = ctx.token;
                node.parent = ctx.parent;

                auto& stmt = ctx.registry->assign<ast::statement_t>(ast_node);
                stmt.expr = lhs;

                // XXX: revisit
                if (ctx.block != entt::null) {
                    auto& block = ctx.registry->get<ast::block_t>(ctx.block);
                    block.children.add(ast_node);
                }

                return ast_node;
            });
    }

    symbol_t* parser_t::literal(
            lexer::token_type_t token_type,
            ast::node_type_t node_type) {
        auto literal = prefix(
            token_type,
            [&](context_t& ctx) {
                auto ast_node = ctx.registry->create();

                auto& node = ctx.registry->assign<ast::node_t>(ast_node);
                node.type = ctx.symbol->node_type;
                node.token = ctx.token;
                node.parent = ctx.parent;

                return ast_node;
            });
        literal->node_type = node_type;
        return literal;
    }

    symbol_t* parser_t::symbol(
            lexer::token_type_t type,
            int32_t bp,
            std::string_view token) {
        token_selector_t selector{
            .type = type,
            .value = token
        };

        auto s = _symbol_table.find(selector);

        if (s) {
            if (bp >= s->lbp)
                s->lbp = bp;
        } else {
            auto p = _frame_allocator.allocate(
                sizeof(symbol_t),
                alignof(symbol_t));
            s = new (p) symbol_t {
                .id = selector,
                .lbp = bp,
                .nud = [](context_t& ctx) {
                    const auto& loc = ctx.registry->get<source_location_t>(ctx.token);
                    ctx.parser->add_source_highlighted_error(
                        *ctx.r,
                        errors::parser::undefined_symbol,
                        loc);
                    return entt::null;
                },
                .led = [](context_t& ctx, entity_t lhs) {
                    const auto& loc = ctx.registry->get<source_location_t>(ctx.token);
                    ctx.parser->add_source_highlighted_error(
                        *ctx.r,
                        errors::parser::missing_operator,
                        loc);
                    return entt::null;
                },
            };

            _symbol_table.insert(selector, s);
        }

        return s;
    }

    symbol_t* parser_t::constant(
            lexer::token_type_t token_type,
            ast::node_type_t node_type) {
        auto constant = prefix(
            token_type,
            [](context_t& ctx) {
                auto ast_node = ctx.registry->create();

                auto& node = ctx.registry->assign<ast::node_t>(ast_node);
                node.type = ctx.symbol->node_type;
                node.token = ctx.token;
                node.parent = ctx.parent;

                return ast_node;
            });
        constant->node_type = node_type;
        return constant;
    }

    bool parser_t::has_more() const {
        return _token_index < _tokens.size()
            && _symbols[_token_index]->id.type != lexer::token_type_t::end_of_input;
    }

    bool parser_t::apply(result_t& r) {
        _symbols.reserve(_tokens.size());

        auto& registry = _session.registry();
        for (auto entity : _tokens) {
            const lexer::token_t& token = registry.get<lexer::token_t>(entity);
            auto s = find_symbol(token.type, token.value);
            if (!s) {
                const auto& loc = registry.get<source_location_t>(entity);
                add_source_highlighted_error(
                    r,
                    errors::parser::invalid_token,
                    loc);
                return false;
            }
            _symbols.add(s);
        }

        return true;
    }

    bool parser_t::initialize(result_t& r) {
        symbol(lexer::token_type_t::comma);
        symbol(lexer::token_type_t::semicolon);
        symbol(lexer::token_type_t::right_paren);
        symbol(lexer::token_type_t::right_bracket);
        symbol(lexer::token_type_t::else_keyword);
        symbol(lexer::token_type_t::end_of_input);
        symbol(lexer::token_type_t::right_curly_brace);

        prefix(lexer::token_type_t::operator_, "-");
        prefix(lexer::token_type_t::operator_, "!");
        prefix(lexer::token_type_t::operator_, "~");

        literal(lexer::token_type_t::block_literal, ast::node_type_t::block_literal);
        literal(lexer::token_type_t::number_literal, ast::node_type_t::number_literal);
        literal(lexer::token_type_t::string_literal, ast::node_type_t::string_literal);

        constant(lexer::token_type_t::nil_keyword, ast::node_type_t::nil_literal);
        constant(lexer::token_type_t::true_keyword, ast::node_type_t::boolean_literal);
        constant(lexer::token_type_t::false_keyword, ast::node_type_t::boolean_literal);

        infix(
            token_selector_t{.type = lexer::token_type_t::operator_, .value = "+"sv},
            50);
        infix(
            token_selector_t{.type = lexer::token_type_t::operator_, .value = "-"sv},
            50);
        infix(
            token_selector_t{.type = lexer::token_type_t::operator_, .value = "*"sv},
            60);
        infix(
            token_selector_t{.type = lexer::token_type_t::operator_, .value = "/"sv},
            60);
        infix(
            token_selector_t{.type = lexer::token_type_t::operator_, .value = "%"sv},
            60);
        infix(
            token_selector_t{.type = lexer::token_type_t::operator_, .value = ":="sv},
            90);

        prefix(
            lexer::token_type_t::identifier,
            [&](context_t& ctx) {
                auto ast_node = ctx.registry->create();

                auto& node = ctx.registry->assign<ast::node_t>(ast_node);
                node.type = ast::node_type_t::identifier;
                node.token = ctx.token;
                node.parent = ctx.parent;

                auto& identifier = ctx.registry->assign<ast::identifier_t>(ast_node);
                identifier.scope = ctx.scope;
                identifier.block = ctx.block;

                return ast_node;
            });

        prefix(
            lexer::token_type_t::annotation,
            [&](context_t& ctx) {
                auto ast_node = ctx.registry->create();

                auto& node = ctx.registry->assign<ast::node_t>(ast_node);
                node.type = ast::node_type_t::annotation;
                node.token = ctx.token;
                node.parent = ctx.parent;

                auto& annotation = ctx.registry->assign<ast::annotation_t>(ast_node);
                annotation.lhs = expression(*ctx.r, 0);
                annotation.rhs = expression(*ctx.r, 0);

                return ast_node;
            });

        prefix(
            lexer::token_type_t::directive,
            [&](context_t& ctx) {
                auto ast_node = ctx.registry->create();

                auto& node = ctx.registry->assign<ast::node_t>(ast_node);
                node.type = ast::node_type_t::directive;
                node.token = ctx.token;
                node.parent = ctx.parent;

                auto& directive = ctx.registry->assign<ast::directive_t>(ast_node);
                directive.lhs = expression(*ctx.r, 0);
                directive.rhs = expression(*ctx.r, 0);

                return ast_node;
            });

        statement(
            token_selector_t{.type = lexer::token_type_t::semicolon},
            80);

        return apply(r);
    }

    entity_t parser_t::expression(result_t& r, int32_t rbp) {
        if (!has_more()) return entt::null;

        auto current_token = _tokens[_token_index];
        auto current_symbol = _symbols[_token_index];

        context_t ctx{
            .r = &r,
            .parser = this,
            .scope = entt::null,        // XXX: fix me!
            .block = entt::null,        // XXX: fix me!
            .parent = entt::null,       // XXX: fix me!
            .token = current_token,
            .symbol = current_symbol,
            .registry = &_session.registry(),
        };

        if (!advance(r))
            return entt::null;

        auto lhs = current_symbol->nud(ctx);

        auto next_symbol = _symbols[_token_index];
        while (rbp < next_symbol->lbp) {
            ctx.symbol = next_symbol;
            ctx.token = _tokens[_token_index];

            if (!advance(r)) return entt::null;

            lhs = next_symbol->led(ctx, lhs);

            next_symbol = _symbols[_token_index];
        }

        return lhs;
    }

    bool parser_t::expect(result_t& r, std::string_view token) {
        if (token.empty()) return true;

        auto& registry = _session.registry();
        auto entity = _tokens[_token_index];
        const auto& t = registry.get<lexer::token_t>(entity);
        if (token != t.value) {
            const auto& loc = registry.get<source_location_t>(entity);
            add_source_highlighted_error(
                r,
                errors::parser::unexpected_token,
                loc,
                token,
                t.value);
            return false;
        }

        return true;
    }

    bool parser_t::advance(result_t& r, std::string_view token) {
        if (_token_index < _tokens.size()) {
            ++_token_index;
            return expect(r, token);
        } else {
            return false;
        }
    }

    symbol_t* parser_t::find_symbol(lexer::token_type_t type, std::string_view token) {
        token_selector_t selector{
            .type = type,
            .value = token
        };
        return _symbol_table.find(selector);
    }

    void parser_t::infix_right(std::string_view token, int32_t bp, const led_callback_t& led) {
    }

}
