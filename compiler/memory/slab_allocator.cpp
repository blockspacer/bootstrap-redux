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

#include "slab_allocator.h"

namespace basecode::compiler::memory {

    uint32_t slab_allocator_t::make_size(uint8_t cache_id, uint32_t size) {
        auto bottom_24_bits = (size & 0b00000000111111111111111111111111);
        auto shifted_id = (cache_id << 24);
        return shifted_id | bottom_24_bits;
    }

    ///////////////////////////////////////////////////////////////////////////

    slab_allocator_t::slab_allocator_t(
            allocator_t* backing,
            uint32_t slab_size) : _slab_size(slab_size),
                                  _backing(backing),
                                  _caches(backing) {
    }

    slab_allocator_t::~slab_allocator_t() {
        for (auto& cache : _caches) {
            for (const auto& slab : cache.slabs) {
                _backing->deallocate(slab.chunk);
                _total_allocated -= _slab_size;
            }
            cache.~cache_t();
        }
        assert(_total_allocated == 0);
    }

    void* slab_allocator_t::allocate(
            uint32_t size,
            uint32_t align) {
        auto cache_id = (size & 0b11111111000000000000000000000000) >> 24;
        auto actual_size = size & 0b00000000111111111111111111111111;

        auto cache = find_cache(cache_id);
        if (cache == nullptr) {
            _caches.add(cache_t(_backing, cache_id));
            cache = &_caches.back();
        }

        void* new_chunk = nullptr;
        if (cache->slabs.empty()) {
            slab_t slab{};
            slab.size = actual_size;
            slab.state = slab_state_t::partial;

            slab.chunk = (char*)_backing->allocate(_slab_size);
            cache->slabs.add(slab);

            new_chunk = slab.chunk;
            _total_allocated += _slab_size;
        } else {
            slab_t* slab = nullptr;

            for (size_t i = 0; i < cache->slabs.size(); i++) {
                const auto& s = cache->slabs[i];
                if (s.size != actual_size
                ||  s.state == slab_state_t::full) {
                    continue;
                }
                slab = &cache->slabs[i];
                break;
            }

            if (slab == nullptr
            ||  slab->offset + actual_size > _slab_size) {
                if (slab)
                    slab->state = slab_state_t::full;

                slab_t new_slab{};
                new_slab.size = actual_size;
                new_slab.state = slab_state_t::partial;
                new_slab.chunk = (char*)_backing->allocate(_slab_size);
                cache->slabs.add(new_slab);

                new_chunk = new_slab.chunk;
                _total_allocated += _slab_size;
            } else {
                slab->offset += numbers::align(actual_size, align);
                new_chunk = slab->chunk + slab->offset;
            }
        }

        return new_chunk;
    }

    void slab_allocator_t::deallocate(void* p) {
    }

    std::optional<uint32_t> slab_allocator_t::total_allocated() {
        return _total_allocated;
    }

    std::optional<uint32_t> slab_allocator_t::allocated_size(void* p) {
        return {};
    }

    slab_allocator_t::cache_t* slab_allocator_t::find_cache(uint8_t id) {
        for (auto& cache : _caches) {
            if (cache.id == id)
                return &cache;
        }
        return nullptr;
    }

}