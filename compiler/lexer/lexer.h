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

#include <tsl/htrie_map.h>
#include <compiler/types.h>
#include <compiler/utf8/source_buffer.h>
#include "token.h"

namespace basecode::compiler::lexer {

    class lexer_t;

    using lexeme_tokenizer_t = std::function<bool (
        lexer_t*,
        result_t&,
        entity_list_t&)>;

    using entity_maker_t = std::function<entt::entity (
        token_type_t type,
        size_t,
        size_t,
        std::string_view)>;

    struct lexeme_t final {
        token_type_t type{};
        lexeme_tokenizer_t tokenizer{};
    };

    class lexer_t final {
    public:
        lexer_t(
            workspace_t& workspace,
            utf8::source_buffer_t& buffer);

        bool tokenize(result_t& r, entity_list_t& entities);

    private:
        bool identifier(result_t& r, entity_list_t& entities);

        bool rune_literal(result_t& r, entity_list_t& entities);

        bool line_comment(result_t& r, entity_list_t& entities);

        bool block_comment(result_t& r, entity_list_t& entities);

        bool string_literal(result_t& r, entity_list_t& entities);

        bool directive_literal(result_t& r, entity_list_t& entities);

        bool annotation_literal(result_t& r, entity_list_t& entities);

        bool dec_number_literal(result_t& r, entity_list_t& entities);

        bool hex_number_literal(result_t& r, entity_list_t& entities);

        bool octal_number_literal(result_t& r, entity_list_t& entities);

        bool block_string_literal(result_t& r, entity_list_t& entities);

        bool binary_number_literal(result_t& r, entity_list_t& entities);

    private:
        bool make_number_token(
                result_t& r, 
                entity_list_t& entities,
                size_t start_pos,
                bool is_signed, 
                uint8_t radix,
                number_type_t type, 
                std::string_view capture, 
                bool check_sign_bit = true);

        bool tokenize_identifier(
            result_t& r,
            entity_list_t& entities,
            token_type_t token_type,
            const entity_maker_t& entity_maker);

        source_location_t make_location(size_t start_pos, size_t end_pos);

    private:
        static tsl::htrie_map<char, lexeme_t> s_lexemes;

        workspace_t& _workspace;
        utf8::source_buffer_t& _buffer;
    };

}
