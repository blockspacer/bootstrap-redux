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

#include "lexer.h"

namespace basecode::compiler::lexer {

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

        {"//",          {.type = token_type_t::comment, .tokenizer = &lexer_t::line_comment}},
        {"/*",          {.type = token_type_t::comment, .tokenizer = &lexer_t::block_comment}},

        {";",           {.type = token_type_t::punctuation}},

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
        {"false",       {.type = token_type_t::keyword}},
        {"defer",       {.type = token_type_t::keyword}},
        {"break",       {.type = token_type_t::keyword}},
        {"tuple",       {.type = token_type_t::keyword}},
        {"array",       {.type = token_type_t::keyword}},
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

    lexer_t::lexer_t(utf8::source_buffer_t* buffer) : _buffer(buffer) {
        s_lexemes.burst_threshold(1024);
    }

    bool lexer_t::tokenize(result_t& r) {
        while (!_buffer->eof()) {
            auto rune = _buffer->curr(r);
            if (rune == utf8::rune_invalid)
                break;

            if (rune.is_space()) {
                _buffer->move_next(r);
                continue;
            }

            // XXX: navigating into the trie-map with a full codepoint?

            struct match_t final {
                std::string_view key;
                lexeme_t& value;
            };

            auto start_pos = _buffer->pos();
            auto len = 1;
            while (true) {
                auto slice = _buffer->make_slice(start_pos, len);
                auto range = s_lexemes.equal_prefix_range(slice);

                std::vector<match_t> matches{};
                for (auto it = range.first; it != range.second; it++)
                    matches.push_back(match_t{it.key(), it.value()});

                if (matches.empty()) {
                    if (!identifier(r)) {
                        r.error("X000", "expected identifier");
                        return false;
                    }
                    break;
                }

                if (matches.size() == 1
                &&  matches[0].key.length() == len) {
                    auto& match = matches[0];
                    if (match.value.tokenizer) {
                        if (!match.value.tokenizer(this, r))
                            return false;

                        // XXX: do something clever
                    } else {
                        // XXX: create entity + components
                        fmt::print("match.key = {}\n", match.key);
                    }
                    break;
                }

                ++len;

                if (start_pos + len >= _buffer->length() - 1) {
                    r.error("X000", "unexpected end of input");
                    return false;
                }
            }

            if (!_buffer->move_next(r))
                return false;
        }

        // XXX: create entity & components for end_of_file
        return !r.is_failed();
    }

    bool lexer_t::identifier(result_t& r) {
        return false;
    }

    bool lexer_t::rune_literal(result_t& r) {
        return false;
    }

    bool lexer_t::line_comment(result_t& r) {
        return false;
    }

    bool lexer_t::block_comment(result_t& r) {
        return false;
    }

    bool lexer_t::string_literal(result_t& r) {
        return false;
    }

    bool lexer_t::directive_literal(result_t& r) {
        return false;
    }

    bool lexer_t::annotation_literal(result_t& r) {
        return false;
    }

    bool lexer_t::dec_number_literal(result_t& r) {
        auto type = number_type_t::integer;
        auto rune = _buffer->curr(r);
        bool is_signed = rune == '-';
        if (is_signed) {
            if (!_buffer->move_next(r))
                return false;
        }

        auto start_pos = _buffer->pos();
        while (true) {
            auto ch = static_cast<int32_t>(_buffer->curr(r));
            if (ch == '.') {
                if (type == number_type_t::floating_point) {
                    r.error("X000", "unexpected decimal point");
                    return false;
                } else {
                    type = number_type_t::floating_point;
                }
            }
            if (ch == '_' || ch == 'E' || ch == 'e') {
                if (!_buffer->move_next(r))
                    return false;
                continue;
            }
            if (ch < 0x30 || ch > 0x39)     break;
            if (!_buffer->move_next(r))  return false;
        }

        auto capture = _buffer->make_slice(
            start_pos,
            _buffer->pos() - start_pos);

        return true;
    }

    bool lexer_t::hex_number_literal(result_t& r) {
        auto rune = _buffer->next(r);
        if (rune != '$') {
            r.error("X000", "expected hex prefix: $");
            return false;
        }

        auto start_pos = _buffer->pos();
        while (true) {
            rune = _buffer->curr(r);
            if (rune == '_') {
                if (!_buffer->move_next(r))
                    return false;
                continue;
            }
            if (!rune.is_xdigit())          break;
            if (!_buffer->move_next(r))  return false;
        }

        auto capture = _buffer->make_slice(
            start_pos,
            _buffer->pos() - start_pos);

        return true;
    }

    bool lexer_t::octal_number_literal(result_t& r) {
        auto rune = _buffer->next(r);
        if (rune != '@') {
            r.error("X000", "expected octal prefix: @");
            return false;
        }

        auto start_pos = _buffer->pos();
        while (true) {
            auto ch = static_cast<int32_t>(_buffer->curr(r));
            if (ch == '_') {
                if (!_buffer->move_next(r))
                    return false;
                continue;
            }
            if (ch < 0x30 || ch > 0x37)     break;
            if (!_buffer->move_next(r))  return false;
        }

        auto capture = _buffer->make_slice(
            start_pos,
            _buffer->pos() - start_pos);

        return true;
    }

    bool lexer_t::binary_number_literal(result_t& r) {
        auto rune = _buffer->next(r);
        if (rune != '%') {
            r.error("X000", "expected binary prefix: %");
            return false;
        }

        auto start_pos = _buffer->pos();
        while (true) {
            auto ch = static_cast<int32_t>(_buffer->curr(r));
            if (ch == '_') {
                if (!_buffer->move_next(r))
                    return false;
                continue;
            }
            if (ch < 0x30 || ch > 0x31)     break;
            if (!_buffer->move_next(r))  return false;
        }

        auto capture = _buffer->make_slice(
            start_pos,
            _buffer->pos() - start_pos);

        return true;
    }

}