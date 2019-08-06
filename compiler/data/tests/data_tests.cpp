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

#include <catch2/catch.hpp>
#include <compiler/language/core/lexer/lexer.h>

namespace basecode {

    using namespace std::literals;
    using namespace compiler;
    using namespace compiler::data;

    TEST_CASE("array_t with small initializer list") {
        array_t<int32_t> numbers{0, 1, 2, 3, 4, 5, 6, 7, 8};

        REQUIRE(!numbers.empty());
        REQUIRE(numbers.size() == 9);

        int32_t k = 0;
        for (auto n : numbers)
            REQUIRE(n == k++);
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