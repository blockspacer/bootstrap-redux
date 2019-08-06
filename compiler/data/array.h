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

    template <typename T>
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

        void reset() {
            std::memset(_data, 0, _capacity);
            _size = 0;
        }

        void add(T&& value) {
            const auto value_size = numbers::align(sizeof(T), alignof(T));
            const auto offset = _size * value_size;
            if (offset + value_size > _capacity)
                adjust_capacity();
            uint64_t addr = reinterpret_cast<uint64_t>(_data) + offset;
            auto data = reinterpret_cast<T*>(addr);
            *data = value;
            ++_size;
        }

        T& operator[](size_t index) {
            const auto value_size = numbers::align(sizeof(T), alignof(T));
            const auto offset = index * value_size;
            uint64_t addr = reinterpret_cast<uint64_t>(_data) + offset;
            return *(reinterpret_cast<T*>(addr));
        }

        // N.B. this function does not shrink the array if element_count < _size
        void resize(size_t element_count) {
            reserve(element_count);
            for (size_t i = 0; i < element_count; i++)
                add(T{});
            _size = element_count;
        }

        void reserve(size_t element_count) {
            if (element_count == 0) return;

            const auto value_size = numbers::align(sizeof(T), alignof(T));
            auto new_capacity = numbers::next_power_of_two(value_size * element_count);
            if (new_capacity > _capacity) {
                adjust_capacity(new_capacity);
            }
        }

        [[nodiscard]] size_t size() const {
            return _size;
        }

        [[nodiscard]] size_t capacity() const {
            return _capacity;
        }

        array_t& operator = (array_t&& other) noexcept {
            // XXX: it would be nice to assert that the other type is equivalent to our array type
            //static_assert(
            //    std::is_same<decltype(this), decltype(other)>::value,
            //    "arrays must be of equivalent type");

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
            for (auto& e : elements) add(e);
        }

        void adjust_capacity(std::optional<size_t> target_capacity = {}) {
            const auto value_size = numbers::align(sizeof(T), alignof(T));
            if (!_data) {
                _size = 0;
                _capacity = target_capacity ?
                    *target_capacity :
                    numbers::next_power_of_two(value_size * 16);
                _data = static_cast<T*>(_allocator->allocate(_capacity));
            } else {
                auto new_capacity = target_capacity ?
                    *target_capacity :
                    numbers::next_power_of_two(_capacity);
                auto new_data = _allocator->allocate(new_capacity, alignof(T));
                std::memcpy(new_data, _data, _capacity);
                _allocator->deallocate(_data);
                _capacity = new_capacity;
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