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

#include <compiler/utf8/reader.h>
#include "stream_builder.h"

namespace basecode::compiler::terminal {

    std::string color_code(colors_t fg_color, colors_t bg_color) {
        return fmt::format(
            "\033[1;{};{}m",
            (uint32_t) fg_color,
            ((uint32_t) bg_color) + 10);
    }

    ///////////////////////////////////////////////////////////////////////////

    std::string stream_builder_t::colorize(
        std::string_view text,
        colors_t fg_color,
        colors_t bg_color) const {
        if (!_enabled)
            return std::string(text);
        return fmt::format(
            "{}{}{}",
            color_code(fg_color, bg_color),
            text,
            color_code_reset());
    }

    bool stream_builder_t::enabled() const {
        return _enabled;
    }

    void stream_builder_t::enabled(bool value) {
        _enabled = value;
    }

    std::string stream_builder_t::colorize_range(
            std::string_view text,
            size_t begin,
            size_t end,
            colors_t fg_color,
            colors_t bg_color) const {
        if (!_enabled)
            return std::string(text);

        auto j = 0;
        result_t r{};
        utf8::reader_t reader(text);
        auto in_colored_range = false;
        std::stringstream colored_source;
        while (true) {
            auto rune = reader.next(r);
            if (rune == utf8::rune_eof)
                break;

            if (begin == end && j == begin) {
                colored_source << color_code(fg_color, bg_color);
                colored_source << rune;
                colored_source << color_code_reset();
            } else {
                if (j == begin) {
                    colored_source << color_code(fg_color, bg_color);
                    in_colored_range = true;
                } else if (j == end) {
                    colored_source << color_code_reset();
                    in_colored_range = false;
                }
                colored_source << rune;
            }

            j++;
        }

        if (in_colored_range) {
            colored_source << color_code_reset();
        }

        return colored_source.str();
    }

    stream_unique_ptr_t stream_builder_t::use_stream(std::stringstream& stream) const {
        if (_enabled)
            return stream_unique_ptr_t(new ansi_stream_t(stream));
        else
            return stream_unique_ptr_t(new ascii_stream_t(stream));
    }

}