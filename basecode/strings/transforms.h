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

#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <type_traits>
#include <basecode/types.h>

namespace basecode::strings {

    std::string word_wrap(
        std::string text,
        size_t width,
        size_t right_pad = 0,
        const char& fill = ' ');

    inline std::string pad_to(
            const std::string& str,
            const size_t num,
            const char padding = ' ') {
        if (num > str.size()) {
            auto padded = str;
            padded.insert(padded.begin(), num - padded.size(), padding);
            return padded;
        } else {
            return str;
        }
    }

    std::string remove_underscores(const std::string_view& value);

    std::pair<std::string, std::string> size_to_units(size_t size);

    std::string list_to_string(const string_list_t& list, const char& sep = ',');

    string_list_t string_to_list(const std::string& value, const char& sep = ',');

}