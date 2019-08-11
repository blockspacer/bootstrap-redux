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

#include <functional>
#include <initializer_list>
#include <compiler/memory/allocator.h>

namespace basecode::compiler::data {

    template <typename K>
    struct node_t final {
        K key;
        node_t* left{};
        node_t* right{};
    };

    template <typename K, typename Comparator = std::less<K>>
    class bst_t final {
    public:
        using bst_walk_callback_t = std::function<void (node_t<K>*)>;

        explicit bst_t(memory::allocator_t* allocator = memory::default_scratch_allocator()) : _allocator(allocator) {
            assert(_allocator);
        }

        bst_t(
                std::initializer_list<K> elements,
                memory::allocator_t* allocator = memory::default_scratch_allocator()) : _allocator(allocator) {
            assert(_allocator);
            insert(elements);
        }

        ~bst_t() {
            auto i = 0;
            node_t<K>* to_free[_size];
            walk(_root, [&](auto node) {
                to_free[i++] = node;
            });
            to_free[i] = _root;

            for (auto node : to_free)
                _allocator->deallocate(node);
        }

        void walk(
                node_t<K>* root,
                const bst_walk_callback_t& callback) {
            if (root == nullptr) return;
            walk(root->left, callback);
            callback(root);
            walk(root->right, callback);
        }

        node_t<K>* root() {
            return _root;
        }

        node_t<K>* insert(K key) {
            return insert(_root, key);
        }

        node_t<K>* search(K key) {
            return search(_root, key);
        }

        [[nodiscard]] bool empty() const {
            return _size == 0;
        }

        node_t<K>* search(node_t<K>* root, K key) {
            if (root == nullptr || root->key == key)
                return root;

            if (_comparator(root->key, key))
                return search(root->right, key);

            return search(root->left, key);
        }

        node_t<K>* remove(node_t<K>* root, K key) {
            if (root == nullptr)
                return nullptr;

            if (root->key > key) {
                root->left = remove(root->left, key);
                return root;
            } else if (_comparator(root->key, key)) {
                root->right = remove(root->right, key);
                return root;
            }

            if (root->left == nullptr) {
                auto temp = root->right;
                _allocator->deallocate(root);
                return temp;
            } else if (root->right == nullptr) {
                auto temp = root->left;
                _allocator->deallocate(root);
                return temp;
            } else {
                auto succ_parent = root->right;

                auto succ = root->right;
                while (succ->left) {
                    succ_parent = succ;
                    succ = succ->left;
                }

                succ_parent->left = succ->right;
                root->key = succ->key;
            }
        }

        node_t<K>* insert(node_t<K>* node, K key) {
            if (node == nullptr)
                return make_node(key);

            if (_comparator(key, node->key))
                node->left = insert(node->left, key);
            else if (key > node->key)
                node->right = insert(node->right, key);

            return node;
        }

        [[nodiscard]] uint32_t size() const {
            return _size;
        }
    private:

        node_t<K>* make_node(K key) {
            auto mem = _allocator->allocate(
                sizeof(node_t<K>),
                alignof(node_t<K>));

            auto node = new (mem) node_t<K>();
            node->key = key;

            _size++;

            if (!_root)
                _root = node;

            return node;
        }

        void insert(std::initializer_list<K> elements) {
            node_t<K>* root = nullptr;
            for (const auto& k : elements)
                root = insert(root, k);
        }

    private:
        uint32_t _size{};
        node_t<K>* _root{};
        Comparator _comparator{};
        memory::allocator_t* _allocator;
    };

}