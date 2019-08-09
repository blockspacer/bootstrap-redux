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

#include <compiler/data/array.h>

namespace basecode::compiler::memory {

    class slab_allocator_t : public allocator_t {
        enum class slab_state_t : uint8_t {
            empty,
            partial,
            full
        };

        struct slab_t final {
            char* chunk{};
            uint32_t size{};
            uint32_t offset{};
            slab_state_t state{};
        };

        struct cache_t final {
            explicit cache_t(
                    memory::allocator_t* allocator,
                    uint8_t id) : id(id),
                                  slabs(allocator) {
            }

            uint8_t id{};
            data::array_t<slab_t> slabs;
        };

    public:
        static inline uint32_t make_size(
                uint8_t cache_id,
                uint32_t size) {
            auto bottom_24_bits = (size & 0b00000000111111111111111111111111);
            auto shifted_id = (cache_id << 24);
            return shifted_id | bottom_24_bits;
        }

        explicit slab_allocator_t(
                allocator_t* backing,
                uint32_t slab_size = 4096) : _slab_size(slab_size),
                                             _backing(backing),
                                             _caches(backing) {
        }

        ~slab_allocator_t() override {
        }

        cache_t* find_cache(uint8_t id) {
            for (auto& cache : _caches) {
                if (cache.id == id)
                    return &cache;
            }
            return nullptr;
        }

        void* allocate(
                uint32_t size,
                uint32_t align = default_align) override {
            auto cache_id = (size & 0b11111111000000000000000000000000) >> 24;
            auto actual_size = size & 0b00000000111111111111111111111111;

            auto cache = find_cache(cache_id);
            if (cache == nullptr) {
                _caches.add(cache_t(_backing, cache_id));
                cache = &_caches[0];
            }

            if (cache->slabs.empty()) {
                slab_t slab{};
                slab.size = actual_size;
                slab.state = slab_state_t::partial;
                slab.chunk = (char*)_backing->allocate(_slab_size);
                _total_allocated += _slab_size;
                cache->slabs.add(slab);
                return slab.chunk;
            } else {
                for (auto& slab : cache->slabs) {
                    if (slab.size != actual_size
                    ||  slab.state == slab_state_t::full) {
                        continue;
                    }
                    if (slab.offset + actual_size > _slab_size) {
                        slab.state = slab_state_t::full;

                        slab_t new_slab{};
                        new_slab.chunk = (char*)_backing->allocate(_slab_size);
                        _total_allocated += _slab_size;
                        cache->slabs.add(new_slab);

                        return new_slab.chunk;
                    } else {
                        slab.offset += numbers::align(actual_size, align);
                        return slab.chunk + slab.offset;
                    }
                }
            }

            assert(false);
        }

        void deallocate(void* p) override {
        }

        std::optional<uint32_t> total_allocated() override {
            return _total_allocated;
        }

        std::optional<uint32_t> allocated_size(void *p) override {
            return {};
        }

    private:
        uint32_t _slab_size;
        allocator_t* _backing;
        uint32_t _total_allocated{};
        data::array_t<cache_t> _caches;
    };

}
