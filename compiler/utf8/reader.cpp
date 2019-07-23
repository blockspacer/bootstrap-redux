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

#include "reader.h"

namespace basecode::compiler::utf8 {

    reader_t::reader_t(std::string_view slice) : _slice(slice) {
    }

    rune_t reader_t::next() {
        if (_index > _slice.size() - 1)
            return rune_eof;
        _last_width = 1;
        uint8_t ch = _slice[_index];
        auto rune = rune_t(ch);
        if (ch == 0) {
            return rune_invalid;
        } else if (ch >= 0x80) {
            auto cp = decode(
                (char*)(_slice.data() + _index),
                _slice.size() - _index);
            _last_width = cp.width;
            rune = cp.value;
            if (rune == rune_invalid && _last_width == 1) {
                return rune_invalid;
            } else if (rune == rune_bom && _index > 0) {
                return rune_invalid;
            }
        }
        _index += _last_width;
        return rune;
    }

}