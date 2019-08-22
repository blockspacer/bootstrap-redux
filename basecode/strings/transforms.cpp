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

#include <sstream>
#include <fmt/format.h>
#include <basecode/formatters/formatters.h>
#include "transforms.h"

namespace basecode::strings {

    // XXX: FIX THIS
    //
    void word_wrap(
            adt::string_t& text,
            size_t width,
            size_t right_pad,
            const char& fill) {
        size_t line_begin = 0;

        while (line_begin < text.size()) {
            const auto ideal_end = line_begin + width;
            size_t line_end = ideal_end <= text.size() ?
                              ideal_end :
                              text.size() - 1;

            if (line_end == text.size() - 1)
                ++line_end;
            else if (std::isspace(text[line_end])) {
                text[line_end++] = '\n';
                text.insert(line_end++, right_pad, fill);
            } else {
                auto end = line_end;
                while (end > line_begin && !std::isspace(text[end]))
                    --end;

                if (end != line_begin) {
                    line_end = end;
                    text[line_end++] = '\n';
                    text.insert(line_end++, right_pad, fill);
                } else {
                    text.insert(line_end++, 1, '\n');
                    text.insert(line_end++, right_pad, fill);
                }
            }

            line_begin = line_end;
        }
    }

    // XXX: FIX THIS
    //
    adt::string_t remove_underscores(const std::string_view& value) {
        fmt::memory_buffer buffer{};
        for (const auto& c : value)
            if (c != '_') fmt::format_to(buffer, "{}", c);
        return fmt::to_string(buffer);
    }

    // XXX: FIX THIS
    //
    std::pair<std::string, std::string> size_to_units(size_t size) {
        auto i = 0;
        const char* units[] = {"bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
        while (size > 1024) {
            size /= 1024;
            i++;
        }
        return std::make_pair(
            i > 1 ?
            fmt::format("{}.{}", i, size) :
            fmt::format("{}", size),
            units[i]);
    }

    // XXX: FIX THIS
    //
    adt::string_t list_to_string(const string_list_t& list, const char& sep) {
        fmt::memory_buffer buffer{};
        for (size_t i = 0; i < list.size(); i++) {
            if (i > 0)
                fmt::format_to(buffer, "{}", sep);
            fmt::format_to(buffer, "{}", list[i]);
        }
        return fmt::to_string(buffer);
    }

    string_list_t string_to_list(const adt::string_t& value, const char& sep) {
        string_list_t list{};

        std::istringstream f(value.as_std_string());
        std::string s;
        while (std::getline(f, s, sep)) {
            list.add(s);
        }

        return list;
    }

    adt::string_t pad_to(const adt::string_t& str, const size_t num, const char padding) {
        if (num > str.size()) {
            auto padded = str;
            padded.insert(0, num - padded.size(), padding);
            return padded;
        } else {
            return str;
        }
    }

}