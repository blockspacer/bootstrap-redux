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
#include <algorithm>
#include <initializer_list>
#include <compiler/hashing/murmur.h>
#include <compiler/memory/allocator.h>
#include "array.h"
#include "hashable.h"

namespace basecode::compiler::data {

    template <typename K, typename V, std::uint32_t InitialSize = 16>
    class hash_table_t final {
        static constexpr uint64_t hash_mask = 0b0011111111111111111111111111111111111111111111111111111111111111;

        struct hash_pair_t final {
            K key;
            V value;
        };

        enum hash_bucket_state_t {
            s_empty,
            s_filled,
            s_removed
        };

        struct hash_bucket_t final {
            size_t hash:62;
            size_t state:2;
        };

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

            hash_pair_t* target_pair{};
            hash_bucket_t* target_bucket{};

            auto found = find_bucket_and_pair_by_matching_key(
                bucket_start_index,
                hash,
                key,
                &target_bucket,
                &target_pair);

            if (!found) return {};
            return target_pair->value;
        }

        bool remove(K key) {
            auto hash = hash_key(key) & hash_mask;
            auto bucket_start_index = hash % _buckets.size();

            hash_pair_t* target_pair{};
            hash_bucket_t* target_bucket{};

            auto found = find_bucket_and_pair_by_matching_key(
                bucket_start_index,
                hash,
                key,
                &target_bucket,
                &target_pair);
            if (found) {
                target_bucket->hash = 0;
                target_bucket->state = hash_bucket_state_t::s_removed;
                --_size;
                return true;
            }

            return false;
        }

        void insert(K key, V value) {
            if (_size * 3 > _buckets.size() * 2)
                rehash(_buckets.size() * 2);

            auto hash = hash_key(key) & hash_mask;
            auto bucket_start_index = hash % _buckets.size();

            hash_pair_t* target_pair{};
            hash_bucket_t* target_bucket{};

            auto found = find_available_bucket_and_pair(
                _buckets,
                _pairs,
                bucket_start_index,
                &target_bucket,
                &target_pair);
            assert(found);

            target_bucket->hash = hash;
            target_bucket->state = hash_bucket_state_t::s_filled;
            target_pair->key = key;
            target_pair->value = value;

            ++_size;
        }

        void reserve(uint32_t new_size) {
            rehash(new_size * 3 / 2);
        }

        [[nodiscard]] bool empty() const {
            return _size == 0;
        }

        [[nodiscard]] size_t size() const {
            return _size;
        }

    private:
        void init() {
            _pairs.resize(InitialSize);
            _buckets.resize(InitialSize);
        }

        void rehash(uint32_t new_bucket_count) {
            new_bucket_count = std::max<uint32_t>(
                std::max<uint32_t>(new_bucket_count, _size),
                InitialSize);

            array_t<hash_pair_t> new_pairs(_allocator);
            new_pairs.resize(new_bucket_count);

            array_t<hash_bucket_t> new_buckets(_allocator);
            new_buckets.resize(new_bucket_count);

            auto i = 0;
            for (auto& bucket : _buckets) {
                if (bucket.state != hash_bucket_state_t::s_filled) {
                    ++i;
                    continue;
                }

                hash_pair_t* target_pair{};
                hash_bucket_t* target_bucket{};
                auto bucket_start_index = bucket.hash % new_bucket_count;

                auto found = find_available_bucket_and_pair(
                    new_buckets,
                    new_pairs,
                    bucket_start_index,
                    &target_bucket,
                    &target_pair);

                assert(found);

                target_bucket->hash = bucket.hash;
                target_bucket->state = hash_bucket_state_t::s_filled;

                auto& original_pair = _pairs[i];
                target_pair->key = std::move(original_pair.key);
                target_pair->value = std::move(original_pair.value);

                ++i;
            }

            _pairs = std::move(new_pairs);
            _buckets = std::move(new_buckets);
        }

        bool find_available_bucket_and_pair(
                array_t<hash_bucket_t>& buckets,
                array_t<hash_pair_t>& pairs,
                uint32_t bucket_start_index,
                hash_bucket_t** target_bucket,
                hash_pair_t** target_pair) {
            for (size_t i = bucket_start_index; i < buckets.size(); i++) {
                auto& bucket = buckets[i];
                if (bucket.state != hash_bucket_state_t::s_filled) {
                    *target_bucket = &bucket;
                    *target_pair = &pairs[i];
                    return true;
                }
            }

            for (size_t i = 0; i < bucket_start_index; i++) {
                auto& bucket = buckets[i];
                if (bucket.state != hash_bucket_state_t::s_filled) {
                    *target_bucket = &bucket;
                    *target_pair = &pairs[i];
                    return true;
                }
            }

            return false;
        }

        bool find_bucket_and_pair_by_matching_key(
                uint32_t bucket_start_index,
                size_t hash,
                K key,
                hash_bucket_t** target_bucket,
                hash_pair_t** target_pair) {
            for (size_t i = bucket_start_index; i < _buckets.size(); i++) {
                auto& bucket = _buckets[i];
                switch (bucket.state) {
                    case hash_bucket_state_t::s_empty:
                        return false;
                    case hash_bucket_state_t::s_filled: {
                        if (bucket.hash == hash) {
                            auto& pair = _pairs[i];
                            if (pair.key == key) {
                                *target_pair = &pair;
                                *target_bucket = &bucket;
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
                auto& bucket = _buckets[i];
                switch (bucket.state) {
                    case hash_bucket_state_t::s_empty:
                        return false;
                    case hash_bucket_state_t::s_filled: {
                        if (bucket.hash == hash) {
                            auto& pair = _pairs[i];
                            if (pair.key == key) {
                                *target_pair = &pair;
                                *target_bucket = &bucket;
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

        void insert(std::initializer_list<std::pair<K, V>> elements) {
            reserve(elements.size());
            for (auto e : elements)
                insert(e.first, e.second);
        }

    private:
        size_t _size{};
        array_t<hash_pair_t> _pairs;
        memory::allocator_t* _allocator;
        array_t<hash_bucket_t> _buckets;
    };

}