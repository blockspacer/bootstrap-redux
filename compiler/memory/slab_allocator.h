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
        static uint32_t make_size(
                uint8_t cache_id,
                uint32_t size);

        explicit slab_allocator_t(
                allocator_t* backing,
                uint32_t slab_size = 4096);

        ~slab_allocator_t() override;

        void* allocate(
                uint32_t size,
                uint32_t align = default_align) override;

        void deallocate(void* p) override;

        std::optional<uint32_t> total_allocated() override;

        std::optional<uint32_t> allocated_size(void *p) override;

    private:
        cache_t* find_cache(uint8_t id);

    private:
        uint32_t _slab_size;
        allocator_t* _backing;
        uint32_t _total_allocated{};
        data::array_t<cache_t> _caches;
    };

}
