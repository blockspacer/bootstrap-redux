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
#include <compiler/data/hash_table.h>
#include <compiler/workspace/session.h>
#include <compiler/memory/object_pool.h>

namespace basecode::compiler::language {

    template <typename V>
    class lexeme_trie_t final {
    public:
        struct tree_node_t;

        struct node_t final {
            V* data{};
            tree_node_t* tree{};
        };

        struct tree_node_t final {
            explicit tree_node_t(memory::allocator_t* allocator) : children(allocator) {}
            data::hash_table_t<utf8::rune_t, node_t*> children;
        };

        explicit lexeme_trie_t(memory::allocator_t* allocator) : _tree_root(allocator),
                                                                 _storage(allocator),
                                                                 _allocator(allocator) {
            assert(_allocator);
        }

        lexeme_trie_t(
                memory::allocator_t* allocator,
                std::initializer_list<std::pair<std::string_view, V>> elements) : _tree_root(allocator),
                                                                                  _storage(allocator),
                                                                                  _allocator(allocator) {
            assert(_allocator);
            insert(elements);
        }

        void insert(std::string_view key, V* value) {
            auto current_node = &_root;

            size_t i = 0;
            const auto key_length = key.length() - 1;
            for (const char c : key) {
                utf8::rune_t rune(c);
                auto& children = current_node->tree->children;
                auto node = children.find(rune);
                if (!node) {
                    current_node = _storage.construct<node_t>();
                    current_node->data = nullptr;
                    current_node->tree = nullptr;
                    if (i <= key_length)
                        current_node->tree = _storage.construct<tree_node_t>(_allocator);
                    children.insert(rune, current_node);
                } else {
                    current_node = node;
                }
                ++i;
            }

            current_node->data = value;
        }

        node_t* find(node_t* node, const utf8::rune_t& rune) {
            auto current_node = node ? node : &_root;
            auto& children = current_node->tree->children;
            auto child_node = children.find(rune);
            if (!child_node)
                return nullptr;
            return child_node;
        }

        lexeme_trie_t& operator =(std::initializer_list<std::pair<std::string_view, V>> elements) {
            insert(elements);
            return *this;
        }

    private:
        void insert(std::initializer_list<std::pair<std::string_view, V>> elements) {
            for (const auto& e : elements) {
                auto lexeme = _storage.construct<V>();
                lexeme->type = e.second.type;
                lexeme->tokenizer = e.second.tokenizer;
                insert(e.first, lexeme);
            }
        }

    private:
        tree_node_t _tree_root;
        memory::object_pool_t _storage;
        memory::allocator_t* _allocator;
        node_t _root{nullptr, &_tree_root};
    };

}
