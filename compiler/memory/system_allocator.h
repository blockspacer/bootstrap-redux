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

#include <string_view>
#include "allocator.h"
#include "dlmalloc_config.h"

namespace basecode::compiler::memory {

    struct header_t final {
        uint32_t size;
    };

    const uint32_t header_pad_value = 0xffffffffu;

    inline header_t* header(void* data) {
        auto p = static_cast<uint32_t*>(data);
        while (p[-1] == header_pad_value)
            --p;
        return reinterpret_cast<header_t*>(p - 1);
    }

    inline void* data_pointer(header_t* header, uint32_t align) {
        void* p = header + 1;
        return align_forward(p, align);
    }

    inline void fill(header_t* header, void* data, uint32_t size) {
        header->size = size;
        auto p = reinterpret_cast<uint32_t*>(header + 1);
        while (p < data)
            *p++ = header_pad_value;
    }

    class system_allocator_t : public allocator_t {
    public:
        system_allocator_t() = default;

        explicit system_allocator_t(mspace* space);

        ~system_allocator_t() override;

        void* allocate(
            uint32_t size,
            uint32_t align) override;

        void deallocate(void* p) override;

        std::optional<uint32_t> total_allocated() override;

        std::optional<uint32_t> allocated_size(void* p) override;

    private:
        static inline uint32_t size_with_padding(uint32_t size, uint32_t align) {
            return size + align + sizeof(header_t);
        }

    private:
        mspace* _space{};
        uint32_t _total_allocated = 0;
    };

}
