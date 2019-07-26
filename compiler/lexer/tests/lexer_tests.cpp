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
#include <compiler/lexer/lexer.h>

namespace basecode {

    using namespace std::literals;

    TEST_CASE("lexer_t::tokenize detects radix prefixed numbers") {
        compiler::result_t r{};
        compiler::utf8::source_buffer_t buffer(0);

        const std::string source =
            "$2334;\n"
            "%1111_0000_1111_0101;\n"
            "@777;\n"
            "3444;\n";

        REQUIRE(buffer.load(r, source));

        compiler::lexer::lexer_t lexer(&buffer);
        REQUIRE(lexer.tokenize(r));
    }

}