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

#include <cstdint>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <compiler/memory/system.h>
#include <compiler/hashing/murmur.h>
#include <compiler/memory/allocator.h>
#include <compiler/memory/temp_allocator.h>
#include <compiler/memory/block_allocator.h>
#include "array.h"

namespace basecode::compiler::data {

    template <typename K> uint64_t hash_key(K key) {
        return hashing::murmur::hash64(&key, sizeof(key));
    }

    template <typename K, typename V, std::uint32_t InitialSize = 16>
    class hash_table_t final {
    public:
        hash_table_t(
                std::initializer_list<std::pair<K, V>> elements,
                memory::allocator_t* allocator) : _pairs(allocator),
                                                  _allocator(allocator),
                                                  _buckets(allocator) {
            init();
            insert(elements);
        }

        explicit hash_table_t(memory::allocator_t* allocator) : _pairs(allocator),
                                                                _allocator(allocator),
                                                                _buckets(allocator) {
            init();
        }

        void reset() {
            _size = 0;
            _pairs.reset();
            _buckets.reset();
        }

        V find(K key) {
            size_t hash = hash_key(key) & hash_mask;
            auto bucket_start_index = hash % _buckets.size();

            for (size_t i = bucket_start_index; i < _buckets.size(); i++) {
                auto bucket = &_buckets[i];
                switch (bucket->state) {
                    case hash_bucket_state_t::empty:
                        return {};
                    case hash_bucket_state_t::filled: {
                        if (bucket->hash == hash) {
                            auto pair = &_pairs[i];
                            if (pair->key == key)
                                return pair->value;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            for (size_t i = 0; i < bucket_start_index; i++) {
                auto bucket = &_buckets[i];
                switch (bucket->state) {
                    case hash_bucket_state_t::empty: return {};
                    case hash_bucket_state_t::filled: {
                        if (bucket->hash == hash) {
                            auto pair = &_pairs[i];
                            if (pair->key == key)
                                return pair->value;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            return {};
        }

        bool remove(K key) {
            auto hash = hash_key(key) & hash_mask;
            auto bucket_start_index = hash % _buckets.size();

            for (size_t i = bucket_start_index; i < _buckets.size(); i++) {
                auto bucket = &_buckets[i];
                switch (bucket->state) {
                    case hash_bucket_state_t::empty:
                        return false;
                    case hash_bucket_state_t::filled: {
                        if (bucket->hash == hash) {
                            auto pair = &_pairs[i];
                            if (pair->key == key) {
                                bucket->hash = 0;
                                bucket->state = hash_bucket_state_t::removed;
                                --_size;
                                return true;
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            for (size_t i = 0; i < bucket_start_index; i++) {
                auto bucket = &_buckets[i];
                switch (bucket->state) {
                    case hash_bucket_state_t::empty:
                        return false;
                    case hash_bucket_state_t::filled: {
                        if (bucket->hash == hash) {
                            auto pair = &_pairs[i];
                            if (pair->key == key) {
                                bucket->hash = 0;
                                bucket->state = hash_bucket_state_t::removed;
                                --_size;
                                return true;
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            return false;
        }

        void insert(K key, V value) {
            if (_size * 3 > _buckets.size() * 2) {
                rehash(_buckets.size() * 2);
            }

            auto hash = hash_key(key) & hash_mask;

            pair_t* target_pair = nullptr;
            hash_bucket_t* target_bucket = nullptr;
            auto bucket_start_index = hash % _buckets.size();

            for (size_t i = bucket_start_index; i < _buckets.size(); i++) {
                auto bucket = &_buckets[i];
                if (bucket->state != hash_bucket_state_t::filled) {
                    target_bucket = bucket;
                    target_pair = &_pairs[i];
                    break;
                }
            }

            if (!target_bucket) {
                for (size_t i = 0; i < bucket_start_index; i++) {
                    auto bucket = &_buckets[i];
                    if (bucket->state != hash_bucket_state_t::filled) {
                        target_bucket = bucket;
                        target_pair = &_pairs[i];
                        break;
                    }

                }
            }

            assert(target_bucket);
            assert(target_pair);

            target_bucket->hash = hash;
            target_bucket->state = hash_bucket_state_t::filled;
            target_pair->key = key;
            target_pair->value = value;

            ++_size;
        }

        void reserve(size_t new_size) {
            rehash(new_size * 3 / 2);
        }

        [[nodiscard]] size_t size() const {
            return _size;
        }

    private:
        static constexpr uint64_t hash_mask = 0b0011111111111111111111111111111111111111111111111111111111111111;

        enum hash_bucket_state_t {
            empty,
            filled,
            removed
        };

        struct hash_bucket_t final {
            size_t hash:62;
            size_t state:2;
        };


        struct pair_t final {
            K key;
            V value;
        };

        void init() {
            _pairs.resize(InitialSize);
            _buckets.resize(InitialSize);
        }

        void rehash(size_t new_bucket_count) {
            new_bucket_count = std::max<uint32_t>(
                std::max(new_bucket_count, _size),
                InitialSize);

            array_t<pair_t> new_pairs(_allocator);
            new_pairs.reserve(new_bucket_count);

            array_t<hash_bucket_t> new_buckets(_allocator);
            new_buckets.reserve(new_bucket_count);

            const auto buckets_size = _buckets.size();
            for (size_t i = 0; i < buckets_size; i++) {
                auto bucket = &_buckets[i];
                if (bucket->state == hash_bucket_state_t::filled)
                    continue;

                pair_t* target_pair = nullptr;
                hash_bucket_t* target_bucket = nullptr;
                auto bucket_start_index = bucket->hash % new_bucket_count;

                for (size_t j = bucket_start_index; j < new_bucket_count; j++) {
                    auto new_bucket = &new_buckets[j];
                    if (new_bucket->state != hash_bucket_state_t::filled) {
                        target_bucket = new_bucket;
                        target_pair = &new_pairs[j];
                        break;
                    }
                }

                if (!target_bucket) {
                    for (size_t j = 0; j < bucket_start_index; j++) {
                        auto new_bucket = &new_buckets[j];
                        if (new_bucket->state != hash_bucket_state_t::filled) {
                            target_bucket = new_bucket;
                            target_pair = &new_pairs[j];
                            break;
                        }

                    }
                }

                assert(target_pair);
                assert(target_bucket);

                target_bucket->hash = bucket->hash;
                target_bucket->state = hash_bucket_state_t::filled;
                pair_t* original_pair = &_pairs[i];
                target_pair->key = std::move(original_pair->key);
                target_pair->value = std::move(original_pair->value);
            }

            _pairs = std::move(new_pairs);
            _buckets = std::move(new_buckets);
        }

        void insert(std::initializer_list<std::pair<K, V>> elements) {
            reserve(elements.size());
            for (auto e : elements) insert(e.first, e.second);
        }

    private:
        size_t _size{};
        array_t<pair_t> _pairs;
        memory::allocator_t* _allocator;
        array_t<hash_bucket_t> _buckets;
    };

}