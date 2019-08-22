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

#include <memory>
#include <string>
#include <memory>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <fmt/format.h>
#include <compiler/utf8/reader.h>

namespace basecode::terminal {

    using namespace std::literals;

    enum class colors_t : uint8_t {
        black = 30,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        light_gray,
        default_color = 39,

        dark_gray = 90,
        light_red,
        light_green,
        light_yellow,
        light_blue,
        light_magenta,
        light_cyan,
        white
    };

    constexpr std::string_view color_code_reset() {
        return "\033[0m"sv;
    }

    std::string color_code(colors_t fg_color, colors_t bg_color);

    class stream_t {
    public:
        virtual ~stream_t() = default;

        virtual stream_t* color(
            colors_t bg,
            colors_t fg) = 0;

        virtual stream_t* color_reset() = 0;

        virtual stream_t* dim(bool enabled) = 0;

        virtual stream_t* bold(bool enabled) = 0;

        virtual stream_t* blink(bool enabled) = 0;

        virtual stream_t* reverse(bool enabled) = 0;

        virtual fmt::memory_buffer& underlying() = 0;

        virtual stream_t* underline(bool enabled) = 0;

        [[nodiscard]] virtual std::string format() const = 0;

        virtual stream_t* append(const std::string& value) = 0;

        virtual stream_t* append(const std::string_view& value) = 0;

        virtual stream_t* append(const std::string& value, ssize_t width) = 0;

        virtual stream_t* append(const std::string_view& value, ssize_t width) = 0;
    };

    class ascii_stream_t : public stream_t {
    public:
        explicit ascii_stream_t(fmt::memory_buffer& buffer) : _buffer(buffer) {
        }

        stream_t* color(
                colors_t bg,
                colors_t fg) override {
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* color_reset() override {
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* dim(bool enabled) override {
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* bold(bool enabled) override {
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* blink(bool enabled) override {
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* reverse(bool enabled) override {
            return dynamic_cast<stream_t*>(this);
        }

        fmt::memory_buffer& underlying() override {
            return _buffer;
        }

        stream_t* underline(bool enabled) override {
            return dynamic_cast<stream_t*>(this);
        }

        [[nodiscard]] std::string format() const override {
            return fmt::to_string(_buffer);
        }

        stream_t* append(const std::string& value) override {
            fmt::format_to(_buffer, "{}", value);
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* append(const std::string_view& value) override {
            fmt::format_to(_buffer, "{}", value);
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* append(const std::string& value, ssize_t width) override {
            fmt::format_to(_buffer, "{:<{}}", value, width);
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* append(const std::string_view& value, ssize_t width) override {
            fmt::format_to(_buffer, "{:<{}}", value, width);
            return dynamic_cast<stream_t*>(this);
        }

    private:
        fmt::memory_buffer& _buffer;
    };

    class ansi_stream_t : public stream_t {
    public:
        explicit ansi_stream_t(fmt::memory_buffer& buffer) : _buffer(buffer) {
        }

        stream_t* color(
                colors_t bg,
                colors_t fg) override {
            fmt::format_to(_buffer, "{}", color_code(fg, bg));
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* color_reset() override {
            fmt::format_to(_buffer, "{}", color_code_reset());
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* dim(bool enabled) override {
            if (enabled)
                fmt::format_to(_buffer, "\033[2m");
            else
                fmt::format_to(_buffer, "\033[22m");
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* bold(bool enabled) override {
            if (enabled)
                fmt::format_to(_buffer, "\033[1m");
            else
                fmt::format_to(_buffer, "\033[21m");
            return dynamic_cast<stream_t*>(this);
        }

        fmt::memory_buffer& underlying() override {
            return _buffer;
        }

        stream_t* blink(bool enabled) override {
            if (enabled)
                fmt::format_to(_buffer, "\033[5m");
            else
                fmt::format_to(_buffer, "\033[25m");
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* reverse(bool enabled) override {
            if (enabled)
                fmt::format_to(_buffer, "\033[7m");
            else
                fmt::format_to(_buffer, "\033[27m");
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* underline(bool enabled) override {
            if (enabled)
                fmt::format_to(_buffer, "\033[4m");
            else
                fmt::format_to(_buffer, "\033[24m");
            return dynamic_cast<stream_t*>(this);
        }

        [[nodiscard]] std::string format() const override {
            return fmt::to_string(_buffer);
        }

        stream_t* append(const std::string& value) override {
            fmt::format_to(_buffer, "{}", value);
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* append(const std::string_view& value) override {
            fmt::format_to(_buffer, "{}", value);
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* append(const std::string& value, ssize_t width) override {
            fmt::format_to(_buffer, "{:<{}}", value, width);
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* append(const std::string_view& value, ssize_t width) override {
            fmt::format_to(_buffer, "{:<{}}", value, width);
            return dynamic_cast<stream_t*>(this);
        }

    private:
        fmt::memory_buffer& _buffer;
    };

    using stream_unique_ptr_t = std::unique_ptr<stream_t>;

    class stream_factory_t {
    public:
        stream_factory_t() = default;

        stream_factory_t(const stream_factory_t&) = delete;

        void enabled(bool value);

        [[nodiscard]] bool enabled() const;

        [[nodiscard]] std::string colorize(
            std::string_view text,
            colors_t fg_color,
            colors_t bg_color = colors_t::default_color) const;

        [[nodiscard]] std::string colorize_range(
            utf8::reader_t& reader,
            size_t begin,
            size_t end,
            colors_t fg_color,
            colors_t bg_color = colors_t::default_color) const;

        [[nodiscard]] stream_unique_ptr_t use_memory_buffer(fmt::memory_buffer& buffer) const;

    private:
        bool _enabled;
    };

}