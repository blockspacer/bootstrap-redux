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

#include "string.h"

namespace basecode::compiler::data {

    string_t::string_t(memory::allocator_t* allocator) : _allocator(allocator) {
        assert(_allocator);
    }

    string_t::string_t(const string_t& other) : _allocator(other._allocator) {
        assert(_allocator);
        const auto n = other._size;
        set_capacity(n);
        std::memcpy(_data, other._data, n * sizeof(char));
        _size = n;
    }

    string_t::string_t(string_t&& other) noexcept : _data(other._data),
                                                    _size(other._size),
                                                    _capacity(other._capacity),
                                                    _allocator(other._allocator) {
        other._moved = true;
    }

    string_t::~string_t() {
        if (_moved) return;
        if (_data)
            _allocator->deallocate(_data);
    }

    void string_t::pop() {
        --_size;
    }

    void string_t::trim() {
        left_trim();
        right_trim();
    }

    char* string_t::end() {
        return _data + _size;
    }

    void string_t::clear() {
        resize(0);
    }

    char& string_t::back() {
        return _data[_size - 1];
    }

    char* string_t::rend() {
        return _data;
    }

    void string_t::shrink() {
        set_capacity(_size);
    }

    char* string_t::begin() {
        return _data;
    }

    char* string_t::rbegin() {
        return _data + _size;
    }

    bool string_t::empty() const {
        return _size == 0;
    }

    uint32_t string_t::size() const {
        return _size;
    }

    const char* string_t::end() const {
        return _data + _size;
    }

    const char* string_t::rend() const {
        return _data;
    }

    const char& string_t::back() const {
        return _data[_size - 1];
    }

    uint32_t string_t::capacity() const {
        return _capacity;
    }

    char* string_t::erase(const char* it) {
        const auto offset = it - _data;
        std::memmove(
            _data + offset,
            _data + offset + 1,
            (_size - offset - 1) * sizeof(char));
        _size--;
        return _data + offset;
    }

    char& string_t::operator[](size_t index) {
        return _data[index];
    }

    void string_t::resize(uint32_t new_size) {
        if (new_size > _capacity) grow(new_size);
        _size = new_size;
    }

    void string_t::grow(uint32_t min_capacity) {
        set_capacity(std::max(_capacity * 2 + 8, min_capacity));
    }

    void string_t::reserve(uint32_t new_capacity) {
        if (new_capacity > _capacity)
            set_capacity(new_capacity);
    }

    void string_t::set_capacity(uint32_t new_capacity) {
        if (new_capacity == _capacity) return;

        if (new_capacity < _size)
            resize(new_capacity);

        char* new_data{};
        if (new_capacity > 0) {
            new_data = (char*)_allocator->allocate(
                new_capacity * sizeof(char),
                alignof(char));
            if (_data)
                std::memcpy(new_data, _data, _size * sizeof(char));
        }
        _allocator->deallocate(_data);
        _data = new_data;
        _capacity = new_capacity;
    }

    string_t& string_t::operator=(const string_t& other) {
        auto n = other._size;
        resize(n);
        std::memcpy(_data, other._data, n * sizeof(char));
        _allocator = other._allocator;
        return *this;
    }

    const char& string_t::operator[](size_t index) const {
        return _data[index];
    }

    char* string_t::insert(const char* it, const char& v) {
        const auto offset = it - _data;
        if (_size == _capacity)
            reserve(_size + 1);
        if (offset < _size) {
            std::memmove(
                _data + offset + 1,
                _data + offset,
                (_size - offset) * sizeof(char));
        }
        std::memcpy(&_data[offset], &v, sizeof(char));
        _size++;
        return _data + offset;
    }

    string_t& string_t::operator=(string_t&& other) noexcept {
        assert(_allocator == other._allocator);
        _allocator->deallocate(_data);
        _data = other._data;
        _size = other._size;
        _capacity = other._capacity;
        _allocator = other._allocator;
        other._moved = true;
        return *this;
    }

    char* string_t::erase(const char* it_begin, const char* it_end) {
        const auto count = it_end - it_begin;
        const auto offset = it_begin - _data;
        std::memmove(
            _data + offset,
            _data + offset + count,
            (_size - offset - count) * sizeof(char));
        _size -= count;
        return _data + offset;
    }

    void string_t::left_trim() {
        erase(
            begin(),
            std::find_if(
                begin(),
                end(),
                [](unsigned char c) { return !std::isspace(c); }));
    }

    void string_t::right_trim() {
        erase(
            std::find_if(
                rbegin(),
                rend(),
                [](unsigned char c) { return !std::isspace(c); }),
            end());
    }

    void string_t::to_lower() {
        std::transform(begin(), end(), begin(), ::tolower);
    }

    void string_t::to_upper() {
        std::transform(begin(), end(), begin(), ::toupper);
    }

}