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

#include <compiler/errors/types.h>
#include <compiler/numbers/bytes.h>
#include <compiler/numbers/parse.h>
#include <compiler/formatters/formatters.h>
#include "lexer.h"

namespace basecode::compiler::language::core::lexer {

    trie_t lexer_t::s_lexemes {
        {"1"sv,           lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"2"sv,           lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"3"sv,           lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"4"sv,           lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"5"sv,           lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"6"sv,           lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"7"sv,           lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"8"sv,           lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"9"sv,           lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},

        {"-1"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-2"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-3"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-4"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-5"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-6"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-7"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-8"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-9"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},

        {"$0"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$1"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$2"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$3"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$4"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$5"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$6"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$7"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$8"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$9"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$a"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$b"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$c"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$d"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$e"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$f"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},

        {"%0"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::binary_number_literal}},
        {"%1"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::binary_number_literal}},

        {"@0"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@1"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@2"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@3"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@4"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@5"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@6"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@7"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},

        {"\""sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::string_literal}},
        {"{{"sv,          lexeme_t{.type = token_type_t::literal, .tokenizer = &lexer_t::block_string_literal}},

        {"//"sv,          lexeme_t{.type = token_type_t::comment, .tokenizer = &lexer_t::line_comment}},
        {"--"sv,          lexeme_t{.type = token_type_t::comment, .tokenizer = &lexer_t::line_comment}},
        {"/*"sv,          lexeme_t{.type = token_type_t::comment, .tokenizer = &lexer_t::block_comment}},

        {"#"sv,           lexeme_t{.type = token_type_t::punctuation}},
        {"@"sv,           lexeme_t{.type = token_type_t::punctuation}},
        {"'"sv,           lexeme_t{.type = token_type_t::punctuation}},
        {":"sv,           lexeme_t{.type = token_type_t::punctuation}},
        {";"sv,           lexeme_t{.type = token_type_t::punctuation}},
        {","sv,           lexeme_t{.type = token_type_t::punctuation}},
        {"{"sv,           lexeme_t{.type = token_type_t::punctuation}},
        {"}"sv,           lexeme_t{.type = token_type_t::punctuation}},
        {"("sv,           lexeme_t{.type = token_type_t::punctuation}},
        {")"sv,           lexeme_t{.type = token_type_t::punctuation}},
        {"["sv,           lexeme_t{.type = token_type_t::punctuation}},
        {"]"sv,           lexeme_t{.type = token_type_t::punctuation}},
        {"\\"sv,          lexeme_t{.type = token_type_t::punctuation}},

        {"&"sv,           lexeme_t{.type = token_type_t::operator_}},
        {"|"sv,           lexeme_t{.type = token_type_t::operator_}},
        {"~"sv,           lexeme_t{.type = token_type_t::operator_}},
        {"**"sv,          lexeme_t{.type = token_type_t::operator_}},
        {"+"sv,           lexeme_t{.type = token_type_t::operator_}},
        {"-"sv,           lexeme_t{.type = token_type_t::operator_}},
        {"*"sv,           lexeme_t{.type = token_type_t::operator_}},
        {"/"sv,           lexeme_t{.type = token_type_t::operator_}},
        {"%"sv,           lexeme_t{.type = token_type_t::operator_}},

        {"&&"sv,          lexeme_t{.type = token_type_t::operator_}},
        {"||"sv,          lexeme_t{.type = token_type_t::operator_}},
        {">"sv,           lexeme_t{.type = token_type_t::operator_}},
        {"<"sv,           lexeme_t{.type = token_type_t::operator_}},
        {">="sv,          lexeme_t{.type = token_type_t::operator_}},
        {"<="sv,          lexeme_t{.type = token_type_t::operator_}},
        {"=="sv,          lexeme_t{.type = token_type_t::operator_}},
        {"!="sv,          lexeme_t{.type = token_type_t::operator_}},

        {":="sv,          lexeme_t{.type = token_type_t::operator_}},
        {"%:="sv,         lexeme_t{.type = token_type_t::operator_}},
        {"/:="sv,         lexeme_t{.type = token_type_t::operator_}},
        {"*:="sv,         lexeme_t{.type = token_type_t::operator_}},
        {"-:="sv,         lexeme_t{.type = token_type_t::operator_}},
        {"+:="sv,         lexeme_t{.type = token_type_t::operator_}},
        {"&:="sv,         lexeme_t{.type = token_type_t::operator_}},
        {"|:="sv,         lexeme_t{.type = token_type_t::operator_}},
        {"~:="sv,         lexeme_t{.type = token_type_t::operator_}},

        {"::"sv,          lexeme_t{.type = token_type_t::operator_}}, // bind operator
        {"=>"sv,          lexeme_t{.type = token_type_t::operator_}}, // association operator
        {"->"sv,          lexeme_t{.type = token_type_t::operator_}}, // placeholder

        {"in"sv,          lexeme_t{.type = token_type_t::operator_}},
        {"xor"sv,         lexeme_t{.type = token_type_t::operator_}},
        {"shl"sv,         lexeme_t{.type = token_type_t::operator_}},
        {"shr"sv,         lexeme_t{.type = token_type_t::operator_}},
        {"rol"sv,         lexeme_t{.type = token_type_t::operator_}},
        {"ror"sv,         lexeme_t{.type = token_type_t::operator_}},

        {"if"sv,          lexeme_t{.type = token_type_t::keyword}},
        {"ns"sv,          lexeme_t{.type = token_type_t::keyword}},
        {"for"sv,         lexeme_t{.type = token_type_t::keyword}},
        {"nil"sv,         lexeme_t{.type = token_type_t::keyword}},
        {"use"sv,         lexeme_t{.type = token_type_t::keyword}},
        {"true"sv,        lexeme_t{.type = token_type_t::keyword}},
        {"cast"sv,        lexeme_t{.type = token_type_t::keyword}},
        {"case"sv,        lexeme_t{.type = token_type_t::keyword}},
        {"proc"sv,        lexeme_t{.type = token_type_t::keyword}},
        {"enum"sv,        lexeme_t{.type = token_type_t::keyword}},
        {"else"sv,        lexeme_t{.type = token_type_t::keyword}},
        {"with"sv,        lexeme_t{.type = token_type_t::keyword}},
        {"goto"sv,        lexeme_t{.type = token_type_t::keyword}},
        {"false"sv,       lexeme_t{.type = token_type_t::keyword}},
        {"defer"sv,       lexeme_t{.type = token_type_t::keyword}},
        {"break"sv,       lexeme_t{.type = token_type_t::keyword}},
        {"union"sv,       lexeme_t{.type = token_type_t::keyword}},
        {"yield"sv,       lexeme_t{.type = token_type_t::keyword}},
        {"struct"sv,      lexeme_t{.type = token_type_t::keyword}},
        {"return"sv,      lexeme_t{.type = token_type_t::keyword}},
        {"switch"sv,      lexeme_t{.type = token_type_t::keyword}},
        {"family"sv,      lexeme_t{.type = token_type_t::keyword}},
        {"module"sv,      lexeme_t{.type = token_type_t::keyword}},
        {"import"sv,      lexeme_t{.type = token_type_t::keyword}},
        {"else if"sv,     lexeme_t{.type = token_type_t::keyword}},
        {"continue"sv,    lexeme_t{.type = token_type_t::keyword}},
        {"transmute"sv,   lexeme_t{.type = token_type_t::keyword}},
        {"fallthrough"sv, lexeme_t{.type = token_type_t::keyword}},
    };

    ///////////////////////////////////////////////////////////////////////////

    trie_t::trie_t(std::initializer_list<std::pair<std::string_view, lexeme_t>> elements) {
        insert(elements);
    }

    void trie_t::insert(std::string_view key, lexeme_t* value) {
        auto current_node = &_root;

        size_t i = 0;
        const auto key_length = key.length() - 1;
        for (const char c : key) {
            auto& children = current_node->tree->children;
            auto it = children.find(utf8::rune_t(c));
            if (it == std::end(children)) {
                current_node = _node_storage.alloc();
                current_node->data = nullptr;
                current_node->tree = nullptr;
                if (i <= key_length)
                    current_node->tree = _tree_node_storage.alloc();
                children.emplace(c, current_node);
            } else {
                current_node = it->second;
            }
            ++i;
        }

        current_node->data = value;
    }

    trie_node_t* trie_t::find(trie_node_t* node, const utf8::rune_t& rune) {
        auto current_node = node ? node : &_root;
        auto& children = current_node->tree->children;
        auto it = children.find(rune);
        if (it == std::end(children))
            return nullptr;
        return it->second;
    }

    void trie_t::insert(std::initializer_list<std::pair<std::string_view, lexeme_t>> elements) {
        for (const auto& e : elements) {
            auto lexeme = _lexeme_storage.alloc();
            lexeme->type = e.second.type;
            lexeme->tokenizer = e.second.tokenizer;
            insert(e.first, lexeme);
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    lexer_t::lexer_t(
            workspace::session_t& session,
            utf8::source_buffer_t& buffer) : _buffer(buffer),
                                             _session(session) {
    }

    bool lexer_t::make_number_token(
            result_t& r,
            entity_list_t& entities,
            size_t start_pos,
            bool imaginary,
            bool is_signed,
            uint8_t radix,
            number_type_t type,
            std::string_view capture,
            bool check_sign_bit) {
        auto& registry = _session.registry();

        auto token = registry.create();
        registry.assign<token_t>(token, token_type_t::literal, capture);

        auto& number_token = registry.assign<number_token_t>(
            token,
            is_signed,
            imaginary,
            radix,
            type,
            number_size_t::qword);

        if (type == number_type_t::integer) {
            int64_t value;
            auto result = numbers::parse_integer(capture, radix, value);
            if (result != numbers::conversion_result_t::success) {
                errors::add_source_highlighted_error(
                    r,
                    errors::lexer::unable_to_convert_integer_value,
                    _buffer,
                    make_location(start_pos, _buffer.pos()),
                    capture,
                    numbers::conversion_result_to_name(result));
                return false;
            }

            auto narrowed_size = narrow_type(value);
            if (!narrowed_size) {
                errors::add_source_highlighted_error(
                    r,
                    errors::lexer::unable_to_narrow_integer_value,
                    _buffer,
                    make_location(start_pos, _buffer.pos()));
                return false;
            }

            apply_narrowed_value(number_token, *narrowed_size, value, check_sign_bit);
        } else if (type == number_type_t::floating_point) {
            double value;

            if (imaginary)
                capture = std::string_view(capture.data(), capture.length() - 1);

            auto result = numbers::parse_double(capture, value);
            if (result != numbers::conversion_result_t::success) {
                errors::add_source_highlighted_error(
                    r,
                    errors::lexer::unable_to_convert_floating_point_value,
                    _buffer,
                    make_location(start_pos, _buffer.pos()),
                    capture,
                    numbers::conversion_result_to_name(result));
                return false;
            }

            auto narrowed_size = narrow_type(value);
            if (!narrowed_size) {
                errors::add_source_highlighted_error(
                    r,
                    errors::lexer::unable_to_narrow_floating_point_value,
                    _buffer,
                    make_location(start_pos, _buffer.pos()));
                return false;
            }

            apply_narrowed_value(number_token, *narrowed_size, value);
        }

        registry.assign<source_location_t>(
            token,
            make_location(start_pos, _buffer.pos()));
        entities.push_back(token);

        return true;
    }

    bool lexer_t::tokenize_identifier(
            result_t& r,
            entity_list_t& entities,
            token_type_t token_type,
            const entity_maker_t& entity_maker) {
        auto start_pos = _buffer.pos();

        auto rune = _buffer.curr(r);
        if (rune.is_errored())
            return false;

        if (rune != '_' && !rune.is_alpha()) {
            errors::add_source_highlighted_error(
                r,
                errors::lexer::invalid_identifier_start_character,
                _buffer,
                make_location(start_pos, _buffer.pos()),
                rune);
            return false;
        }

        if (!_buffer.move_next(r))
            return false;

        while (true) {
            rune = _buffer.curr(r);
            if (rune.is_errored())
                return false;

            if (!rune.is_digit()
            &&  !rune.is_alpha()
            &&  rune != '_') {
                break;
            }

            if (!_buffer.move_next(r))
                return false;
        }

        auto end_pos = _buffer.pos();
        auto capture = _buffer.make_slice(start_pos, end_pos - start_pos);

        return entity_maker(token_type, start_pos, end_pos, capture) != entt::null;
    }

    bool lexer_t::tokenize(result_t& r, entity_list_t& entities) {
        auto& registry = _session.registry();

        while (!_buffer.eof()) {
            trie_node_t* current_node = nullptr;
            lexeme_t* matched_lexeme = nullptr;

            auto rune = _buffer.curr(r);
            if (rune.is_errored())
                return false;

            if (rune.is_space()) {
                if (!_buffer.move_next(r))
                    break;
                continue;
            }

            _buffer.push_mark();

            while (true) {
                current_node = s_lexemes.find(current_node, rune);
                if (current_node == nullptr) {
                    if (matched_lexeme
                    &&  matched_lexeme->type == token_type_t::keyword) {
                        if (rune.is_alpha() || rune == '_')
                            matched_lexeme = nullptr;
                    }
                    break;
                }

                if (current_node->data != nullptr)
                    matched_lexeme = current_node->data;

                if (!_buffer.move_next(r))
                    return false;

                rune = _buffer.curr(r);
                if (rune.is_errored())
                    return false;
            }

            if (matched_lexeme == nullptr) {
                _buffer.restore_top_mark();
                auto start_pos = _buffer.pop_mark();

                if (!identifier(r, entities)) {
                    errors::add_source_highlighted_error(
                        r,
                        errors::lexer::expected_identifier,
                        _buffer,
                        make_location(start_pos, _buffer.pos()));
                    return false;
                }
            } else {
                if (matched_lexeme->tokenizer) {
                    _buffer.restore_top_mark();
                    _buffer.pop_mark();
                    if (!matched_lexeme->tokenizer(this, r, entities))
                        return false;
                } else {
                    auto start_pos = _buffer.pop_mark();
                    auto end_pos = _buffer.pos();
                    auto token = registry.create();
                    registry.assign<token_t>(
                        token,
                        matched_lexeme->type,
                        _buffer.make_slice(start_pos, end_pos - start_pos));
                    registry.assign<source_location_t>(
                        token,
                        make_location(start_pos, end_pos));
                    entities.push_back(token);
                }
            }
        }

        auto token = registry.create();
        registry.assign<token_t>(token, token_type_t::end_of_input);
        registry.assign<source_location_t>(
            token,
            make_location(_buffer.pos(), _buffer.pos()));
        entities.push_back(token);

        return !r.is_failed();
    }

    bool lexer_t::identifier(result_t& r, entity_list_t& entities) {
        return tokenize_identifier(
            r,
            entities,
            token_type_t::identifier,
            [&](auto token_type, auto start_pos, auto end_pos, auto capture) {
                auto& registry = _session.registry();
                auto token = registry.create();
                registry.assign<token_t>(token, token_type, capture);
                registry.assign<source_location_t>(
                    token,
                    make_location(start_pos, end_pos));
                entities.push_back(token);
                return token;
            });
    }

    bool lexer_t::line_comment(result_t& r, entity_list_t& entities) {
        // prefixed with // or --
        if (!_buffer.move_next(r)) return false;
        if (!_buffer.move_next(r)) return false;
        
        auto start_pos = _buffer.pos();
        while (true) {
            auto rune = _buffer.curr(r);
            if (rune.is_errored())
                return false;
            if (rune == '\n') break;
            if (!_buffer.move_next(r))
                return false;
        }

        auto capture = _buffer.make_slice(
            start_pos,
            _buffer.pos() - start_pos);

        auto& registry = _session.registry();
        auto token = registry.create();
        registry.assign<token_t>(token, token_type_t::comment, capture);
        registry.assign<line_comment_token_t>(token);
        registry.assign<source_location_t>(
            token,
            make_location(start_pos, _buffer.pos()));
        entities.push_back(token);

        return true;
    }

    bool lexer_t::block_comment(result_t& r, entity_list_t& entities) {
        // prefixed with /*
        if (!_buffer.move_next(r)) return false;
        if (!_buffer.move_next(r)) return false;

        auto block_count = 1;
        auto start_pos = _buffer.pos();

        std::stack<size_t> block_starts{};
        block_starts.push(start_pos);

        std::stack<block_comment_token_t*> blocks{};

        block_comment_token_t root{};
        auto* current_block = &root;

        while (true) {
            auto rune = _buffer.curr(r);
            if (rune.is_errored())
                return false;

            if (rune == '/') {
                if (!_buffer.move_next(r)) 
                    return false;
                rune = _buffer.curr(r);

                if (rune == '*') {
                    ++block_count;

                    block_starts.push(_buffer.pos() + 1);
                    blocks.push(current_block);

                    current_block = &current_block->children.emplace_back();
                } else {
                    continue;
                }
            } else if (rune == '*') {
                if (!_buffer.move_next(r)) 
                    return false;
                rune = _buffer.curr(r);

                if (rune == '/') {
                    if (block_count > 0) {
                        --block_count;

                        auto block_start = block_starts.top();
                        block_starts.pop();

                        current_block->capture = _buffer.make_slice(
                            block_start,
                            _buffer.pos() - block_start - 1);

                        if (!blocks.empty()) {
                            current_block = blocks.top();
                            blocks.pop();
                        }
                    }
                    if (block_count == 0) { 
                        if (!_buffer.move_next(r)) 
                            return false;
                        break;
                    }
                } else {
                    continue;
                }
            }

            if (!_buffer.move_next(r))
                return false;
        }

        auto end_pos = _buffer.pos() - 2;
        auto capture = _buffer.make_slice(start_pos, end_pos - start_pos);

        auto& registry = _session.registry();
        auto token = registry.create();
        registry.assign<token_t>(token, token_type_t::comment, capture);
        registry.assign<block_comment_token_t>(token, root.capture, root.children);
        registry.assign<source_location_t>(
            token,
            make_location(start_pos, end_pos));
        entities.push_back(token);

        return true;
    }

    bool lexer_t::string_literal(result_t& r, entity_list_t& entities) {
        // prefixed with "
        if (!_buffer.move_next(r)) return false;
        
        auto should_exit = false;
        auto start_pos = _buffer.pos();
        while (!should_exit) {
            auto rune = _buffer.curr(r);
            if (rune.is_errored())
                return false;
            if (rune == '\"') should_exit = true;
            if (!_buffer.move_next(r))
                return false;
        }

        auto end_pos = _buffer.pos() - 1;
        auto capture = _buffer.make_slice(start_pos, end_pos - start_pos);

        auto& registry = _session.registry();

        auto token = registry.create();
        registry.assign<token_t>(token, token_type_t::literal, capture);
        registry.assign<string_literal_token_t>(token);
        registry.assign<source_location_t>(
            token,
            make_location(start_pos, end_pos));
        entities.push_back(token);

        return true;
    }

    bool lexer_t::dec_number_literal(result_t& r, entity_list_t& entities) {
        bool imaginary{};
        auto type = number_type_t::integer;

        auto rune = _buffer.curr(r);
        bool is_signed = rune == '-';
        if (is_signed) {
            if (!_buffer.move_next(r))
                return false;
        }

        auto start_pos = _buffer.pos();

        if (!scan_dec_digits(r, start_pos, type))
            return false;

        rune = _buffer.curr(r);
        if (rune.is_errored())
            return false;

        if (rune == 'e' || rune == 'E') {
            if (type != number_type_t::floating_point) {
                errors::add_source_highlighted_error(
                    r,
                    errors::lexer::exponent_notation_not_valid_for_integers,
                    _buffer,
                    make_location(start_pos, _buffer.pos()));
                return false;
            }

            if (!_buffer.move_next(r))
                return false;

            rune = _buffer.curr(r);
            if (rune.is_errored())
                return false;

            if (rune == '-' || rune == '+') {
                if (!_buffer.move_next(r))
                    return false;

                rune = _buffer.curr(r);
                if (rune.is_errored())
                    return false;
            }

            if (!scan_dec_digits(r, _buffer.pos(), type))
                return false;

            rune = _buffer.curr(r);
            if (rune.is_errored())
                return false;

            if (rune == 'i') {
                if (!_buffer.move_next(r))
                    return false;
                imaginary = true;
            }
        } else if (rune.is_alpha()) {
            errors::add_source_highlighted_error(
                r,
                errors::lexer::unexpected_letter_after_decimal_number_literal,
                _buffer,
                make_location(start_pos, _buffer.pos()));
            return false;
        }

        auto capture = _buffer.make_slice(
            start_pos,
            _buffer.pos() - start_pos);

        return make_number_token(
                r,
                entities, 
                start_pos,
                imaginary,
                is_signed, 
                10, 
                type, 
                capture, 
                false);
    }

    bool lexer_t::hex_number_literal(result_t& r, entity_list_t& entities) {
        auto start_pos = _buffer.pos();

        auto rune = _buffer.next(r);
        if (rune != '$') {
            errors::add_source_highlighted_error(
                r,
                errors::lexer::expected_hex_literal_prefix,
                _buffer,
                make_location(start_pos, _buffer.pos()));
            return false;
        }

        start_pos = _buffer.pos();
        while (true) {
            rune = _buffer.curr(r);
            if (rune.is_errored())
                return false;
            if (rune == '_') {
                if (!_buffer.move_next(r))
                    return false;
                continue;
            }
            if (!rune.is_xdigit()) {
                if (rune.is_alpha()) {
                    errors::add_source_highlighted_error(
                        r,
                        errors::lexer::unexpected_letter_after_hexadecimal_number_literal,
                        _buffer,
                        make_location(start_pos, _buffer.pos()));
                    return false;
                }
                break;
            }
            if (!_buffer.move_next(r))  return false;
        }

        auto capture = _buffer.make_slice(
            start_pos,
            _buffer.pos() - start_pos);

        return make_number_token(
                r, 
                entities, 
                start_pos,
                false,
                false, 
                16, 
                number_type_t::integer, 
                capture);
    }

    bool lexer_t::octal_number_literal(result_t& r, entity_list_t& entities) {
        auto start_pos = _buffer.pos();

        auto rune = _buffer.next(r);
        if (rune != '@') {
            errors::add_source_highlighted_error(
                r,
                errors::lexer::expected_octal_literal_prefix,
                _buffer,
                make_location(start_pos, _buffer.pos()));
            return false;
        }

        start_pos = _buffer.pos();
        while (true) {
            rune = _buffer.curr(r);
            if (rune.is_errored())
                return false;
            if (rune == '_') {
                if (!_buffer.move_next(r))
                    return false;
                continue;
            }
            if (rune < 0x30 || rune > 0x37) {
                if (rune.is_alpha()) {
                    errors::add_source_highlighted_error(
                        r,
                        errors::lexer::unexpected_letter_after_octal_number_literal,
                        _buffer,
                        make_location(start_pos, _buffer.pos()));
                    return false;
                }
                break;
            }
            if (!_buffer.move_next(r))  return false;
        }

        auto capture = _buffer.make_slice(
            start_pos,
            _buffer.pos() - start_pos);

        return make_number_token(
                r, 
                entities, 
                start_pos,
                false,
                false, 
                8, 
                number_type_t::integer, capture);
    }

    bool lexer_t::binary_number_literal(result_t& r, entity_list_t& entities) {
        auto start_pos = _buffer.pos();

        auto rune = _buffer.next(r);
        if (rune != '%') {
            errors::add_source_highlighted_error(
                r,
                errors::lexer::expected_binary_literal_prefix,
                _buffer,
                make_location(start_pos, _buffer.pos()));
            return false;
        }

        start_pos = _buffer.pos();
        while (true) {
            rune = _buffer.curr(r);
            if (rune.is_errored())
                return false;
            if (rune == '_') {
                if (!_buffer.move_next(r))
                    return false;
                continue;
            }
            if (rune < 0x30 || rune > 0x31) {
                if (rune.is_alpha() || rune.is_digit()) {
                    errors::add_source_highlighted_error(
                        r,
                        errors::lexer::unexpected_letter_after_binary_number_literal,
                        _buffer,
                        make_location(start_pos, _buffer.pos()));
                    return false;
                }
                break;
            }
            if (!_buffer.move_next(r))  return false;
        }

        auto capture = _buffer.make_slice(
            start_pos,
            _buffer.pos() - start_pos);

        return make_number_token(
                r, 
                entities, 
                start_pos,
                false,
                false, 
                2, 
                number_type_t::integer, 
                capture);
    }

    bool lexer_t::block_string_literal(result_t& r, entity_list_t& entities) {
        // prefixed with {{
        if (!_buffer.move_next(r)) return false;
        if (!_buffer.move_next(r)) return false;

        auto start_pos = _buffer.pos();
        while (true) {
            auto rune = _buffer.curr(r);
            if (rune.is_errored())
                return false;
            if (rune == '}') {
                if (!_buffer.move_next(r))
                    return false;

                rune = _buffer.curr(r);
                if (rune.is_errored())
                    return false;

                if (rune != '}') {
                    errors::add_source_highlighted_error(
                        r,
                        errors::lexer::expected_closing_block_literal,
                        _buffer,
                        make_location(start_pos, _buffer.pos()),
                        rune);
                    return false;
                }

                break;
            }
            if (!_buffer.move_next(r))
                return false;
        }

        auto end_pos = _buffer.pos() - 2;
        auto capture = _buffer.make_slice(start_pos, end_pos - start_pos);

        auto& registry = _session.registry();
        auto token = registry.create();
        registry.assign<token_t>(token, token_type_t::literal, capture);
        registry.assign<block_literal_token_t>(token);
        registry.assign<source_location_t>(
            token,
            make_location(start_pos, end_pos));
        entities.push_back(token);

        return true;
    }

    source_location_t lexer_t::make_location(size_t start_pos, size_t end_pos) {
        auto start_line = _buffer.line_by_index(start_pos);
        auto start_column = _buffer.column_by_index(start_pos);

        auto end_line = _buffer.line_by_index(end_pos);
        auto end_column = _buffer.column_by_index(end_pos);

        return {{end_line->line, end_column}, {start_line->line, start_column}};
    }

    bool lexer_t::scan_dec_digits(result_t& r, size_t start_pos, number_type_t& type) {
        while (true) {
            auto rune = _buffer.curr(r);

            if (rune.is_errored())
                return false;

            if (rune == '.') {
                if (type == number_type_t::floating_point) {
                    errors::add_source_highlighted_error(
                        r,
                        errors::lexer::unexpected_decimal_point,
                        _buffer,
                        make_location(start_pos, _buffer.pos()));
                    return false;
                } else {
                    type = number_type_t::floating_point;
                }
            } else if (rune == '_') {
                // N.B. ignore
            } else if (rune < 0x30 || rune > 0x39) {
                break;
            }
            if (!_buffer.move_next(r))   return false;
        }

        return true;
    }

}
