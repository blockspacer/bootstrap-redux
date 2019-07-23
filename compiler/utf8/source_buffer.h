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
#include <compiler/id/pool.h>
#include <boost/filesystem.hpp>
#include <compiler/terminal/stream_builder.h>
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
        size_t end {};
        size_t begin {};
        uint32_t line {};
        uint32_t columns {};
    };

    class source_buffer_t {
    public:
        explicit source_buffer_t(
            id::type_t id,
            bool console_color_enabled = true);

        bool load(
            result_t& r,
            const std::string& buffer);

        bool load(
            result_t& r,
            const path_t& path);

        void error(
            result_t& r,
            const std::string& code,
            const std::string& message,
            const source_location_t& location) const;

        void push_mark();

        size_t pop_mark();

        size_t current_mark();

        bool seek(size_t index);

        void restore_top_mark();

        rune_t next(result_t& r);

        rune_t prev(result_t& r);

        [[nodiscard]] bool eof() const;

        uint8_t operator[](size_t index);

        [[nodiscard]] size_t pos() const;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] id::type_t id() const;

        [[nodiscard]] size_t length() const;

        [[nodiscard]] const path_t& path() const;

        [[nodiscard]] size_t number_of_lines() const;

        [[nodiscard]] uint32_t column_by_index(size_t index) const;

        [[nodiscard]] std::string_view substring(size_t start, size_t end) const;

        [[nodiscard]] const source_buffer_line_t* line_by_number(size_t line) const;

        [[nodiscard]] const source_buffer_line_t* line_by_index(size_t index) const;

        [[nodiscard]] std::string_view make_slice(size_t offset, size_t length) const;

    private:
        void dump_lines();

        void index_lines(result_t& r);

    private:
        path_t _path{};
        id::type_t _id{};
        std::vector<uint8_t> _buffer{};
        std::unique_ptr<reader_t> _reader;
        terminal::stream_builder_t _term_builder{};
        std::map<size_t, source_buffer_line_t*> _lines_by_number {};
        std::map<
            source_buffer_range_t,
            source_buffer_line_t,
            source_buffer_range_compare_t> _lines_by_index_range {};
    };

}