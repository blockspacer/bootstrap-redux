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

#include <locale>
#include <fmt/format.h>
#include <compiler/types.h>
#include <compiler/hashing/murmur.h>

namespace basecode::compiler::errors {

    using error_code_t = uint32_t;

    struct error_decl_key_t final {
        std::string locale;
        error_code_t code;

        bool operator==(const error_decl_key_t& other) const {
            return locale == other.locale && code == other.code;
        }
    };

    struct error_decl_t final {
        std::string code{};
        std::string message{};
        std::string details{};
    };

    error_decl_t* find_decl(error_code_t code);

    ///////////////////////////////////////////////////////////////////////////

    namespace lexer {
        static constexpr error_code_t unable_to_convert_integer_value = 1;
        static constexpr error_code_t unable_to_narrow_integer_value = 2;
        static constexpr error_code_t unable_to_convert_floating_point_value = 3;
        static constexpr error_code_t unable_to_narrow_floating_point_value = 4;
        static constexpr error_code_t invalid_identifier_start_character = 5;
        static constexpr error_code_t expected_identifier = 6;
        static constexpr error_code_t unexpected_end_of_input = 7;
        static constexpr error_code_t unexpected_decimal_point = 8;
        static constexpr error_code_t expected_closing_single_quote = 9;
        static constexpr error_code_t expected_directive_prefix = 10;
        static constexpr error_code_t expected_annotation_prefix = 11;
        static constexpr error_code_t exponent_notation_not_valid_for_integers = 12;
        static constexpr error_code_t unexpected_letter_after_decimal_number_literal = 13;
        static constexpr error_code_t expected_hex_literal_prefix = 14;
        static constexpr error_code_t unexpected_letter_after_hexadecimal_number_literal = 15;
        static constexpr error_code_t expected_octal_literal_prefix = 16;
        static constexpr error_code_t unexpected_letter_after_octal_number_literal = 17;
        static constexpr error_code_t expected_binary_literal_prefix = 18;
        static constexpr error_code_t unexpected_letter_after_binary_number_literal = 19;
        static constexpr error_code_t expected_closing_block_literal = 20;
    }

    ///////////////////////////////////////////////////////////////////////////

    template <typename... Args>
    void add_error(
            result_t& r,
            error_code_t code,
            const source_location_t& loc,
            Args&&... args) {
        auto decl = find_decl(code);
        assert(decl != nullptr);

        auto message = decl->message;
        if (sizeof...(args) > 0) {
            message = fmt::format(message, std::forward<Args>(args)...);
        }

        r.error(decl->code, message, loc, decl->details);
    }

    template <typename... Args>
    void add_warning(
            result_t& r,
            error_code_t code,
            const source_location_t& loc,
            Args&&... args) {
        auto decl = find_decl(code);
        assert(decl != nullptr);

        auto message = decl->message;
        if (sizeof...(args) > 0) {
            message = fmt::format(message, std::forward<Args>(args)...);
        }

        r.warning(decl->code, message, loc, decl->details);
    }

    template <typename... Args>
    void add_info(result_t& r, error_code_t code, Args&&... args) {
        auto decl = find_decl(code);
        assert(decl != nullptr);

        auto message = decl->message;
        if (sizeof...(args) > 0) {
            message = fmt::format(message, std::forward<Args>(args)...);
        }

        r.info(decl->code, message, {}, decl->details);
    }

    template <typename... Args>
    void add_error(result_t& r, error_code_t code, Args&&... args) {
        auto decl = find_decl(code);
        assert(decl != nullptr);

        auto message = decl->message;
        if (sizeof...(args) > 0) {
            message = fmt::format(message, std::forward<Args>(args)...);
        }

        r.error(decl->code, message, {}, decl->details);
    }

    template <typename... Args>
    void add_warning(result_t& r, error_code_t code, Args&&... args) {
        auto decl = find_decl(code);
        assert(decl != nullptr);

        auto message = decl->message;
        if (sizeof...(args) > 0) {
            message = fmt::format(message, std::forward<Args>(args)...);
        }

        r.warning(decl->code, message, {}, decl->details);
    }

}

namespace std {

    template<>
    struct hash<basecode::compiler::errors::error_decl_key_t> {
        std::size_t operator()(const basecode::compiler::errors::error_decl_key_t& key) const {
            fmt::memory_buffer buffer{};
            fmt::format_to(buffer, "{}:{}", key.locale, key.code);
            return basecode::compiler::hashing::murmur::hash64(
                buffer.data(),
                buffer.size());
        }
    };

}