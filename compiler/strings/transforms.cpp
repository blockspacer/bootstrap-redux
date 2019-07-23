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

#include <fmt/format.h>
#include "transforms.h"

namespace basecode::compiler::strings {

    std::string word_wrap(
            std::string text,
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
                for (size_t i = 0; i < right_pad; i++)
                    text.insert(line_end++, 1, fill);
            } else {
                auto end = line_end;
                while (end > line_begin && !std::isspace(text[end]))
                    --end;

                if (end != line_begin) {
                    line_end = end;
                    text[line_end++] = '\n';
                    for (size_t i = 0; i < right_pad; i++)
                        text.insert(line_end++, 1, fill);
                } else {
                    text.insert(line_end++, 1, '\n');
                    for (size_t i = 0; i < right_pad; i++)
                        text.insert(line_end++, 1, fill);
                }
            }

            line_begin = line_end;
        }

        return text;
    }

    std::string escaped_string(const std::string& value) {
        std::stringstream stream;
        for (size_t i = 0; i < value.size(); i++) {
            auto ch = value[i];
            if (ch == '\\') {
                ch = value[++i];
                switch (ch) {
                    case 'a': {
                        stream << (char)0x07;
                        break;
                    }
                    case 'b': {
                        stream << (char)0x08;
                        break;
                    }
                    case 'e': {
                        stream << (char)0x1b;
                        break;
                    }
                    case 'n': {
                        stream << (char)0x0a;
                        break;
                    }
                    case 'r': {
                        stream << (char)0x0d;
                        break;
                    }
                    case 't': {
                        stream << (char)0x09;
                        break;
                    }
                    case 'v': {
                        stream << (char)0x0b;
                        break;
                    }
                    case '\\': {
                        stream << "\\";
                        break;
                    }
                    case '\'': {
                        stream << "'";
                        break;
                    }
                    case 'x': {
//                        if (!read_hex_digits(2, value))
//                            return false;
//                        radix = 16;
//                        number_type = number_types_t::integer;
                        break;
                    }
                    case 'u': {
//                        if (!read_hex_digits(4, value))
//                            return false;
//                        radix = 16;
//                        number_type = number_types_t::integer;
                        break;
                    }
                    case 'U': {
//                        if (!read_hex_digits(8, value))
//                            return false;
//                        radix = 16;
//                        number_type = number_types_t::integer;
                        break;
                    }
                    default: {
//                        if (!read_dec_digits(3, value))
//                            return false;
//                        radix = 8;
//                        number_type = number_types_t::integer;
                        break;
                    }
                }
            } else {
                stream << ch;
            }
        }
        return stream.str();
    }

    std::string remove_underscores(const std::string_view& value) {
        std::stringstream stream {};
        for (const auto& c : value)
            if (c != '_') stream << c;
        return stream.str();
    }

    std::pair<std::string, std::string> size_to_units(size_t size) {
        auto i = 0;
        const char* units[] = {"bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
        while (size > 1024) {
            size /= 1024;
            i++;
        }
        return std::make_pair(
            i > 1 ? fmt::format("{}.{}", i, size) : fmt::format("{}", size),
            units[i]);
    }

    std::string list_to_string(const string_list_t& list, const char& sep) {
        std::stringstream stream;

        for (size_t i = 0; i < list.size(); i++) {
            if (i > 0)
                stream << sep;
            stream << list[i];
        }

        return stream.str();
    }

    string_list_t string_to_list(const std::string& value, const char& sep) {
        string_list_t list;

        std::istringstream f(value);
        std::string s;
        while (std::getline(f, s, sep)) {
            list.push_back(s);
        }

        return list;
    }

}