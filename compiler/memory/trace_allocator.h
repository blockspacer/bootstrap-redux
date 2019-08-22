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

#include <compiler/terminal/stream_factory.h>
#include "allocator.h"

namespace basecode::memory {

    class trace_allocator_t : public allocator_t {
    public:
        explicit trace_allocator_t(allocator_t* backing);

        ~trace_allocator_t() override;

        void* allocate(
            uint32_t size,
            uint32_t align) override;

        void deallocate(void* p) override;

        std::optional<uint32_t> total_allocated() override;

        std::optional<uint32_t> allocated_size(void *p) override;

    private:
        allocator_t* _backing{};
        fmt::memory_buffer _buffer{};
        terminal::stream_unique_ptr_t _stream{};
        terminal::stream_factory_t _stream_factory{};
    };

}