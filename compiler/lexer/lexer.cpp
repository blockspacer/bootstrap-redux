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

#include <tsl/htrie_map.h>
#include "token.h"
#include "lexer.h"

namespace basecode::compiler::lexer {

    // XXX: add new flag for non-conditional prefixes to lexeme_t
    tsl::htrie_map<char, lexeme_t> lexer_t::s_lexemes {
        {"@",           {.prefix = prefix_type_t::conditional, .type = token_type_t::at}},
        {"!",           {.type = token_type_t::bang}},
        {"+",           {.type = token_type_t::plus}},
        {"|",           {.type = token_type_t::pipe}},
        {"-",           {.type = token_type_t::minus}},
        {"^",           {.type = token_type_t::caret}},
        {",",           {.type = token_type_t::comma}},
        {":",           {.type = token_type_t::colon}},
        {"=",           {.type = token_type_t::equal}},
        {"~",           {.type = token_type_t::tilde}},
        {"\"",          {.prefix = prefix_type_t::non_conditional, .type = token_type_t::quote}},
        {".",           {.type = token_type_t::period}},
        {"$",           {.prefix = prefix_type_t::conditional, .type = token_type_t::dollar}},
        {"%",           {.prefix = prefix_type_t::conditional, .type = token_type_t::percent}},
        {"*",           {.type = token_type_t::asterisk}},
        {"?",           {.type = token_type_t::question}},
        {"&",           {.type = token_type_t::ampersand}},
        {"<",           {.type = token_type_t::less_than}},
        {"`",           {.type = token_type_t::back_tick}},
        {"_",           {.type = token_type_t::underscore}},
        {"#",           {.type = token_type_t::octothorpe}},
        {"\\",          {.type = token_type_t::back_slash}},
        {";",           {.type = token_type_t::semi_colon}},
        {"(",           {.type = token_type_t::left_paren}},
        {"'",           {.prefix = prefix_type_t::non_conditional, .type = token_type_t::apostrophe}},
        {"{",           {.type = token_type_t::left_brace}},
        {"}",           {.type = token_type_t::right_brace}},
        {")",           {.type = token_type_t::right_paren}},
        {"[",           {.type = token_type_t::left_bracket}},
        {"]",           {.type = token_type_t::right_bracket}},
        {">",           {.type = token_type_t::greater_than}},
        {"/",           {.type = token_type_t::forward_slash}},

        {"if",          {.type = token_type_t::keyword}},
        {"ns",          {.type = token_type_t::keyword}},
        {"xor",         {.type = token_type_t::keyword}},
        {"shl",         {.type = token_type_t::keyword}},
        {"shr",         {.type = token_type_t::keyword}},
        {"rol",         {.type = token_type_t::keyword}},
        {"ror",         {.type = token_type_t::keyword}},
        {"for",         {.type = token_type_t::keyword}},
        {"nil",         {.type = token_type_t::keyword}},
        {"use",         {.type = token_type_t::keyword}},
        {"true",        {.type = token_type_t::keyword}},
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
        {"fallthrough", {.type = token_type_t::keyword}},
    };

    std::array<lexeme_tokenizer_t, 3> lexer_t::s_custom_tokenizers {
        std::bind(&lexer_t::tokenize_binary_number, std::placeholders::_1, std::placeholders::_2),
        std::bind(&lexer_t::tokenize_octal_number, std::placeholders::_1, std::placeholders::_2),
        std::bind(&lexer_t::tokenize_hex_number, std::placeholders::_1, std::placeholders::_2)
    };

    tsl::htrie_map<char, uint8_t> lexer_t::s_custom_lexemes {
        {"%0",  0},
        {"%1",  0},

        {"@0",  1},
        {"@1",  1},
        {"@2",  1},
        {"@3",  1},
        {"@4",  1},
        {"@5",  1},
        {"@6",  1},
        {"@7",  1},

        {"$0",  2},
        {"$1",  2},
        {"$2",  2},
        {"$3",  2},
        {"$4",  2},
        {"$5",  2},
        {"$6",  2},
        {"$7",  2},
        {"$a",  2},
        {"$b",  2},
        {"$c",  2},
        {"$d",  2},
        {"$e",  2},
        {"$f",  2},
    };

    ///////////////////////////////////////////////////////////////////////////

    lexer_t::lexer_t(utf8::source_buffer_t* buffer) : _buffer(buffer) {
    }

    bool lexer_t::tokenize(result_t&) {
        return false;
    }

    bool lexer_t::tokenize_hex_number(result_t& r) {
        return false;
    }

    bool lexer_t::tokenize_octal_number(result_t& r) {
        return false;
    }

    bool lexer_t::tokenize_binary_number(result_t& r) {
        return false;
    }

}