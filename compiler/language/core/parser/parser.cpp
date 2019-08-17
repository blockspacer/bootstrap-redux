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

namespace basecode::compiler::language::core::parser {

    parser_t::parser_t(
            workspace::session_t& session,
            utf8::source_buffer_t& buffer,
            entity_list_t tokens) : _tokens(std::move(tokens)),
                                    _session(session),
                                    _buffer(buffer),
                                    _scopes(session.allocator()),
                                    _blocks(session.allocator()),
                                    _parent(session.allocator()),
                                    _rules(session.allocator()),
                                    _frame_allocator(session.allocator()),
                                    _rule_table(session.allocator()) {
    }

    bool parser_t::has_more() const {
        return _token_index < _tokens.size()
               && _rules[_token_index]->id != lexer::token_type_t::end_of_input;
    }

    production_rule_t* parser_t::infix(
            lexer::token_type_t token_type,
            int32_t bp,
            const led_callback_t& led) {
        auto s = terminal(token_type, bp);
        if (led) {
            s->led = led;
        } else {
            s->led = [&](context_t& ctx, entity_t lhs) {
                auto ast_node = ctx.registry->create();
                ctx.registry->assign<ast::node_t>(
                    ast_node,
                    ctx.allocator,
                    ast::node_type_t::binary_operator,
                    ctx.token,
                    ctx.parent);
                auto& binary_op = ctx.registry->assign<ast::binary_operator_t>(ast_node);
                binary_op.lhs = lhs;
                binary_op.rhs = expression(*ctx.r, ctx.rule->lbp);
                return ast_node;
            };
        }
        return s;
    }

    production_rule_t* parser_t::prefix(
            lexer::token_type_t token_type,
            const nud_callback_t& nud) {
        auto s = terminal(token_type);
        if (nud) {
            s->nud = nud;
        } else {
            s->nud = [&](context_t& ctx) {
                auto ast_node = ctx.registry->create();
                ctx.registry->assign<ast::node_t>(
                    ast_node,
                    ctx.allocator,
                    ast::node_type_t::unary_operator,
                    ctx.token,
                    ctx.parent);
                auto& unary_op = ctx.registry->assign<ast::unary_operator_t>(ast_node);
                unary_op.lhs = expression(*ctx.r, 70);
                return ast_node;
            };
        }
        return s;
    }

    production_rule_t* parser_t::statement(
            lexer::token_type_t token_type,
            int32_t bp) {
        return infix(
            token_type,
            bp,
            [&](context_t& ctx, entity_t lhs) {
                auto ast_node = ctx.registry->create();
                ctx.registry->assign<ast::node_t>(
                    ast_node,
                    ctx.allocator,
                    ast::node_type_t::statement,
                    ctx.token,
                    ctx.parent);
                ctx.registry->assign<ast::statement_t>(
                    ast_node,
                    ctx.allocator,
                    lhs);
                auto& block = ctx.registry->get<ast::block_t>(ctx.block);
                block.children.add(ast_node);
                return ast_node;
            });
    }

    production_rule_t* parser_t::terminal(
            lexer::token_type_t token_type,
            int32_t bp) {
        auto s = _rule_table.find(token_type);

        if (s) {
            if (bp >= s->lbp)
                s->lbp = bp;
        } else {
            auto p = _frame_allocator.allocate(
                sizeof(production_rule_t),
                alignof(production_rule_t));
            s = new (p) production_rule_t {
                .id = token_type,
                .lbp = bp,
                .nud = [](context_t& ctx) {
                    const auto& loc = ctx.registry->get<source_location_t>(ctx.token);
                    ctx.parser->add_source_highlighted_error(
                        *ctx.r,
                        errors::parser::undefined_production_rule,
                        loc);
                    return entt::null;
                },
                .led = [](context_t& ctx, entity_t lhs) {
                    const auto& loc = ctx.registry->get<source_location_t>(ctx.token);
                    ctx.parser->add_source_highlighted_error(
                        *ctx.r,
                        errors::parser::missing_operator_production_rule,
                        loc);
                    return entt::null;
                },
            };

            _rule_table.insert(token_type, s);
        }

        return s;
    }

    production_rule_t* parser_t::literal(
            lexer::token_type_t token_type,
            ast::node_type_t node_type) {
        auto literal = prefix(
            token_type,
            [&](context_t& ctx) {
                auto ast_node = ctx.registry->create();
                ctx.registry->assign<ast::node_t>(
                    ast_node,
                    ctx.allocator,
                    ctx.rule->node_type,
                    ctx.token,
                    ctx.parent);
                return ast_node;
            });
        literal->node_type = node_type;
        return literal;
    }

    production_rule_t* parser_t::constant(
            lexer::token_type_t token_type,
            ast::node_type_t node_type) {
        auto constant = prefix(
            token_type,
            [](context_t& ctx) {
                auto ast_node = ctx.registry->create();
                ctx.registry->assign<ast::node_t>(
                    ast_node,
                    ctx.allocator,
                    ctx.rule->node_type,
                    ctx.token,
                    ctx.parent);
                return ast_node;
            });
        constant->node_type = node_type;
        return constant;
    }

    production_rule_t* parser_t::infix_right(
            lexer::token_type_t token_type,
            int32_t bp,
            const led_callback_t& led) {
        return nullptr;
    }

    bool parser_t::apply(result_t& r) {
        _rules.reserve(_tokens.size());

        auto& registry = _session.registry();
        for (auto entity : _tokens) {
            const lexer::token_t& token = registry.get<lexer::token_t>(entity);
            auto s = _rule_table.find(token.type);
            if (!s) {
                const auto& loc = registry.get<source_location_t>(entity);
                add_source_highlighted_error(
                    r,
                    errors::parser::invalid_token,
                    loc);
                return false;
            }
            _rules.add(s);
        }

        return true;
    }

    entity_t parser_t::parse(result_t& r) {
        auto& registry = _session.registry();

        auto scope_node = registry.create();
        registry.assign<ast::scope_t>(
            scope_node,
            _session.allocator());

        auto block_node = registry.create();
        registry.assign<ast::block_t>(
            block_node,
            _session.allocator(),
            scope_node);

        std::string_view name;
        if (_buffer.path().empty()) {
            name = _session.intern_pool().intern("(anonymous source)"sv);
        } else {
            auto base_filename = _buffer
                .path()
                .filename()
                .replace_extension("");
            name = _session.intern_pool().intern(base_filename.string());
        }

        auto module_node = registry.create();
        registry.assign<ast::module_t>(
            module_node,
            _session.allocator(),
            _buffer.path(),
            name,
            block_node);

        _scopes.push(scope_node);
        _blocks.push(block_node);
        _parent.push(block_node);

        while (true) {
            auto root = expression(r);
            if (root == entt::null) break;
        }

        _scopes.pop();
        assert(_scopes.empty());

        _blocks.pop();
        assert(_blocks.empty());

        _parent.pop();
        assert(_parent.empty());

        return module_node;
    }

    bool parser_t::initialize(result_t& r) {
        terminal(lexer::token_type_t::comma);
        terminal(lexer::token_type_t::semicolon);
        terminal(lexer::token_type_t::right_paren);
        terminal(lexer::token_type_t::right_bracket);
        terminal(lexer::token_type_t::else_keyword);
        terminal(lexer::token_type_t::end_of_input);
        terminal(lexer::token_type_t::right_curly_brace);

        prefix(lexer::token_type_t::minus);
        prefix(lexer::token_type_t::binary_not_operator);
        prefix(lexer::token_type_t::logical_not_operator);

        literal(lexer::token_type_t::block_literal, ast::node_type_t::block_literal);
        literal(lexer::token_type_t::number_literal, ast::node_type_t::number_literal);
        literal(lexer::token_type_t::string_literal, ast::node_type_t::string_literal);

        constant(lexer::token_type_t::nil_keyword, ast::node_type_t::nil_literal);
        constant(lexer::token_type_t::true_keyword, ast::node_type_t::boolean_literal);
        constant(lexer::token_type_t::false_keyword, ast::node_type_t::boolean_literal);

        infix(lexer::token_type_t::minus, 50);
        infix(lexer::token_type_t::add_operator, 50);
        infix(lexer::token_type_t::divide_operator, 60);
        infix(lexer::token_type_t::modulo_operator, 60);
        infix(lexer::token_type_t::multiply_operator, 60);
        infix(lexer::token_type_t::assignment_operator, 90);

        prefix(
            lexer::token_type_t::identifier,
            [&](context_t& ctx) {
                auto ast_node = ctx.registry->create();
                ctx.registry->assign<ast::node_t>(
                    ast_node,
                    ctx.allocator,
                    ast::node_type_t::identifier,
                    ctx.token,
                    ctx.parent);
                ctx.registry->assign<ast::identifier_t>(
                    ast_node,
                    ctx.scope,
                    ctx.block);
                auto& token = ctx.registry->get<lexer::token_t>(ctx.token);
                auto& scope = ctx.registry->get<ast::scope_t>(ctx.scope);
                scope.identifiers.insert(token.value, ast_node);
                return ast_node;
            });

        prefix(
            lexer::token_type_t::annotation,
            [&](context_t& ctx) {
                auto ast_node = ctx.registry->create();
                ctx.registry->assign<ast::node_t>(
                    ast_node,
                    ctx.allocator,
                    ast::node_type_t::annotation,
                    ctx.token,
                    ctx.parent);
                auto& annotation = ctx.registry->assign<ast::annotation_t>(ast_node);
                annotation.lhs = expression(*ctx.r, 0);
                annotation.rhs = expression(*ctx.r, 0);
                return ast_node;
            });

        prefix(
            lexer::token_type_t::directive,
            [&](context_t& ctx) {
                auto ast_node = ctx.registry->create();
                ctx.registry->assign<ast::node_t>(
                    ast_node,
                    ctx.allocator,
                    ast::node_type_t::directive,
                    ctx.token,
                    ctx.parent);
                auto& directive = ctx.registry->assign<ast::directive_t>(ast_node);
                directive.lhs = expression(*ctx.r, 0);
                directive.rhs = expression(*ctx.r, 0);
                return ast_node;
            });

        statement(lexer::token_type_t::semicolon, 100);

        return apply(r);
    }

    entity_t parser_t::expression(result_t& r, int32_t rbp) {
        if (!has_more()) return entt::null;

        auto current_token = _tokens[_token_index];
        auto current_symbol = _rules[_token_index];

        context_t ctx{
            .r = &r,
            .parser = this,
            .token = current_token,
            .rule = current_symbol,
            .scope = *_scopes.top(),
            .block = *_blocks.top(),
            .parent = *_parent.top(),
            .registry = &_session.registry(),
            .allocator = _session.allocator()
        };

        if (!advance(r))
            return entt::null;

        auto lhs = current_symbol->nud(ctx);

        auto next_symbol = _rules[_token_index];
        while (rbp < next_symbol->lbp) {
            ctx.rule = next_symbol;
            ctx.scope = *_scopes.top();
            ctx.block = *_blocks.top();
            ctx.parent = *_parent.top();
            ctx.token = _tokens[_token_index];

            if (!advance(r)) return entt::null;

            lhs = next_symbol->led(ctx, lhs);

            next_symbol = _rules[_token_index];
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

}
