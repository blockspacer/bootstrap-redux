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

    using namespace std::literals;

    enum class token_type_t {
        literal,
        comment,
        keyword,
        operator_,
        identifier,
        punctuation,
        end_of_input,
    };

    static inline std::string_view token_type_to_name(token_type_t type) {
        switch (type) {
            case token_type_t::literal: return "literal"sv;
            case token_type_t::comment: return "comment"sv;
            case token_type_t::keyword: return "keyword"sv;
            case token_type_t::operator_: return "operator"sv;
            case token_type_t::identifier: return "identifier"sv;
            case token_type_t::punctuation: return "punctuation"sv;
            case token_type_t::end_of_input: return "end_of_input"sv;
        }
    }

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

    enum class number_size_t {
        byte,
        word,
        dword,
        qword
    };

    static inline std::string_view number_type_to_name(number_type_t type) {
        switch (type) {
            case number_type_t::none:           return "none"sv;
            case number_type_t::integer:        return "integer"sv;
            case number_type_t::arbitrary:      return "arbitrary"sv;
            case number_type_t::floating_point: return "floating_point";
        }
    }

    struct number_token_t final {
        bool is_signed{};
        uint8_t radix = 10;
        number_type_t type{};
        number_size_t size{};
        union {
            float f32;
            double f64;
            uint8_t u8;
            uint16_t u16;
            uint32_t u32;
            uint64_t u64;
        } value{.u64 = 0};
    };

    std::optional<number_size_t> narrow_type(double value);

    std::optional<number_size_t> narrow_type(int64_t value);

    void apply_narrowed_value(number_token_t& token, number_size_t size, double value);

    void apply_narrowed_value(number_token_t& token, number_size_t size, int64_t value, bool check_sign_bit = true);

}
