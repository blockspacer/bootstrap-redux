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

#include <map>
#include <stack>
#include <cstdint>
#include <string_view>
#include <compiler/types.h>
#include <compiler/terminal/stream_factory.h>
#include "reader.h"

namespace basecode::compiler::utf8 {

    using source_buffer_range_t = std::pair<size_t, size_t>;

    struct source_buffer_range_compare_t {
        bool operator()(
            const source_buffer_range_t& lhs,
            const source_buffer_range_t& rhs) const {
            return lhs.second < rhs.first;
        }
    };

    struct source_buffer_line_t {
        size_t end{};
        size_t begin{};
        int32_t line{};
        int32_t columns{};
    };

    class source_buffer_t {
    public:
        explicit source_buffer_t(memory::allocator_t* allocator);

        ~source_buffer_t();

        bool load(
            result_t& r,
            strings::pool_t& pool,
            const std::string& buffer);

        bool load(
            result_t& r,
            strings::pool_t& pool,
            const path_t& path);

        void push_mark();

        size_t pop_mark();

        size_t current_mark();

        bool seek(size_t index);

        void restore_top_mark();

        rune_t curr(result_t& r);

        rune_t next(result_t& r);

        rune_t prev(result_t& r);

        bool move_prev(result_t& r);

        bool move_next(result_t& r);

        [[nodiscard]] bool eof() const;

        memory::allocator_t* allocator();

        uint8_t operator[](size_t index);

        [[nodiscard]] size_t pos() const;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t length() const;

        [[nodiscard]] uint32_t width() const;

        [[nodiscard]] const path_t& path() const;

        [[nodiscard]] size_t number_of_lines() const;

        [[nodiscard]] int32_t column_by_index(size_t index) const;

        [[nodiscard]] std::string_view substring(size_t start, size_t end) const;

        [[nodiscard]] const source_buffer_line_t* line_by_number(size_t line) const;

        [[nodiscard]] const source_buffer_line_t* line_by_index(size_t index) const;

        [[nodiscard]] std::string_view make_slice(size_t offset, size_t length) const;

    private:
        void dump_lines();

        void index_lines(result_t& r);

    private:
        path_t _path{};
        char* _buffer{};
        reader_t* _reader{};
        size_t _buffer_size{};
        memory::allocator_t* _allocator;
        std::map<size_t, source_buffer_line_t*> _lines_by_number {};
        std::map<
            source_buffer_range_t,
            source_buffer_line_t,
            source_buffer_range_compare_t> _lines_by_index_range {};
    };

}