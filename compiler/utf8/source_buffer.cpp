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

#include "source_buffer.h"

namespace basecode::compiler::utf8 {

    source_buffer_t::source_buffer_t(
            id::type_t id,
            bool console_color_enabled) : _id(id) {
        _term_builder.enabled(console_color_enabled);
    }

    bool source_buffer_t::load(
            result_t& r,
            const std::string& buffer) {
        _buffer.clear();
        _lines_by_number.clear();
        _lines_by_index_range.clear();

        std::stringstream stream;
        stream.unsetf(std::ios::skipws);
        stream << buffer << "\n";
        stream.seekg(0, std::ios::beg);

        _buffer.reserve(buffer.length() + 1);
        _buffer.insert(_buffer.begin(),
                       std::istream_iterator<uint8_t>(stream),
                       std::istream_iterator<uint8_t>());

        _reader = std::make_unique<reader_t>(std::string_view(
            reinterpret_cast<const char*>(_buffer.data()),
            _buffer.size()));

        index_lines(r);

        return true;
    }

    bool source_buffer_t::load(
            result_t& r,
            const boost::filesystem::path& path) {
        _path = path;
        _buffer.clear();
        _lines_by_number.clear();
        _lines_by_index_range.clear();

        std::ifstream file(
            _path.string(),
            std::ios::in | std::ios::binary);
        if (file.is_open()) {
            file.unsetf(std::ios::skipws);
            file.seekg(0, std::ios::end);
            const auto file_size = file.tellg();
            file.seekg(0, std::ios::beg);
            _buffer.reserve(static_cast<size_t>(file_size) + 1);
            _buffer.insert(_buffer.begin(),
                           std::istream_iterator<uint8_t>(file),
                           std::istream_iterator<uint8_t>());
            _buffer.push_back('\n');

            _reader = std::make_unique<reader_t>(std::string_view(
                reinterpret_cast<const char*>(_buffer.data()),
                _buffer.size()));

            index_lines(r);
        } else {
            r.error(
                "S001",
                fmt::format("unable to open source file: {}", _path.string()));
        }
        return !r.is_failed();
    }

    void source_buffer_t::error(
            result_t& r,
            const std::string& code,
            const std::string& message,
            const source_location_t& location) const {
        std::stringstream stream;

        const auto number_of_lines = static_cast<int32_t>(_lines_by_number.size());
        const auto target_line = static_cast<int32_t>(location.start.line);
        const auto message_indicator = _term_builder.colorize(
            "^ " + message,
            terminal::colors_t::red);

        auto start_line = static_cast<int32_t>(location.start.line - 4);
        if (start_line < 0)
            start_line = 0;

        auto stop_line = static_cast<int32_t>(location.end.line + 4);
        if (stop_line >= number_of_lines)
            stop_line = number_of_lines;

        for (int32_t i = start_line; i < stop_line; i++) {
            const auto source_line = line_by_number(static_cast<size_t>(i));
            if (source_line == nullptr)
                break;
            const auto source_text = substring(
                source_line->begin,
                source_line->end);
            if (i == target_line) {
                stream << fmt::format("{:04d}: ", i + 1)
                       << _term_builder.colorize_range(
                           source_text,
                           location.start.column,
                           location.end.column,
                           terminal::colors_t::yellow,
                           terminal::colors_t::blue)
                       << "\n"
                       << fmt::format("{}{}",
                                      std::string(6 + location.start.column, ' '),
                                      message_indicator);
            } else {
                stream << fmt::format("{:04d}: ", i + 1)
                       << source_text;
            }

            if (i < static_cast<int32_t>(stop_line - 1))
                stream << "\n";
        }

        if (!_path.empty()) {
            r.error(
                code,
                fmt::format(
                    "({}@{}:{}) {}",
                    _path.filename().string(),
                    location.start.line + 1,
                    location.start.column + 1,
                    message),
                location,
                stream.str());
        } else {
            r.error(
                code,
                fmt::format(
                    "((anonymous source)@{}:{}) {}",
                    location.start.line + 1,
                    location.start.column + 1,
                    message),
                location,
                stream.str());
        }
    }

    void source_buffer_t::push_mark() {
        _reader->push_mark();
    }

    bool source_buffer_t::eof() const {
        return _reader->eof();
    }

    void source_buffer_t::dump_lines() {
        for (size_t i = 0; i < number_of_lines(); i++) {
            auto line = line_by_number(i);
            fmt::print("{}\n", substring(line->begin, line->end));
        }
    }

    size_t source_buffer_t::pop_mark() {
        return _reader->pop_mark();
    }

    size_t source_buffer_t::pos() const {
        return _reader->pos();
    }

    bool source_buffer_t::empty() const {
        return _buffer.empty();
    }

    size_t source_buffer_t::current_mark() {
        return _reader->current_mark();
    }

    size_t source_buffer_t::length() const {
        return _buffer.size();
    }

    id::type_t source_buffer_t::id() const {
        return _id;
    }

    void source_buffer_t::restore_top_mark() {
        _reader->restore_top_mark();
    }

    bool source_buffer_t::seek(size_t index) {
        return _reader->seek(index);
    }

    rune_t source_buffer_t::prev(result_t& r) {
        return _reader->prev(r);
    }

    rune_t source_buffer_t::next(result_t& r) {
        return _reader->next(r);
    }

    const path_t& source_buffer_t::path() const {
        return _path;
    }

    void source_buffer_t::index_lines(result_t& r) {
        uint32_t line = 0;
        uint32_t columns = 0;
        size_t line_start = 0;

        while (true) {
            auto rune = next(r);
            if (rune == rune_invalid) {
                break;
            } else if (rune == rune_bom) {
                rune = next(r);
            }

            const auto end_of_buffer = rune == rune_eof;
            const auto unix_new_line = rune == '\n';

            if (unix_new_line || end_of_buffer) {
                const auto end = end_of_buffer ? _buffer.size() : _reader->pos() - 1;
                const auto it = _lines_by_index_range.insert(std::make_pair(
                    std::make_pair(line_start, end),
                    source_buffer_line_t {
                        .end = end,
                        .begin = line_start,
                        .line = line,
                        .columns = columns
                    }));
                _lines_by_number.insert(std::make_pair(
                    line,
                    &it.first->second));
                line_start = _reader->pos();
                line++;
                columns = 0;
            } else {
                columns++;
            }

            if (rune == rune_eof)
                break;
        }

        seek(0);
    }

    size_t source_buffer_t::number_of_lines() const {
        return _lines_by_number.size();
    }

    uint8_t source_buffer_t::operator[](size_t index) {
        return _buffer[index];
    }

    uint32_t source_buffer_t::column_by_index(size_t index) const {
        auto line = line_by_index(index);
        if (line == nullptr)
            return 0;
        return static_cast<const uint32_t>(index - line->begin);
    }

    std::string_view source_buffer_t::substring(size_t start, size_t end) const {
        const auto length = end - start;
        return std::string_view(
            reinterpret_cast<const char*>(_buffer.data() + start),
            length);
    }

    const source_buffer_line_t* source_buffer_t::line_by_number(size_t line) const {
        auto it = _lines_by_number.find(line);
        if (it == _lines_by_number.end())
            return nullptr;
        return it->second;
    }

    const source_buffer_line_t* source_buffer_t::line_by_index(size_t index) const {
        auto it = _lines_by_index_range.find(std::make_pair(index, index));
        if (it == _lines_by_index_range.end())
            return nullptr;
        return &it->second;
    }

    std::string_view source_buffer_t::make_slice(size_t offset, size_t length) const {
        return std::string_view(
            reinterpret_cast<const char*>(_buffer.data() + offset),
            length);
    }

}