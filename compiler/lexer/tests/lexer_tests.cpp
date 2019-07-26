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
#include <compiler/formatters/formatters.h>

namespace basecode {

    using namespace std::literals;

    TEST_CASE("lexer_t::tokenize detects radix prefixed numbers") {
        compiler::result_t r{};
        compiler::workspace_t workspace{};
        compiler::utf8::source_buffer_t buffer(0);

        const std::string source =
            "$2334;\n"
            "%1111_0000_1111_0101;\n"
            "@777;\n"
            "3444;\n";

        REQUIRE(buffer.load(r, source));

        compiler::entity_list_t tokens{};
        compiler::lexer::lexer_t lexer(&workspace, &buffer);
        REQUIRE(lexer.tokenize(r, tokens));

        for (auto entity : tokens) {
            const auto& token = workspace.registry.get<compiler::lexer::token_t>(entity);
            const auto& source_location = workspace.registry.get<compiler::source_location_t>(entity);
            fmt::print("token = {}", token);
            if (workspace.registry.has<compiler::lexer::number_token_t>(entity)) {
                const auto& number_token = workspace.registry.get<compiler::lexer::number_token_t>(entity);
                fmt::print(", number_token = {}", number_token);
            }
            fmt::print(", location = {}\n", source_location);
        }
    }

}