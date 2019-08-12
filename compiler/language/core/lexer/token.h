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
#include <compiler/data/array.h>

namespace basecode::compiler::language::core::lexer {

    using namespace std::literals;

    enum class token_type_t {
        literal,
        comment,
        keyword,
        directive,
        operator_,
        identifier,
        annotation,
        punctuation,
        end_of_input,
    };

    static inline std::string_view token_type_to_name(token_type_t type) {
        switch (type) {
            case token_type_t::literal:         return "literal"sv;
            case token_type_t::comment:         return "comment"sv;
            case token_type_t::keyword:         return "keyword"sv;
            case token_type_t::directive:       return "directive"sv;
            case token_type_t::operator_:       return "operator"sv;
            case token_type_t::identifier:      return "identifier"sv;
            case token_type_t::annotation:      return "annotation"sv;
            case token_type_t::punctuation:     return "punctuation"sv;
            case token_type_t::end_of_input:    return "end_of_input"sv;
        }
    }

    struct token_t final {
        token_type_t type{};
        std::string_view value{};
    };

    ///////////////////////////////////////////////////////////////////////////
    
    enum class number_type_t {
        none,
        integer,
        arbitrary,
        floating_point
    };

    static inline std::string_view number_type_to_name(number_type_t type) {
        switch (type) {
            case number_type_t::none:           return "none"sv;
            case number_type_t::integer:        return "integer"sv;
            case number_type_t::arbitrary:      return "arbitrary"sv;
            case number_type_t::floating_point: return "floating_point"sv;
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    enum class number_size_t {
        byte,
        word,
        dword,
        qword
    };

    static inline std::string_view number_size_to_name(number_size_t size) {
        switch (size) {
            case number_size_t::byte:  return "byte"sv;
            case number_size_t::word:  return "word"sv;
            case number_size_t::dword: return "dword"sv;
            case number_size_t::qword: return "qword"sv;
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    struct number_token_t final {
        bool is_signed{};
        bool imaginary{};
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

    void apply_narrowed_value(
            number_token_t& token, 
            number_size_t size, 
            double value);

    void apply_narrowed_value(
            number_token_t& token, 
            number_size_t size, 
            int64_t value, 
            bool check_sign_bit = true);

    std::optional<number_size_t> narrow_type(double value);

    std::optional<number_size_t> narrow_type(int64_t value);

    ///////////////////////////////////////////////////////////////////////////

    struct line_comment_token_t final {};

    struct block_comment_token_t final {
        explicit block_comment_token_t(memory::allocator_t* allocator) : children(allocator) {}
        std::string_view capture{};
        data::array_t<block_comment_token_t> children;
    };

    ///////////////////////////////////////////////////////////////////////////

    struct block_literal_token_t final {};

    struct string_literal_token_t final {};

}
