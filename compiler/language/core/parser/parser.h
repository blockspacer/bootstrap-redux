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

#include <boost/any.hpp>
#include <compiler/types.h>
#include <compiler/workspace/session.h>
#include <compiler/language/core/ast/ast.h>

namespace basecode::compiler::language::core::parser {

    using namespace std::literals;

    // prefix
    using nud_callback_t = std::function<entt::entity(result_t&)>;

    // infix
    using led_callback_t = std::function<entt::entity(result_t&, entt::entity)>;

    struct symbol_t final {
        int32_t lbp{};
        nud_callback_t nud{};
        led_callback_t led{};
        std::string_view token{};
    };

    ///////////////////////////////////////////////////////////////////////////

    struct token_info_t final {
        symbol_t* atom{};
        entt::entity token{};
        source_location_t loc{};
    };

    class parser_t final {
    public:
        parser_t(
            workspace::session_t& session,
            utf8::source_buffer_t& buffer,
            entity_list_t tokens);

        bool initialize(result_t& r);

        entt::entity expression(result_t& r, int32_t rbp = 0);

    private:
        bool apply(result_t& r);

        bool advance(std::string_view expected = {});

        [[nodiscard]] bool token(result_t& r, token_info_t& info) const;

    private:
        void symbol(std::string_view token, int32_t bp = 0);

        void constant(std::string_view token, boost::any value);

        void prefix(std::string_view token, const nud_callback_t& nud = {});

        void infix(std::string_view token, int32_t bp, const led_callback_t& led = {});

        void infix_right(std::string_view token, int32_t bp, const led_callback_t& led = {});

    private:
        size_t _token_index{};
        entity_list_t _tokens;
        workspace::session_t& _session;
        utf8::source_buffer_t& _buffer;
    };

}
