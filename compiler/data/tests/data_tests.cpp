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

#include <fmt/format.h>
#include <catch2/catch.hpp>
#include <compiler/data/set.h>
#include <compiler/data/array.h>

namespace basecode {

    using namespace std::literals;
    using namespace compiler;
    using namespace compiler::data;

    TEST_CASE("set_t with small initializer list") {
        set_t<int32_t> set_of_integers{0, 1, 2, 3, 4, 5, 6, 7, 8};

        REQUIRE(!set_of_integers.empty());
        REQUIRE(set_of_integers.size() == 9);

        REQUIRE(set_of_integers.count(1) == 1);
        REQUIRE(set_of_integers.has(6));
        REQUIRE(!set_of_integers.has(10));

        REQUIRE(set_of_integers.remove(3));
        REQUIRE(!set_of_integers.has(3));
        REQUIRE(set_of_integers.count(3) == 0);

        for (size_t i = 10; i < 100; i++)
            set_of_integers.insert(i);

        REQUIRE(set_of_integers.size() == 98);

        auto elements = set_of_integers.elements();
        std::sort(
            std::begin(elements),
            std::end(elements),
            [](auto lhs, auto rhs) {
                return lhs < rhs;
            });

        REQUIRE(elements.size() == 98);
    }

    TEST_CASE("array_t with small initializer list") {
        array_t<int32_t> numbers{0, 1, 2, 3, 4, 5, 6, 7, 8};

        REQUIRE(!numbers.empty());
        REQUIRE(numbers.size() == 9);

        int32_t k = 0;
        for (auto n : numbers)
            REQUIRE(n == k++);
    }

    TEST_CASE("array_t insert") {
        array_t<int32_t> numbers{0, 1, 2, 3, 4, 5, 6, 7, 8};

        REQUIRE(!numbers.empty());
        REQUIRE(numbers.size() == 9);

        auto v = *numbers.insert(std::begin(numbers) + 3, 30);
        REQUIRE(v == 30);

        REQUIRE(numbers.size() == 10);

        int32_t k = 0;
        int32_t j = 0;
        for (auto n : numbers) {
            if (j == 3) {
                REQUIRE(n == 30);
            } else {
                REQUIRE(n == k++);
            }
            ++j;
        }
    }

    TEST_CASE("array_t erase") {
        array_t<int32_t> numbers{0, 1, 2, 3, 4, 5, 6, 7, 8};

        REQUIRE(!numbers.empty());
        REQUIRE(numbers.size() == 9);

        auto v = *numbers.erase(std::begin(numbers) + 3);
        REQUIRE(v == 4);

        REQUIRE(numbers.size() == 8);

        int32_t k = 0;
        int32_t j = 0;
        for (auto n : numbers) {
            if (j == 3) {
                REQUIRE(n == 4);
                k = n + 1;
            } else {
                REQUIRE(n == k++);
            }
            ++j;
        }
    }

    TEST_CASE("array_t reserve space; fill") {
        array_t<int32_t> numbers;
        numbers.reserve(4096);

        REQUIRE(numbers.empty());
        REQUIRE(numbers.size() == 0);
        REQUIRE(numbers.capacity() == 4096);

        for (size_t i = 0; i < 4096; i++)
            numbers.add(i);

        REQUIRE(!numbers.empty());
        REQUIRE(numbers.size() == 4096);
        REQUIRE(numbers.capacity() == 4096);

        std::sort(
            std::begin(numbers),
            std::end(numbers),
            [](auto lhs, auto rhs) {
                return lhs > rhs;
            });

        for (size_t i = 0; i < 4096; i++)
            REQUIRE(numbers[i] == 4095 - i);
    }

}