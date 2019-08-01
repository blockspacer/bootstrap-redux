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

#include <cstdint>
#include <optional>

namespace basecode::compiler::memory {

    class allocator_t {
    public:
        static const uint32_t default_align = 4;

        allocator_t() = default;

        allocator_t(const allocator_t& other) = delete;

        allocator_t& operator=(const allocator_t& other) = delete;

        virtual ~allocator_t() = default;

        virtual void* allocate(
            uint32_t size,
            uint32_t align = default_align) = 0;

        virtual void deallocate(void* p) = 0;

        virtual std::optional<uint32_t> total_allocated() = 0;

        virtual std::optional<uint32_t> allocated_size(void* p) = 0;
    };

}