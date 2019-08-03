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

namespace basecode::compiler::terminal {

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

        virtual std::stringstream& underlying() = 0;

        virtual stream_t* blink(bool enabled) = 0;

        virtual stream_t* reverse(bool enabled) = 0;

        virtual stream_t* underline(bool enabled) = 0;

        [[nodiscard]] virtual std::string format() const = 0;

        virtual stream_t* append(const std::string& value) = 0;

        virtual stream_t* append(const std::string_view& value) = 0;

        virtual stream_t* append(const std::string& value, ssize_t width) = 0;

        virtual stream_t* append(const std::string_view& value, ssize_t width) = 0;
    };

    class ascii_stream_t : public stream_t {
    public:
        explicit ascii_stream_t(std::stringstream& stream) : _stream(stream) {
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

        std::stringstream& underlying() override {
            return _stream;
        }

        stream_t* blink(bool enabled) override {
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* reverse(bool enabled) override {
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* underline(bool enabled) override {
            return dynamic_cast<stream_t*>(this);
        }

        [[nodiscard]] std::string format() const override {
            return _stream.str();
        }

        stream_t* append(const std::string& value) override {
            _stream << value;
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* append(const std::string_view& value) override {
            _stream << value;
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* append(const std::string& value, ssize_t width) override {
            _stream << std::left << std::setw(width) << value;
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* append(const std::string_view& value, ssize_t width) override {
            _stream << std::left << std::setw(width) << value;
            return dynamic_cast<stream_t*>(this);
        }

    private:
        std::stringstream& _stream;
    };

    class ansi_stream_t : public stream_t {
    public:
        explicit ansi_stream_t(std::stringstream& stream) : _stream(stream) {
        }

        stream_t* color(
            colors_t bg,
            colors_t fg) override {
            _stream << color_code(fg, bg);
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* color_reset() override {
            _stream << color_code_reset();
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* dim(bool enabled) override {
            if (enabled)
                _stream << "\033[2m"sv;
            else
                _stream << "\033[22m"sv;
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* bold(bool enabled) override {
            if (enabled)
                _stream << "\033[1m"sv;
            else
                _stream << "\033[21m"sv;
            return dynamic_cast<stream_t*>(this);
        }

        std::stringstream& underlying() override {
            return _stream;
        }

        stream_t* blink(bool enabled) override {
            if (enabled)
                _stream << "\033[5m"sv;
            else
                _stream << "\033[25m"sv;
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* reverse(bool enabled) override {
            if (enabled)
                _stream << "\033[7m"sv;
            else
                _stream << "\033[27m"sv;
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* underline(bool enabled) override {
            if (enabled)
                _stream << "\033[4m"sv;
            else
                _stream << "\033[24m"sv;
            return dynamic_cast<stream_t*>(this);
        }

        [[nodiscard]] std::string format() const override {
            return _stream.str();
        }

        stream_t* append(const std::string& value) override {
            _stream << value;
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* append(const std::string_view& value) override {
            _stream << value;
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* append(const std::string& value, ssize_t width) override {
            _stream << std::left << std::setw(width) << value;
            return dynamic_cast<stream_t*>(this);
        }

        stream_t* append(const std::string_view& value, ssize_t width) override {
            _stream << std::left << std::setw(width) << value;
            return dynamic_cast<stream_t*>(this);
        }

    private:
        std::stringstream& _stream;
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
            std::string_view text,
            size_t begin,
            size_t end,
            colors_t fg_color,
            colors_t bg_color = colors_t::default_color) const;

        [[nodiscard]] stream_unique_ptr_t use_stream(std::stringstream& stream) const;

    private:
        bool _enabled;
    };


}