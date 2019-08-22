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

#include <utf8proc.h>
#include <compiler/numbers/parse.h>
#include <compiler/formatters/formatters.h>
#include "escape_parser.h"

namespace basecode::strings {

    escape_parser_t::escape_parser_t(
            utf8::reader_t& reader) : _reader(reader) {
    }

    bool escape_parser_t::read_digits(
            result_t& r,
            const predicate_t& predicate,
            size_t len,
            std::string_view& value) {
        auto start_pos = _reader.pos();
        while (len > 0) {
            auto ch = _reader.next(r);
            if (ch == '_')
                continue;
            if (predicate(ch)) {
                --len;
            } else {
                r.error("X000", "unexpected digit");
                return false;
            }
        }
        value = _reader.make_slice(start_pos, _reader.pos() - start_pos);
        return true;
    }

    bool escape_parser_t::parse(result_t& r, fmt::memory_buffer& stream) {
        while (!_reader.eof()) {
            auto rune = _reader.next(r);
            if (rune == utf8::rune_invalid)
                break;

            if (rune == '\\') {
                rune = _reader.next(r);
                if (rune == utf8::rune_invalid
                ||  rune == utf8::rune_eof) {
                    r.error("X000", "unexpected end of input.");
                    return false;
                }

                uint8_t radix = 16;
                std::string_view number_value{};

                switch ((int32_t)rune) {
                    case 'a': {
                        fmt::format_to(stream, "{}", (char)0x07);
                        break;
                    }
                    case 'b': {
                        fmt::format_to(stream, "{}", (char)0x08);
                        break;
                    }
                    case 'e': {
                        fmt::format_to(stream, "{}", (char)0x1b);
                        break;
                    }
                    case 'n': {
                        fmt::format_to(stream, "{}", (char)0x0a);
                        break;
                    }
                    case 'r': {
                        fmt::format_to(stream, "{}", (char)0x0d);
                        break;
                    }
                    case 't': {
                        fmt::format_to(stream, "{}", (char)0x09);
                        break;
                    }
                    case 'v': {
                        fmt::format_to(stream, "{}", (char)0x0b);
                        break;
                    }
                    case '\\': {
                        fmt::format_to(stream, "{}", "\\");
                        break;
                    }
                    case '\'': {
                        fmt::format_to(stream, "{}", "'");
                        break;
                    }
                    case '\"': {
                        fmt::format_to(stream, "{}", "\"");
                        break;
                    }
                    case 'x': {
                        if (!read_digits(
                                r,
                                [](utf8::rune_t& rune) { return isxdigit((int)rune); },
                                2,
                                number_value)) {
                            return false;
                        }
                        break;
                    }
                    case 'u': {
                        if (!read_digits(
                                r,
                                [](utf8::rune_t& rune) { return isxdigit((int)rune); },
                                4,
                                number_value)) {
                            return false;
                        }
                        break;
                    }
                    case 'U': {
                        if (!read_digits(
                                r,
                                [](utf8::rune_t& rune) { return isxdigit((int)rune); },
                                8,
                                number_value)) {
                            return false;
                        }
                        break;
                    }
                    default: {
                        if (!read_digits(
                                r,
                                [](utf8::rune_t& rune) { return isdigit((int)rune); },
                                3,
                                number_value)) {
                            return false;
                        }
                        radix = 8;
                        break;
                    }
                }

                auto number_value_len = number_value.size();
                if (number_value_len > 0) {
                    int32_t value{};
                    auto result = numbers::parse_integer(number_value, radix, value);
                    if (result != numbers::conversion_result_t::success) {
                        r.error(
                            "X000",
                            fmt::format(
                                "invalid {} escape {} because: {}",
                                radix == 8 ? "octal" : "hex",
                                number_value,
                                numbers::conversion_result_to_name(result)));
                        return false;
                    }

                    if (number_value_len < 4) {
                        fmt::format_to(stream, "{}", (char)value);
                    } else {
                        if (!utf8proc_codepoint_valid(value)) {
                            r.error("X000", fmt::format("invalid unicode codepoint: {}", value));
                            return false;
                        } else {
                            uint8_t data[4];
                            auto len = utf8proc_encode_char(value, data);
                            for (size_t i = 0; i < len; i++) {
                                fmt::format_to(stream, "{}", (char)data[i]);
                            }
                        }
                    }
                }
            } else {
                fmt::format_to(stream, "{}", rune);
            }
        }

        return !r.is_failed();
    }

}