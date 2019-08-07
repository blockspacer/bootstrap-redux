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

#include <cstdint>
#include <optional>
#include <initializer_list>
#include <compiler/numbers/bytes.h>
#include <compiler/memory/system.h>

namespace basecode::compiler::data {

    template <typename T, std::uint32_t Initial_Capacity = 16>
    class array_t final {
    public:
        using item_type_t = T;

        explicit array_t(
                memory::allocator_t* allocator = memory::default_allocator()) : _allocator(allocator) {
        }

        array_t(
                std::initializer_list<T> elements,
                memory::allocator_t* allocator = memory::default_allocator()) : _allocator(allocator) {
            insert(elements);
        }

        ~array_t() {
            if (!_moved)
                _allocator->deallocate(_data);
        }

        array_t(const array_t& other) : _size(other._size),
                                        _capacity(other._capacity),
                                        _allocator(other._allocator) {
            adjust_capacity(_capacity);
            std::memcpy(_data, other._data, _capacity * sizeof(T));
        }

        T* end() {
            return _data + _size;
        }

        T* begin() {
            return _data;
        }

        void reset() {
            std::memset(_data, 0, _capacity * sizeof(T));
            _size = 0;
        }

        void add(const T& value) {
            if (_size + 1 > _capacity)
                adjust_capacity();
            _data[_size++] = value;
        }

        void add(T&& value) {
            if (_size + 1 > _capacity)
                adjust_capacity();
            _data[_size++] = value;
        }

        const T* end() const {
            return _data + _size;
        }

        const T* begin() const {
            return _data;
        }

        T& operator[](size_t index) {
            return _data[index];
        }

        [[nodiscard]] bool empty() const {
            return _size == 0;
        }

        void resize(uint32_t element_count) {
            reserve(element_count);
            for (size_t i = 0; i < element_count; i++)
                _data[i] = T{};
            _size = element_count;
        }

        [[nodiscard]] uint32_t size() const {
            return _size;
        }

        void reserve(uint32_t element_count) {
            if (element_count == 0
            ||  element_count == _capacity) {
                return;
            }
            adjust_capacity(element_count);
        }

        [[nodiscard]] uint32_t capacity() const {
            return _capacity;
        }

        const T& operator[](size_t index) const {
            return _data[index];
        }

        array_t& operator = (array_t&& other) noexcept {
            assert(_allocator == other._allocator);

            // free any memory we're already using
            _allocator->deallocate(_data);

            _data = other._data;
            _size = other._size;
            _capacity = other._capacity;

            // mark donor as being moved so it doesn't deallocate
            other._moved = true;

            return *this;
        }

    private:
        void insert(std::initializer_list<T> elements) {
            reserve(elements.size());
            for (const auto& e : elements)
                _data[_size++] = e;
        }

        void adjust_capacity(std::optional<uint32_t> target_capacity = {}) {
            if (!_data) {
                _size = 0;
                _capacity = numbers::next_power_of_two(target_capacity ?
                    *target_capacity :
                    Initial_Capacity);
                auto new_size = _capacity * sizeof(T);
                _data = static_cast<T*>(_allocator->allocate(new_size, alignof(T)));
            } else {
                auto old_size = _capacity * sizeof(T);
                _capacity = numbers::next_power_of_two(target_capacity ?
                    *target_capacity :
                    _capacity + 1);
                auto new_size = _capacity * sizeof(T);
                auto new_data = _allocator->allocate(new_size, alignof(T));
                std::memcpy(new_data, _data, old_size);
                _allocator->deallocate(_data);
                _data = static_cast<T*>(new_data);
            }
        }

    private:
        T* _data{};
        bool _moved{};
        uint32_t _size{};
        uint32_t _capacity{};
        memory::allocator_t* _allocator;
    };

}