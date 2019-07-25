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

namespace basecode::compiler::lexer {

    enum class token_type_t {
        invalid,
        at,
        bang,
        plus,
        pipe,
        minus,
        caret,
        comma,
        colon,
        equal,
        tilde,
        quote,
        period,
        dollar,
        percent,
        keyword,
        asterisk,
        question,
        ampersand,
        less_than,
        back_tick,
        underscore,
        octothorpe,
        back_slash,
        semi_colon,
        left_paren,
        apostrophe,
        left_brace,
        right_brace,
        right_paren,
        left_bracket,
        greater_than,
        forward_slash,
        right_bracket,
        number_literal,
    };

    struct token_t final {
        token_type_t type{};
        std::string_view value{};
    };

    enum class number_type_t {
        none,
        integer,
        arbitrary,
        floating_point
    };

    struct number_token_t final {
        bool is_signed{};
        uint8_t radix = 10;
        number_type_t type{};
    };

}