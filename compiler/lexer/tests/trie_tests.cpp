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
#include <string_view>
#include <fmt/format.h>
#include <tsl/htrie_map.h>

namespace basecode {

    using namespace std::literals;

    TEST_CASE("htrie_map works the way we want?") {
        tsl::htrie_map<char, uint64_t> map{
            {"false", 1},
            {"for",   2},
            {"from",  3}
        };
        map.burst_threshold(512);

        auto input = "for"sv;

        std::string_view v(input.data(), 1);

        while (true) {
            fmt::print("input: {}\n", v);
            auto range = map.equal_prefix_range(v);
            if (range.first == range.second) {
                break;
            }

            for (auto it = range.first; it != range.second; ++it) {
                if (v.length() < it.key().length())
                    continue;
                fmt::print("it.key() = {}, v = {}\n", it.key(), v);
                if (it.key() == v) {
                    fmt::print("match!\n");
                    break;
                }
            }

            if (v.length() + 1 > input.length())
                break;

            v = std::string_view(input.data(), v.length() + 1);
        }
    }

}