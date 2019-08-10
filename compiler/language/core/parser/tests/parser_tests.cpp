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
#include <compiler/language/core/lexer/lexer.h>
#include <compiler/language/core/parser/parser.h>

namespace basecode {

    using namespace std::literals;
    using namespace compiler;
    using namespace compiler::language::core;

    TEST_CASE("lexer_t::tokenize identifiers") {
        result_t r{};
        workspace::session_options_t options{};
        workspace::session_t session(options);
        utf8::source_buffer_t buffer(options.allocator);

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

        parser::parser_t parser(session, buffer, tokens);
        REQUIRE(parser.initialize(r));
    }

}
