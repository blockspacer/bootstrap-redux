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
#include <compiler/utf8/rune.h>

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

}
