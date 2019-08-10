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
#include <compiler/strings/pool.h>
#include <compiler/hashing/murmur.h>
#include <compiler/utf8/source_buffer.h>
#include <compiler/terminal/stream_factory.h>

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
        static constexpr error_code_t unescaped_quote = 22;
    }

    namespace parser {
        static constexpr error_code_t invalid_token = 21;
    }

    namespace source_buffer {
        static constexpr error_code_t unable_to_open_file = 23;
    }

    ///////////////////////////////////////////////////////////////////////////

    template <typename... Args>
    void add_error(
            result_t& r,
            strings::pool_t& intern_pool,
            error_code_t code,
            const source_location_t& loc,
            Args&&... args) {
        auto decl = find_decl(code);
        assert(decl != nullptr);

        if (sizeof...(args) > 0) {
            auto formatted_message = intern_pool.intern(fmt::format(
                decl->message,
                std::forward<Args>(args)...));
            r.error(decl->code, formatted_message, loc, decl->details);
        } else {
            r.error(decl->code, decl->message, loc, decl->details);
        }
    }

    template <typename... Args>
    void add_warning(
            result_t& r,
            strings::pool_t& intern_pool,
            error_code_t code,
            const source_location_t& loc,
            Args&&... args) {
        auto decl = find_decl(code);
        assert(decl != nullptr);

        if (sizeof...(args) > 0) {
            auto formatted_message = intern_pool.intern(fmt::format(
                decl->message,
                std::forward<Args>(args)...));
            r.warning(decl->code, formatted_message, loc, decl->details);
        } else {
            r.warning(decl->code, decl->message, loc, decl->details);
        }
    }

    template <typename... Args>
    void add_info(
            result_t& r,
            strings::pool_t& intern_pool,
            error_code_t code,
            Args&&... args) {
        auto decl = find_decl(code);
        assert(decl != nullptr);

        if (sizeof...(args) > 0) {
            auto formatted_message = intern_pool.intern(fmt::format(
                decl->message,
                std::forward<Args>(args)...));
            r.info(decl->code, formatted_message, {}, decl->details);
        } else {
            r.info(decl->code, decl->message, {}, decl->details);
        }
    }

    template <typename... Args>
    void add_error(
            result_t& r,
            strings::pool_t& intern_pool,
            error_code_t code,
            Args&&... args) {
        auto decl = find_decl(code);
        assert(decl != nullptr);

        if (sizeof...(args) > 0) {
            auto formatted_message = intern_pool.intern(fmt::format(
                decl->message,
                std::forward<Args>(args)...));
            r.error(decl->code, formatted_message, {}, decl->details);
        } else {
            r.error(decl->code, decl->message, {}, decl->details);
        }
    }

    template <typename... Args>
    void add_warning(
            result_t& r,
            strings::pool_t& intern_pool,
            error_code_t code,
            Args&&... args) {
        auto decl = find_decl(code);
        assert(decl != nullptr);

        if (sizeof...(args) > 0) {
            auto formatted_message = intern_pool.intern(fmt::format(
                decl->message,
                std::forward<Args>(args)...));
            r.warning(decl->code, formatted_message, {}, decl->details);
        } else {
            r.warning(decl->code, decl->message, {}, decl->details);
        }
    }

    template <typename... Args>
    void add_source_highlighted_error(
            result_t& r,
            strings::pool_t& intern_pool,
            error_code_t code,
            utf8::source_buffer_t& buffer,
            const source_location_t& loc,
            Args&&... args) {
        auto decl = find_decl(code);
        assert(decl != nullptr);

        auto message = decl->message;
        if (sizeof...(args) > 0)
            message = fmt::format(message, std::forward<Args>(args)...);

        fmt::memory_buffer stream;
        terminal::stream_factory_t term{};
        term.enabled(true);

        const auto number_of_lines = buffer.number_of_lines();
        const auto target_line = loc.start.line;
        const auto message_indicator = term.colorize(
            fmt::format("^ {}", message),
            terminal::colors_t::red);

        auto start_line = loc.start.line - 4;
        if (start_line < 0)
            start_line = 0;

        auto stop_line = loc.end.line + 4;
        if (stop_line >= number_of_lines)
            stop_line = number_of_lines;

        for (int32_t i = start_line; i < stop_line; i++) {
            const auto source_line = buffer.line_by_number(i);
            if (source_line == nullptr)
                break;
            const auto source_text = buffer.substring(
                source_line->begin,
                source_line->end);
            if (!source_text.empty()) {
                if (i == target_line) {
                    fmt::format_to(stream, "{:8d}: ", i + 1);
                    fmt::format_to(
                        stream,
                        "{}\n",
                        term.colorize_range(
                            source_text,
                            loc.start.column,
                            loc.end.column,
                            terminal::colors_t::yellow,
                            terminal::colors_t::blue));
                    fmt::format_to(
                        stream,
                        "{}{}",
                        std::string(10 + loc.start.column, ' '),
                        message_indicator);
                } else {
                    fmt::format_to(stream, "{:8d}: {}", i + 1, source_text);
                }
            }
            if (i < static_cast<int32_t>(stop_line - 1))
                fmt::format_to(stream, "\n");
        }

        auto& path = buffer.path();
        if (!path.empty()) {
            message = fmt::format(
                "({}@{}:{}) {}",
                path.filename().string(),
                loc.start.line + 1,
                loc.start.column + 1,
                message);
        } else {
            message = fmt::format(
                "((anonymous source)@{}:{}) {}",
                loc.start.line + 1,
                loc.start.column + 1,
                message);
        }

        auto interned_message = intern_pool.intern(message);
        auto interned_details = intern_pool.intern(fmt::to_string(stream));
        r.error(decl->code, interned_message, loc, interned_details);
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