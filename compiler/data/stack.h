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

#include "array.h"

namespace basecode::compiler::data {

    template <typename T, std::uint32_t Initial_Capacity = 16>
    class stack_t final {
    public:
        explicit stack_t(
                memory::allocator_t* allocator = memory::default_allocator()) : _allocator(allocator),
                                                                                _values(allocator) {
            assert(_allocator);
        }

        stack_t(
                std::initializer_list<T> elements,
                memory::allocator_t* allocator = memory::default_allocator()) : _allocator(allocator) {
            assert(_allocator);
            insert(elements);
        }

        stack_t(const stack_t& other) : _allocator(other._allocator),
                                        _values(other._values) {
        }

        T* top() {
            return _values.empty() ? nullptr : &_values[_values.size() - 1];
        }

        void pop() {
            _values.pop();
        }

        void clear() {
            _values.clear();
        }

        void push(T& value) {
            _values.add(value);
        }

        const T* top() const {
            return _values.empty() ? nullptr : &_values[_values.size() - 1];
        }

        void push(const T& value) {
            _values.add(value);
        }

        void resize(uint32_t new_size) {
            _values.resize(new_size);
        }

        [[nodiscard]] bool empty() const {
            return _values.size() == 0;
        }

        void reserve(uint32_t new_capacity) {
            _values.reserve(new_capacity);
        }

        [[nodiscard]] uint32_t depth() const {
            return _values.size();
        }

        [[nodiscard]] uint32_t capacity() const {
            return _values.capacity();
        }

    private:
        void insert(std::initializer_list<T> elements) {
            _values.reserve(elements.size());
            for (const auto& e : elements)
                _values.add(e);
        }

    private:
        memory::allocator_t* _allocator;
        array_t<T, Initial_Capacity> _values;
    };

}