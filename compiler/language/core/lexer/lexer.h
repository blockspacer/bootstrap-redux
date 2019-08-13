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

#include <utility>
#include <compiler/types.h>
#include <compiler/memory/pool.h>
#include <compiler/data/hash_table.h>
#include <compiler/workspace/session.h>
#include <compiler/language/lexeme_trie.h>
#include "token.h"

namespace basecode::compiler::language::core::lexer {

    class lexer_t;

    using lexeme_tokenizer_t = std::function<bool (
        lexer_t*,
        result_t&,
        entity_list_t&)>;

    struct lexeme_t final {
        bool keyword{};
        token_type_t type{};
        lexeme_tokenizer_t tokenizer{};
    };

    using lexer_trie_t = lexeme_trie_t<lexeme_t>;

    using entity_maker_t = std::function<entt::entity (
        token_type_t type,
        size_t,
        size_t,
        std::string_view)>;

    class lexer_t final {
    public:
        lexer_t(
            workspace::session_t& workspace,
            utf8::source_buffer_t& buffer);

        ~lexer_t();

        lexer_trie_t* lexemes();

        bool tokenize(result_t& r, entity_list_t& entities);

    private:
        bool identifier(result_t& r, entity_list_t& entities);

        bool line_comment(result_t& r, entity_list_t& entities);

        bool block_comment(result_t& r, entity_list_t& entities);

        bool string_literal(result_t& r, entity_list_t& entities);

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
                bool imaginary,
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

        bool scan_dec_digits(result_t& r, size_t start_pos, number_type_t& type);

    private:
        lexer_trie_t* _lexemes{};
        utf8::source_buffer_t& _buffer;
        workspace::session_t& _session;
        const utf8::source_buffer_line_t* _source_line{};
    };

}
