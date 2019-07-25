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

    void reader_t::push_mark() {
        _mark_stack.push(_index);
    }

    bool reader_t::eof() const {
        return _index > _slice.size() - 1;
    }

    size_t reader_t::pop_mark() {
        if (_mark_stack.empty())
            return _index;
        const auto mark = _mark_stack.top();
        _mark_stack.pop();
        return mark;
    }

    size_t reader_t::pos() const {
        return _index;
    }

    size_t reader_t::current_mark() {
        if (_mark_stack.empty())
            return _index;
        return _mark_stack.top();
    }

    bool reader_t::seek(size_t index) {
        if (index > _slice.size() - 1)
            return false;
        _index = index;
        return true;
    }

    rune_t reader_t::next(result_t& r) {
        if (eof()) return rune_eof;

        uint8_t width;
        auto rune = read(r, width);
        _index += width;
        _width_stack.push(width);
        return rune;
    }

    void reader_t::restore_top_mark() {
        if (_mark_stack.empty())
            return;
        _index = _mark_stack.top();
    }

    rune_t reader_t::prev(result_t& r) {
        if (_index == 0 || _width_stack.empty()) {
            r.error("S003", "at beginning of buffer");
            return rune_invalid;
        }
        _index = _width_stack.top();
        _width_stack.pop();
        uint8_t width;
        return read(r, width);
    }

    rune_t reader_t::read(result_t& r, uint8_t& width) {
        width = 1;
        uint8_t ch = _slice[_index];
        auto rune = rune_t(ch);
        if (ch == 0) {
            r.error("S003", "illegal character NUL");
            return rune_invalid;
        } else if (ch >= 0x80) {
            auto cp = decode(
                (char*)(_slice.data() + _index),
                _slice.size() - _index);
            width = cp.width;
            rune = cp.value;
            if (rune == rune_invalid && width == 1) {
                r.error("S001", "illegal utf-8 encoding");
                return rune_invalid;
            } else if (rune == rune_bom && _index > 0) {
                r.error("S002", "illegal byte order mark");
                return rune_invalid;
            }
        }
        return rune;
    }

    std::string_view reader_t::make_slice(size_t offset, size_t length) const {
        return std::string_view(
            reinterpret_cast<const char*>(_slice.data() + offset),
            length);
    }

}