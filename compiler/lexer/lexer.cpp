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

#include <compiler/defer.h>
#include <compiler/numbers/bytes.h>
#include <compiler/numbers/parse.h>
#include <compiler/formatters/formatters.h>
#include "lexer.h"

namespace basecode::compiler::lexer {

    struct match_t final {
        lexeme_t value;
        bool candidate{};
        std::string_view key;
    };

    tsl::htrie_map<char, lexeme_t> lexer_t::s_lexemes {
        {"1",           {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"2",           {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"3",           {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"4",           {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"5",           {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"6",           {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"7",           {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"8",           {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"9",           {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},

        {"-1",          {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-2",          {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-3",          {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-4",          {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-5",          {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-6",          {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-7",          {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-8",          {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},
        {"-9",          {.type = token_type_t::literal, .tokenizer = &lexer_t::dec_number_literal}},

        {"$0",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$1",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$2",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$3",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$4",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$5",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$6",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$7",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$8",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$9",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$a",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$b",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$c",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$d",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$e",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},
        {"$f",          {.type = token_type_t::literal, .tokenizer = &lexer_t::hex_number_literal}},

        {"%0",          {.type = token_type_t::literal, .tokenizer = &lexer_t::binary_number_literal}},
        {"%1",          {.type = token_type_t::literal, .tokenizer = &lexer_t::binary_number_literal}},

        {"@0",          {.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@1",          {.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@2",          {.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@3",          {.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@4",          {.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@5",          {.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@6",          {.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},
        {"@7",          {.type = token_type_t::literal, .tokenizer = &lexer_t::octal_number_literal}},

        {"@a",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@b",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@c",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@d",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@e",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@f",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@g",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@h",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@i",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@j",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@k",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@l",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@m",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@n",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@o",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@p",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@q",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@r",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@s",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@t",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@u",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@v",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@w",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@x",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@y",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},
        {"@z",          {.type = token_type_t::literal, .tokenizer = &lexer_t::annotation_literal}},

        {"#a",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#b",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#c",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#d",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#e",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#f",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#g",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#h",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#i",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#j",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#k",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#l",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#m",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#n",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#o",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#p",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#q",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#r",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#s",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#t",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#u",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#v",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#w",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#x",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#y",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},
        {"#z",          {.type = token_type_t::literal, .tokenizer = &lexer_t::directive_literal}},

        {"'",           {.type = token_type_t::literal, .tokenizer = &lexer_t::rune_literal}},
        {"\"",          {.type = token_type_t::literal, .tokenizer = &lexer_t::string_literal}},
        {"{{",          {.type = token_type_t::literal, .tokenizer = &lexer_t::block_string_literal}},

        {"//",          {.type = token_type_t::comment, .tokenizer = &lexer_t::line_comment}},
        {"--",          {.type = token_type_t::comment, .tokenizer = &lexer_t::line_comment}},
        {"/*",          {.type = token_type_t::comment, .tokenizer = &lexer_t::block_comment}},

        {";",           {.type = token_type_t::punctuation}},
        {",",           {.type = token_type_t::punctuation}},
        {"{",           {.type = token_type_t::punctuation}},
        {"}",           {.type = token_type_t::punctuation}},
        {"(",           {.type = token_type_t::punctuation}},
        {")",           {.type = token_type_t::punctuation}},
        {"[",           {.type = token_type_t::punctuation}},
        {"]",           {.type = token_type_t::punctuation}},
        {"\\",          {.type = token_type_t::punctuation}},

        {"&",           {.type = token_type_t::operator_}},
        {"|",           {.type = token_type_t::operator_}},
        {"~",           {.type = token_type_t::operator_}},
        {"**",          {.type = token_type_t::operator_}},
        {"+",           {.type = token_type_t::operator_}},
        {"-",           {.type = token_type_t::operator_}},
        {"*",           {.type = token_type_t::operator_}},
        {"/",           {.type = token_type_t::operator_}},
        {"%",           {.type = token_type_t::operator_}},

        {"&&",          {.type = token_type_t::operator_}},
        {"||",          {.type = token_type_t::operator_}},
        {">",           {.type = token_type_t::operator_}},
        {"<",           {.type = token_type_t::operator_}},
        {">=",          {.type = token_type_t::operator_}},
        {"<=",          {.type = token_type_t::operator_}},
        {"==",          {.type = token_type_t::operator_}},
        {"!=",          {.type = token_type_t::operator_}},

        {":=",          {.type = token_type_t::operator_}},
        {"%:=",         {.type = token_type_t::operator_}},
        {"/:=",         {.type = token_type_t::operator_}},
        {"*:=",         {.type = token_type_t::operator_}},
        {"-:=",         {.type = token_type_t::operator_}},
        {"+:=",         {.type = token_type_t::operator_}},
        {"&:=",         {.type = token_type_t::operator_}},
        {"|:=",         {.type = token_type_t::operator_}},
        {"~:=",         {.type = token_type_t::operator_}},

        {":",           {.type = token_type_t::operator_}}, // type operator
        {"::",          {.type = token_type_t::operator_}}, // bind operator
        {"=>",          {.type = token_type_t::operator_}}, // association operator
        {"->",          {.type = token_type_t::operator_}}, // placeholder

        {"in",          {.type = token_type_t::operator_}},
        {"xor",         {.type = token_type_t::operator_}},
        {"shl",         {.type = token_type_t::operator_}},
        {"shr",         {.type = token_type_t::operator_}},
        {"rol",         {.type = token_type_t::operator_}},
        {"ror",         {.type = token_type_t::operator_}},

        {"if",          {.type = token_type_t::keyword}},
        {"ns",          {.type = token_type_t::keyword}},
        {"for",         {.type = token_type_t::keyword}},
        {"nil",         {.type = token_type_t::keyword}},
        {"use",         {.type = token_type_t::keyword}},
        {"true",        {.type = token_type_t::keyword}},
        {"cast",        {.type = token_type_t::keyword}},
        {"case",        {.type = token_type_t::keyword}},
        {"proc",        {.type = token_type_t::keyword}},
        {"enum",        {.type = token_type_t::keyword}},
        {"else",        {.type = token_type_t::keyword}},
        {"with",        {.type = token_type_t::keyword}},
        {"goto",        {.type = token_type_t::keyword}},
        {"false",       {.type = token_type_t::keyword}},
        {"defer",       {.type = token_type_t::keyword}},
        {"break",       {.type = token_type_t::keyword}},
        {"union",       {.type = token_type_t::keyword}},
        {"yield",       {.type = token_type_t::keyword}},
        {"struct",      {.type = token_type_t::keyword}},
        {"return",      {.type = token_type_t::keyword}},
        {"switch",      {.type = token_type_t::keyword}},
        {"family",      {.type = token_type_t::keyword}},
        {"module",      {.type = token_type_t::keyword}},
        {"import",      {.type = token_type_t::keyword}},
        {"else if",     {.type = token_type_t::keyword}},
        {"continue",    {.type = token_type_t::keyword}},
        {"transmute",   {.type = token_type_t::keyword}},
        {"fallthrough", {.type = token_type_t::keyword}},
    };

    ///////////////////////////////////////////////////////////////////////////

    lexer_t::lexer_t(
            workspace_t& workspace,
            utf8::source_buffer_t& buffer) : _workspace(workspace),
                                             _buffer(buffer) {
        s_lexemes.burst_threshold(1024);
    }

    bool lexer_t::make_number_token(
            result_t& r,
            entity_list_t& entities,
            size_t start_pos,
            bool is_signed,
            uint8_t radix,
            number_type_t type,
            std::string_view capture,
            bool check_sign_bit) {
        auto token = _workspace.registry.create();
        _workspace.registry.assign<token_t>(token, token_type_t::literal, capture);

        auto& number_token = _workspace.registry.assign<number_token_t>(
            token,
            is_signed,
            radix,
            type,
            number_size_t::qword);

        if (type == number_type_t::integer) {
            int64_t value;
            auto result = numbers::parse_integer(capture, radix, value);
            if (result != numbers::conversion_result_t::success) {
                r.error(
                    "X000",
                    fmt::format(
                        "unable to convert integer value {} because {}",
                        capture,
                        numbers::conversion_result_to_name(result)));
                return false;
            }

            auto narrowed_size = narrow_type(value);
            if (!narrowed_size) {
                r.error(
                    "X000",
                    "unable to narrow integer value");
                return false;
            }

            apply_narrowed_value(number_token, *narrowed_size, value, check_sign_bit);
        } else if (type == number_type_t::floating_point) {
            double value;
            auto result = numbers::parse_double(capture, value);
            if (result != numbers::conversion_result_t::success) {
                r.error(
                    "X000",
                    fmt::format(
                        "unable to convert floating point value {} because {}",
                        capture,
                        numbers::conversion_result_to_name(result)));
                return false;
            }

            auto narrowed_size = narrow_type(value);
            if (!narrowed_size) {
                r.error(
                    "X000",
                    "unable to narrow floating point value");
                return false;
            }

            apply_narrowed_value(number_token, *narrowed_size, value);
        }

        _workspace.registry.assign<source_location_t>(
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
        if (rune.is_eof_or_invalid())
            return false;

        if (rune != '_' && !rune.is_alpha()) {
            r.error(
                "X000",
                fmt::format(
                    "identifiers must start with _ or a letter; found: {}",
                    rune));
            return false;
        }

        if (!_buffer.move_next(r))
            return false;

        while (true) {
            rune = _buffer.curr(r);
            if (rune.is_eof_or_invalid())
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
        while (!_buffer.eof()) {
            auto rune = _buffer.curr(r);
            if (rune.is_eof_or_invalid())
                return false;

            if (rune.is_space()) {
                if (!_buffer.move_next(r))
                    break;
                continue;
            }

            auto len = 1;
            std::vector<match_t> matches{};

            while (true) {
                auto slice = _buffer.make_slice(_buffer.pos(), len);
                auto range = s_lexemes.equal_prefix_range(slice);

                auto start_size = matches.size();

                for (auto it = range.first; it != range.second; it++) {
                    matches.push_back(match_t{
                            .key = it.key(), 
                            .value = it.value(),
                            .candidate = it.key().length() == len,
                    });
                }

                if (matches.size() > start_size) {
                    for (auto it = matches.begin(); it != matches.end();) {
                        auto& match = *it;
                        if (match.candidate && match.key.length() < len) {
                            it = matches.erase(it);
                        } else {
                            ++it;
                        }
                    }
                }

                if (matches.empty()) {
                    if (!identifier(r, entities)) {
                        r.error("X000", "expected identifier");
                        return false;
                    }
                    break;
                }

                if (matches.size() == 1
                &&  matches[0].candidate) {
                    auto& match = matches[0];

                    if (match.value.tokenizer) {
                        if (!match.value.tokenizer(this, r, entities))
                            return false;
                    } else {
                        token_type_t type = match.value.type;

                        if (match.value.type == token_type_t::keyword) {
                            auto check_slice = _buffer.make_slice(
                                    _buffer.pos(), 
                                    slice.length() + 1);
                            const auto& last_char = check_slice[slice.length()];
                            if (isalpha(last_char) || last_char == '_') {
                                goto next_round;
                            }

                            type = token_type_t::identifier;
                        }

                        auto start_pos = _buffer.pos();
                        _buffer.seek(_buffer.pos() + len);
                        auto token = _workspace.registry.create();
                        _workspace.registry.assign<token_t>(token, type, slice);
                        _workspace.registry.assign<source_location_t>(
                            token,
                            make_location(start_pos, _buffer.pos()));
                        entities.push_back(token);
                    }

                    break;
                }
            
            next_round:
                len += _buffer.width();

                if (_buffer.pos() + len >= _buffer.length() - 1) {
                    r.error("X000", "unexpected end of input");
                    return false;
                }

                for (auto it = matches.begin(); it != matches.end();) {
                    auto& match = *it;
                    if (!match.candidate) {
                        it = matches.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
        }

        auto token = _workspace.registry.create();
        _workspace.registry.assign<token_t>(token, token_type_t::end_of_input);
        _workspace.registry.assign<source_location_t>(
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
                auto token = _workspace.registry.create();
                _workspace.registry.assign<token_t>(token, token_type, capture);
                _workspace.registry.assign<source_location_t>(
                    token,
                    make_location(start_pos, end_pos));
                entities.push_back(token);
                return token;
            });
    }

    bool lexer_t::rune_literal(result_t& r, entity_list_t& entities) {
        // prefixed with '
        if (!_buffer.move_next(r)) return false;
        
        auto start_pos = _buffer.pos();

        auto rune = _buffer.curr(r);
        if (rune.is_eof_or_invalid())
            return false;

        if (rune == '\\') {
            if (!_buffer.move_next(r)) return false;
            rune = _buffer.curr(r);
            if (rune.is_eof_or_invalid())
                return false;

            auto extra_chars = 0;
            
            switch ((int32_t)rune) {
                case 'a': 
                case 'b':
                case 'e': 
                case 'n':
                case 'r':
                case 't':
                case 'v':
                case '\\':
                case '\'':
                    if (!_buffer.move_next(r))
                        return false;
                    break;
                case 'x':
                    if (!_buffer.move_next(r))
                        return false;
                    extra_chars = 2;
                    break;
                case 'u': 
                    if (!_buffer.move_next(r))
                        return false;
                    extra_chars = 4;
                    break;
                case 'U':  
                    if (!_buffer.move_next(r))
                        return false;
                    extra_chars = 8;
                    break;
                default:  
                    extra_chars = 3;
                    break;
            }

            for (size_t i = 0; i < extra_chars; i++)
                if (!_buffer.move_next(r))
                    return false;
        } else {
            if (!_buffer.move_next(r)) return false;
        }

        rune = _buffer.curr(r);
        if (rune.is_eof_or_invalid())
            return false;

        if (rune != '\'') {
            r.error(
                "X000",
                fmt::format(
                    "expected closing ' but found: {}",
                    rune));
            return false;
        }

        if (!_buffer.move_next(r))
            return false;

        auto end_pos = _buffer.pos() - 1;
        auto capture = _buffer.make_slice(start_pos, end_pos - start_pos);

        auto token = _workspace.registry.create();
        _workspace.registry.assign<token_t>(token, token_type_t::literal, capture);
        _workspace.registry.assign<rune_literal_token_t>(token);
        _workspace.registry.assign<source_location_t>(
            token,
            make_location(start_pos, end_pos));
        entities.push_back(token);

        return true;
    }

    bool lexer_t::line_comment(result_t& r, entity_list_t& entities) {
        // prefixed with // or --
        if (!_buffer.move_next(r)) return false;
        if (!_buffer.move_next(r)) return false;
        
        auto start_pos = _buffer.pos();
        while (true) {
            auto rune = _buffer.curr(r);
            if (rune.is_eof_or_invalid())
                return false;
            if (rune == '\n') break;
            if (!_buffer.move_next(r))
                return false;
        }

        auto capture = _buffer.make_slice(
            start_pos,
            _buffer.pos() - start_pos);

        auto token = _workspace.registry.create();
        _workspace.registry.assign<token_t>(token, token_type_t::comment, capture);
        _workspace.registry.assign<line_comment_token_t>(token);
        _workspace.registry.assign<source_location_t>(
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
            if (rune.is_eof_or_invalid()) 
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

         auto token = _workspace.registry.create();
        _workspace.registry.assign<token_t>(token, token_type_t::comment, capture);
        _workspace.registry.assign<block_comment_token_t>(token, root.capture, root.children);
        _workspace.registry.assign<source_location_t>(
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
            if (rune.is_eof_or_invalid())
                return false;
            if (rune == '\"') should_exit = true;
            if (!_buffer.move_next(r))
                return false;
        }

        auto end_pos = _buffer.pos() - 1;
        auto capture = _buffer.make_slice(start_pos, end_pos - start_pos);

        auto token = _workspace.registry.create();
        _workspace.registry.assign<token_t>(token, token_type_t::literal, capture);
        _workspace.registry.assign<string_literal_token_t>(token);
        _workspace.registry.assign<source_location_t>(
            token,
            make_location(start_pos, end_pos));
        entities.push_back(token);

        return true;
    }

    bool lexer_t::directive_literal(result_t& r, entity_list_t& entities) {
        auto rune = _buffer.curr(r);
        if (rune.is_eof_or_invalid())
            return false;

        if (rune != '#') {
            r.error(
                "X000",
                "expected directive prefix: #");
            return false;
        }

        if (!_buffer.move_next(r))
            return false;

        return tokenize_identifier(
            r,
            entities,
            token_type_t::directive,
            [&](auto token_type, auto start_pos, auto end_pos, auto capture) {
                auto token = _workspace.registry.create();
                _workspace.registry.assign<token_t>(token, token_type, capture);
                _workspace.registry.assign<source_location_t>(
                    token,
                    make_location(start_pos, end_pos));
                entities.push_back(token);
                return token;
            });
    }

    bool lexer_t::annotation_literal(result_t& r, entity_list_t& entities) {
        auto rune = _buffer.curr(r);
        if (rune.is_eof_or_invalid())
            return false;

        if (rune != '@') {
            r.error(
                "X000",
                "expected annotation prefix: @");
            return false;
        }

        if (!_buffer.move_next(r))
            return false;

        return tokenize_identifier(
            r,
            entities,
            token_type_t::annotation,
            [&](auto token_type, auto start_pos, auto end_pos, auto capture) {
                auto token = _workspace.registry.create();
                _workspace.registry.assign<token_t>(token, token_type, capture);
                _workspace.registry.assign<source_location_t>(
                    token,
                    make_location(start_pos, end_pos));
                entities.push_back(token);
                return token;
            });
    }

    bool lexer_t::dec_number_literal(result_t& r, entity_list_t& entities) {
        auto type = number_type_t::integer;
        auto rune = _buffer.curr(r);
        bool is_signed = rune == '-';
        if (is_signed) {
            if (!_buffer.move_next(r))
                return false;
        }

        auto start_pos = _buffer.pos();
        while (true) {
            rune = _buffer.curr(r);
            if (rune.is_eof_or_invalid())
                return false;
            auto ch = static_cast<int32_t>(rune);
            if (ch == '.') {
                if (type == number_type_t::floating_point) {
                    r.error(
                        "X000",
                        "unexpected decimal point");
                    return false;
                } else {
                    type = number_type_t::floating_point;
                }
            }
            if (ch == '_' || ch == 'E' || ch == 'e') {
                if (!_buffer.move_next(r))
                    return false;
                continue;
            }
            if (ch < 0x30 || ch > 0x39) {
                if (isalpha(ch)) {
                    r.error(
                        "X000",
                        "unexpected letter immediately after decimal number");
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
                is_signed, 
                10, 
                type, 
                capture, 
                false);
    }

    bool lexer_t::hex_number_literal(result_t& r, entity_list_t& entities) {
        auto rune = _buffer.next(r);
        if (rune != '$') {
            r.error(
                "X000",
                "expected hex prefix: $");
            return false;
        }

        auto start_pos = _buffer.pos();
        while (true) {
            rune = _buffer.curr(r);
            if (rune.is_eof_or_invalid())
                return false;
            if (rune == '_') {
                if (!_buffer.move_next(r))
                    return false;
                continue;
            }
            if (!rune.is_xdigit()) {
                if (rune.is_alpha()) {
                    r.error(
                        "X000",
                        "unexpected letter immediately after hexadecimal number");
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
                16, 
                number_type_t::integer, 
                capture);
    }

    bool lexer_t::octal_number_literal(result_t& r, entity_list_t& entities) {
        auto rune = _buffer.next(r);
        if (rune != '@') {
            r.error(
                "X000",
                "expected octal prefix: @");
            return false;
        }

        auto start_pos = _buffer.pos();
        while (true) {
            rune = _buffer.curr(r);
            if (rune.is_eof_or_invalid())
                return false;
            auto ch = static_cast<int32_t>(rune);
            if (ch == '_') {
                if (!_buffer.move_next(r))
                    return false;
                continue;
            }
            if (ch < 0x30 || ch > 0x37) {
                if (isalpha(ch)) {
                    r.error(
                        "X000",
                        "unexpected letter immediately after octal number");
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
                8, 
                number_type_t::integer, capture);
    }

    bool lexer_t::binary_number_literal(result_t& r, entity_list_t& entities) {
        auto rune = _buffer.next(r);
        if (rune != '%') {
            r.error(
                "X000",
                "expected binary prefix: %");
            return false;
        }

        auto start_pos = _buffer.pos();
        while (true) {
            rune = _buffer.curr(r);
            if (rune.is_eof_or_invalid())
                return false;
            auto ch = static_cast<int32_t>(rune);
            if (ch == '_') {
                if (!_buffer.move_next(r))
                    return false;
                continue;
            }
            if (ch < 0x30 || ch > 0x31) {
                if (isalpha(ch) || isdigit(ch)) {
                    r.error(
                        "X000",
                        "unexpected letter or non-binary digit immediately after decimal number");
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
            if (rune.is_eof_or_invalid())
                return false;
            if (rune == '}') {
                if (!_buffer.move_next(r))
                    return false;

                rune = _buffer.curr(r);
                if (rune.is_eof_or_invalid())
                    return false;

                if (rune != '}') {
                    r.error(
                            "X000", 
                            fmt::format("expected }} but found: {}", rune));
                    return false;
                }

                break;
            }
            if (!_buffer.move_next(r))
                return false;
        }

        auto end_pos = _buffer.pos() - 2;
        auto capture = _buffer.make_slice(start_pos, end_pos - start_pos);

        auto token = _workspace.registry.create();
        _workspace.registry.assign<token_t>(token, token_type_t::literal, capture);
        _workspace.registry.assign<block_literal_token_t>(token);
        _workspace.registry.assign<source_location_t>(
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

}
