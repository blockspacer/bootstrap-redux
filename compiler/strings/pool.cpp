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

#include <compiler/hashing/murmur.h>
#include "pool.h"

namespace basecode::compiler::strings {

    pool_t::pool_t(
            memory::allocator_t* allocator,
            uint32_t block_size) : _block_size(block_size),
                                   _allocator(allocator) {
    }

    char* pool_t::next_data_pointer(uint32_t length) {
        _block_offset += length;

        if (_block == nullptr
        ||  _block_offset > _block_size) {
            _block = (char*)_allocator->allocate(_block_size, 1);
            _block_offset = 0;
        }

        return reinterpret_cast<char*>(_block + _block_offset);
    }

    std::string_view pool_t::intern(std::string_view value) {
        auto hash = hashing::murmur::hash64(
            value.data(),
            value.length());
        auto it = _index.find(hash);
        if (it == std::end(_index)) {
            auto data_ptr = next_data_pointer(value.length());
            std::memcpy(data_ptr, value.data(), value.length());
            auto result = _index.insert(std::make_pair(
                hash,
                std::string_view(data_ptr, value.length())));
            return result.first->second;
        }
        return it->second;
    }

    std::string_view pool_t::intern(const std::string& value) {
        return intern(std::string_view(value));
    }

}
