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
#include <compiler/defer.h>
#include <compiler/formatters/formatters.h>
#include <compiler/language/basecode/lexer/lexer.h>

namespace basecode {

    using namespace std::literals;

    [[maybe_unused]] static void format_tokens(
            basecode::compiler::workspace_t& workspace,
            compiler::entity_list_t& tokens) {
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

    TEST_CASE("lexer_t::tokenize detects radix prefixed numbers") {
        compiler::result_t r{};
        compiler::workspace_t workspace{};
        compiler::utf8::source_buffer_t buffer(0);

        defer(fmt::print("{}", r));

        const std::string source =
            "$2334;\n"
            "$80;\n"
            "%1111_0000_1111_0101;\n"
            "@777;\n"
            "32;\n"
            "3444;\n"
            "-512;\n"
            "131072;\n"
            "3.1459;\n"
            "-3.1459;\n"
            "6.022e23;\n"
            "6.022E23;\n"
            "1.6e-35;\n"
            "1.6e-35i;\n";

        REQUIRE(buffer.load(r, source));

        compiler::entity_list_t tokens{};
        compiler::lexer::lexer_t lexer(workspace, buffer);
        REQUIRE(lexer.tokenize(r, tokens));
        REQUIRE(!r.is_failed());
        REQUIRE(r.messages().empty());
    }

    TEST_CASE("lexer_t::tokenize detects literals") {
        compiler::result_t r{};
        compiler::workspace_t workspace{};
        compiler::utf8::source_buffer_t buffer(0);

        defer(fmt::print("{}", r));

        const std::string source =
            "// this is a line comment\n"
            "-- this is a another type of line comment\n"
            "/* this is a block comment with nested block comments\n"
            "           / this is tokenized \\ correctly.\n"
            "   /* food, hungry */\n"
            "   /* a second nested comment at this level\n"
            "       /* another level nested comment */\n"
            "       /* nesting is fun! */\n"
            "   */\n"
            "*/\n"
            "{{\n"
            "   .local foo\n"
            "   move.qw foo, 256\n"
            "   shl.w   foo, foo, 8\n"
            "}}\n"
            "\n"
            "\n"
            "\"this is a string literal \\n \\t \\b!\";\n"
            "'A';\n"
            "'\\a';\n"
            "'\\b';\n"
            "'\\n';\n"
            "'\\\\';\n"
            "'\\xa9';\n"
            "'\\ufffe';\n"
            "'\\Ueeffaaff';\n"
            "'\\777';\n"
            ;

        REQUIRE(buffer.load(r, source));

        compiler::entity_list_t tokens{};
        compiler::lexer::lexer_t lexer(workspace, buffer);
        REQUIRE(lexer.tokenize(r, tokens));
        REQUIRE(!r.is_failed());
        REQUIRE(r.messages().empty());
    }

    TEST_CASE("lexer_t::tokenize detects identifiers") {
        compiler::result_t r{};
        compiler::workspace_t workspace{};
        compiler::utf8::source_buffer_t buffer(0);

        defer(fmt::print("{}", r));

        const std::string source =
            "@no_fold\n"
            "foo: u8 := 33;\n"
            "bar := foo * 16;\n"
            "print(\"bar := {bar}\\n\");\n"
            "#type foo;\n"
            ;

        REQUIRE(buffer.load(r, source));

        compiler::entity_list_t tokens{};
        compiler::lexer::lexer_t lexer(workspace, buffer);
        REQUIRE(lexer.tokenize(r, tokens));
        REQUIRE(!r.is_failed());
        REQUIRE(r.messages().empty());
    }

    TEST_CASE("lexer_t::tokenize keywords don't match inside identifiers") {
        compiler::result_t r{};
        compiler::workspace_t workspace{};
        compiler::utf8::source_buffer_t buffer(0);

        defer(fmt::print("{}", r));

        const std::string source =
            "continueif: bool := false;\n"
            "if_unexpected := if continueif break true;\n"
            ;
            
        REQUIRE(buffer.load(r, source));

        compiler::entity_list_t tokens{};
        compiler::lexer::lexer_t lexer(workspace, buffer);
        REQUIRE(lexer.tokenize(r, tokens));
        REQUIRE(!r.is_failed());
        REQUIRE(r.messages().empty());
    }

    TEST_CASE("lexer_t::tokenize identifiers can't start with numbers") {
        compiler::result_t r{};
        compiler::workspace_t workspace{};
        compiler::utf8::source_buffer_t buffer(0);

        defer(fmt::print("{}", r));

        const std::string source =
            "123myVar: u8 := 1;\n"
            ;
            
        REQUIRE(buffer.load(r, source));

        compiler::entity_list_t tokens{};
        compiler::lexer::lexer_t lexer(workspace, buffer);
        REQUIRE(!lexer.tokenize(r, tokens));
        REQUIRE(r.is_failed());
        REQUIRE(r.messages()[0].message() == "((anonymous source)@1:1) unexpected letter immediately after decimal number");
    }

}
