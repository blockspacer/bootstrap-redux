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

#include <optional>
#include <basecode/types.h>

namespace basecode::language {

    using namespace std::literals;

    template <typename T>
    struct basic_token_t final {
        T type{};
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

    struct block_comment_token_t final {
        explicit block_comment_token_t(memory::allocator_t* allocator) : children(allocator) {}
        std::string_view capture{};
        adt::array_t<block_comment_token_t> children;
    };

}