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
#include <tsl/htrie_map.h>
#include <compiler/types.h>
#include <compiler/memory/pool.h>
#include <compiler/utf8/source_buffer.h>
#include "token.h"

namespace basecode::compiler::lexer {

    class lexer_t;

    using lexeme_tokenizer_t = std::function<bool (
        lexer_t*,
        result_t&,
        entity_list_t&)>;

    struct lexeme_t final {
        token_type_t type{};
        lexeme_tokenizer_t tokenizer{};
    };

    struct trie_tree_node_t;

    struct trie_node_t final {
        lexeme_t* data{};
        trie_tree_node_t* tree{};
    };

    struct trie_tree_node_t final {
        std::unordered_map<utf8::rune_t, trie_node_t*, utf8::rune_hash_t> children{};
    };

    class trie_t final {
    public:
        trie_t(std::initializer_list<std::pair<std::string_view, lexeme_t>> elements);

        void insert(std::string_view key, lexeme_t* value);

        trie_node_t* find(trie_node_t* node, const utf8::rune_t& rune);

        trie_t& operator =(std::initializer_list<std::pair<std::string_view, lexeme_t>> elements) {
            insert(elements);
            return *this;
        }

    private:
        void insert(std::initializer_list<std::pair<std::string_view, lexeme_t>> elements);

    private:
        trie_tree_node_t _tree_root{};
        trie_node_t _root{nullptr, &_tree_root};
        memory::pool_t<lexeme_t> _lexeme_storage{256};
        memory::pool_t<trie_node_t> _node_storage{256};
        memory::pool_t<trie_tree_node_t> _tree_node_storage{256};
    };

    ///////////////////////////////////////////////////////////////////////////

    using entity_maker_t = std::function<entt::entity (
        token_type_t type,
        size_t,
        size_t,
        std::string_view)>;

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
        static trie_t s_lexemes;

        workspace_t& _workspace;
        utf8::source_buffer_t& _buffer;
    };

}
