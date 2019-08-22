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

#include <basecode/types.h>
#include <basecode/adt/string.h>

namespace basecode::strings {

    void word_wrap(
        adt::string_t& text,
        size_t width,
        size_t right_pad = 0,
        const char& fill = ' ');

    adt::string_t pad_to(
        const adt::string_t& str,
        size_t num,
        char padding = ' ');

    // XXX: FIX THIS
    std::pair<std::string, std::string> size_to_units(size_t size);

    adt::string_t remove_underscores(const std::string_view& value);

    adt::string_t list_to_string(const string_list_t& list, const char& sep = ',');

    string_list_t string_to_list(const adt::string_t& value, const char& sep = ',');

}