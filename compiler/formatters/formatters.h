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

#include <fmt/format.h>
#include <compiler/types.h>
#include <compiler/utf8/rune.h>
#include <compiler/lexer/token.h>

namespace fmt {

    template<>
    struct formatter<basecode::compiler::utf8::rune_t> {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx) {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(
                const basecode::compiler::utf8::rune_t& rune,
                FormatContext& ctx) {
            std::string temp{};
            auto encode_result = basecode::compiler::utf8::encode(rune);
            for (size_t j = 0; j < encode_result.width; j++)
                temp += static_cast<char>(encode_result.data[j]);
            return format_to(ctx.out(), "{}", temp);
        }
    };

    template<>
    struct formatter<basecode::compiler::source_location_t> {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx) {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(
                const basecode::compiler::source_location_t& loc,
                FormatContext& ctx) {
            return format_to(
                ctx.out(),
                "{}@{}-{}@{}",
                loc.start.line + 1,
                loc.start.column + 1,
                loc.end.line + 1,
                loc.end.column);
        }
    };

    template<>
    struct formatter<basecode::compiler::lexer::token_t> {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx) {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(
                const basecode::compiler::lexer::token_t& token,
                FormatContext& ctx) {
            format_to(
                ctx.out(),
                "<type = {}",
                basecode::compiler::lexer::token_type_to_name(token.type));
            if (!token.value.empty()) {
                format_to(ctx.out(), ", value = {}", token.value);
            }
            return format_to(ctx.out(), ">");
        }
    };

    template<>
    struct formatter<basecode::compiler::lexer::number_token_t> {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx) {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(
                const basecode::compiler::lexer::number_token_t& token,
                FormatContext& ctx) {
            format_to(
                ctx.out(),
                "<is_signed = {}, radix = {}, type = {}",
                token.is_signed,
                token.radix,
                basecode::compiler::lexer::number_type_to_name(token.type));
            if (token.type == basecode::compiler::lexer::number_type_t::integer) {
                return format_to(ctx.out(), ", value = {}>", token.value.i);
            } else if (token.type == basecode::compiler::lexer::number_type_t::floating_point) {
                return format_to(ctx.out(), ", value = {}>", token.value.d);
            } else {
                // XXX: revisit
                return format_to(ctx.out(), ">");
            }
        }
    };

}
