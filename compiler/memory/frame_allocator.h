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

#include "system.h"

namespace basecode::compiler::memory {

    template <std::size_t BlockSize, std::size_t MaxBlocks>
    class frame_allocator_t : public allocator_t {
    public:
        explicit frame_allocator_t(
                allocator_t* backing = default_allocator(),
                uint32_t block_size = BlockSize) : _block_size(block_size),
                                                   _backing(backing) {
        }

        ~frame_allocator_t() override {
            for (size_t i = 0; i < _block_index; i++) {
                _backing->deallocate(_blocks[i]);
                _total_allocated -= _block_size;
            }
            assert(_total_allocated == 0);
        }

        void* allocate(
                uint32_t size,
                uint32_t align) override {
            assert(_block_index + 1 < MaxBlocks);

            auto block = _backing->allocate(_block_size, align);
            _total_allocated += _block_size;
            _blocks[_block_index++] = block;
            return block;
        }

        void deallocate(void* p) override {
        }

        std::optional <uint32_t> total_allocated() override {
            return _total_allocated;
        }

        std::optional <uint32_t> allocated_size(void* p) override {
            return _block_size;
        }

    private:
        uint32_t _block_size;
        size_t _block_index{};
        allocator_t* _backing{};
        void* _blocks[MaxBlocks]{};
        uint32_t _total_allocated{};
    };

}