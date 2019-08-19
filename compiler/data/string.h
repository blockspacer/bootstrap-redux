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

#include <cctype>
#include <cstdint>
#include <algorithm>
#include <string_view>
#include <compiler/memory/system.h>

namespace basecode::compiler::data {

    class string_t final {
    public:
        explicit string_t(
                memory::allocator_t* allocator = memory::default_allocator());

        explicit string_t(
                const char* value,
                memory::allocator_t* allocator = memory::default_allocator());

        string_t(const string_t& other);

        string_t(string_t&& other) noexcept;

        ~string_t();

        void pop();

        char* end();

        void trim();

        void clear();

        char& back();

        char* rend();

        char* begin();

        void shrink();

        char* rbegin();

        void to_lower();

        void to_upper();

        void left_trim();

        void right_trim();

        char* erase(const char* it);

        char& operator[](size_t index);

        void resize(uint32_t new_size);

        [[nodiscard]] bool empty() const;

        [[nodiscard]] uint32_t size() const;

        void reserve(uint32_t new_capacity);

        [[nodiscard]] const char* end() const;

        [[nodiscard]] const char* rend() const;

        [[nodiscard]] const char& back() const;

        [[nodiscard]] uint32_t capacity() const;

        [[nodiscard]] const char* begin() const;

        bool operator==(const char* other) const {
            return std::memcmp(_data, other, _size) == 0;
        }

        const char& operator[](size_t index) const;

        string_t& operator=(const string_t& other);

        char* insert(const char* it, const char& v);

        string_t& operator=(string_t&& other) noexcept;

        char* erase(const char* it_begin, const char* it_end);

        bool operator==(const std::string_view& other) const {
            return std::memcmp(_data, other.data(), _size) == 0;
        }

    private:
        void grow(uint32_t min_capacity = 32);

        void set_capacity(uint32_t new_capacity);

    private:
        bool _moved{};
        char* _data{};
        uint32_t _size{};
        uint32_t _capacity{};
        memory::allocator_t* _allocator;
    };
}