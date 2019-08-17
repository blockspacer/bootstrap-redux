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
#include <compiler/memory/trace_allocator.h>
#include <compiler/language/core/lexer/lexer.h>

namespace basecode {

    using namespace std::literals;
    using namespace compiler;
    using namespace compiler::language;
    using namespace compiler::language::core;

    [[maybe_unused]] static void format_tokens(
            basecode::compiler::workspace::session_t& session,
            compiler::entity_list_t& tokens) {
        auto& registry = session.registry();
        for (auto entity : tokens) {
            const auto& token = registry.get<lexer::token_t>(entity);
            const auto& source_location = registry.get<compiler::source_location_t>(entity);
            fmt::print("token = {}", token);
            if (registry.has<number_token_t>(entity)) {
                const auto& number_token = registry.get<number_token_t>(entity);
                fmt::print(", number_token = {}", number_token);
            }
            fmt::print(", location = {}\n", source_location);
        }
    }

    TEST_CASE("lexer_t::tokenize number literals") {
//        memory::trace_allocator_t debug_allocator(memory::default_scratch_allocator());
//        workspace::session_options_t options{
//            .allocator = &debug_allocator
//        };
//        workspace::session_options_t options{
//            .allocator = memory::default_allocator()
//        };
        workspace::session_options_t options{};
        workspace::session_t session(options);
        utf8::source_buffer_t buffer(options.allocator);
        result_t r(options.allocator);

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

        REQUIRE(buffer.load(r, session.intern_pool(), source));

        entity_list_t tokens{};
        lexer::lexer_t lexer(session, buffer);
        REQUIRE(lexer.tokenize(r, tokens));
        REQUIRE(!r.is_failed());
        REQUIRE(r.messages().empty());
    }

    TEST_CASE("lexer_t::tokenize comment literals") {
//        memory::trace_allocator_t debug_allocator(memory::default_scratch_allocator());
//        workspace::session_options_t options{
//            .allocator = &debug_allocator
//        };
        workspace::session_options_t options{};
        workspace::session_t session(options);
        utf8::source_buffer_t buffer(options.allocator);
        result_t r(options.allocator);

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
            ;

        REQUIRE(buffer.load(r, session.intern_pool(), source));

        entity_list_t tokens{};
        lexer::lexer_t lexer(session, buffer);
        REQUIRE(lexer.tokenize(r, tokens));
        REQUIRE(!r.is_failed());
        REQUIRE(r.messages().empty());
    }

    TEST_CASE("lexer_t::tokenize string literals") {
//        memory::trace_allocator_t debug_allocator(memory::default_scratch_allocator());
//        workspace::session_options_t options{
//            .allocator = &debug_allocator
//        };
        workspace::session_options_t options{};
        workspace::session_t session(options);
        utf8::source_buffer_t buffer(options.allocator);
        result_t r(options.allocator);

        defer(fmt::print("{}", r));

        const std::string source =
            "{{\n"
            "   .local foo\n"
            "   move.qw foo, 256\n"
            "   shl.w   foo, foo, 8\n"
            "}}\n"
            "\n"
            "\n"
            "\"this is a string literal \\n \\t \\b!\";\n"
            "#rune \"A\";\n"
            "#rune \"\\a\";\n"
            "#rune \"\\b\";\n"
            "#rune \"\\n\";\n"
            "#rune \"\\\\\";\n"
            "#rune \"\\xa9\";\n"
            "#rune \"\\ufffe\";\n"
            "#rune \"\\Ueeffaaff\";\n"
            "#rune \"\\777\";\n"
        ;

        REQUIRE(buffer.load(r, session.intern_pool(), source));

        entity_list_t tokens{};
        lexer::lexer_t lexer(session, buffer);
        REQUIRE(lexer.tokenize(r, tokens));
        REQUIRE(!r.is_failed());
        REQUIRE(r.messages().empty());
    }

    TEST_CASE("lexer_t::tokenize directives") {
//        memory::trace_allocator_t debug_allocator(memory::default_scratch_allocator());
//        workspace::session_options_t options{
//            .allocator = &debug_allocator
//        };
        workspace::session_options_t options{};
        workspace::session_t session(options);
        utf8::source_buffer_t buffer(options.allocator);
        result_t r(options.allocator);

        defer(fmt::print("{}", r));

        const std::string source =
            "#rune \"A\";\n"
            "#assert(1 == 1);\n"
            "#run some_user_proc();\n"
            "#foreign { \n };\n"
            "#run print(\"Hello, Sailor!\");\n"
            "#eval 6 * 6 + 32;\n"
        ;

        REQUIRE(buffer.load(r, session.intern_pool(), source));

        entity_list_t tokens{};
        lexer::lexer_t lexer(session, buffer);
        REQUIRE(lexer.tokenize(r, tokens));
        REQUIRE(!r.is_failed());
        REQUIRE(r.messages().empty());
    }

    TEST_CASE("lexer_t::tokenize attributes") {
//        memory::trace_allocator_t debug_allocator(memory::default_scratch_allocator());
//        workspace::session_options_t options{
//            .allocator = &debug_allocator
//        };
        workspace::session_options_t options{};
        workspace::session_t session(options);
        utf8::source_buffer_t buffer(options.allocator);
        result_t r(options.allocator);

        defer(fmt::print("{}", r));

        const std::string source =
            "@no_fold b := 3 * 3;\n"
            "@library \"libopengl\";\n"
            "@coroutine j :: proc();\n"
        ;

        REQUIRE(buffer.load(r, session.intern_pool(), source));

        entity_list_t tokens{};
        lexer::lexer_t lexer(session, buffer);
        REQUIRE(lexer.tokenize(r, tokens));
        REQUIRE(!r.is_failed());
        REQUIRE(r.messages().empty());
    }

    TEST_CASE("lexer_t::tokenize identifiers") {
//        memory::trace_allocator_t debug_allocator(memory::default_scratch_allocator());
//        workspace::session_options_t options{
//            .allocator = &debug_allocator
//        };
        workspace::session_options_t options{};
        workspace::session_t session(options);
        utf8::source_buffer_t buffer(options.allocator);
        result_t r(options.allocator);

        defer(fmt::print("{}", r));

        const std::string source =
            "@no_fold\n"
            "foo: u8 := 33;\n"
            "bar := foo * 16;\n"
            "print(\"bar := {bar}\\n\");\n"
            "#type foo;\n"
            ;

        REQUIRE(buffer.load(r, session.intern_pool(), source));

        entity_list_t tokens{};
        lexer::lexer_t lexer(session, buffer);
        REQUIRE(lexer.tokenize(r, tokens));
        REQUIRE(!r.is_failed());
        REQUIRE(r.messages().empty());

        format_tokens(session, tokens);
    }

    TEST_CASE("lexer_t::tokenize keywords don't match inside identifiers") {
//        memory::trace_allocator_t debug_allocator(memory::default_scratch_allocator());
//        workspace::session_options_t options{
//            .allocator = &debug_allocator
//        };
        workspace::session_options_t options{};
        workspace::session_t session(options);
        utf8::source_buffer_t buffer(options.allocator);
        result_t r(options.allocator);

        defer(fmt::print("{}", r));

        const std::string source =
            "continueif: bool := false;\n"
            "if_unexpected := if continueif break true;\n"
            ;
            
        REQUIRE(buffer.load(r, session.intern_pool(), source));

        entity_list_t tokens{};
        lexer::lexer_t lexer(session, buffer);
        REQUIRE(lexer.tokenize(r, tokens));
        REQUIRE(!r.is_failed());
        REQUIRE(r.messages().empty());
    }

    TEST_CASE("lexer_t::tokenize identifiers can't start with numbers") {
//        memory::trace_allocator_t debug_allocator(memory::default_scratch_allocator());
//        workspace::session_options_t options{
//            .allocator = &debug_allocator
//        };
        workspace::session_options_t options{};
        workspace::session_t session(options);
        utf8::source_buffer_t buffer(options.allocator);
        result_t r(options.allocator);

        defer(fmt::print("{}", r));

        const std::string source =
            "123myVar: u8 := 1;\n"
            ;
            
        REQUIRE(buffer.load(r, session.intern_pool(), source));

        entity_list_t tokens{};
        lexer::lexer_t lexer(session, buffer);
        REQUIRE(!lexer.tokenize(r, tokens));
        REQUIRE(r.is_failed());
        REQUIRE(r.messages()[0].message() == "((anonymous source)@1:1) unexpected letter immediately after decimal number");
    }

    TEST_CASE("lexer_t::tokenize large sample source") {
//        memory::trace_allocator_t debug_allocator(memory::default_scratch_allocator());
//        workspace::session_options_t options{
//            .allocator = &debug_allocator
//        };
        workspace::session_options_t options{};
        workspace::session_t session(options);
        utf8::source_buffer_t buffer(options.allocator);
        result_t r(options.allocator);

        defer(fmt::print("{}", r));

        path_t file("../tests/sdl.bc");
        REQUIRE(buffer.load(r, session.intern_pool(), file));

        entity_list_t tokens{};
        lexer::lexer_t lexer(session, buffer);
        REQUIRE(lexer.tokenize(r, tokens));
        REQUIRE(!r.is_failed());
    }

}
