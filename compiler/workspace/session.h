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

#include <compiler/types.h>
#include <compiler/strings/pool.h>
#include <compiler/utf8/source_buffer.h>
#include <compiler/memory/frame_allocator.h>

namespace basecode::compiler::workspace {

    struct session_options_t final {
        uint32_t intern_string_block_size = 64*1024;
        memory::allocator_t* allocator = memory::default_allocator();
    };

    class session_t final {
    public:
        explicit session_t(const session_options_t& options);

        entt::registry& registry() {
            return _registry;
        }

        strings::pool_t& intern_pool() {
            return _intern_pool;
        }

        memory::allocator_t* allocator() {
            return _allocator;
        }

    private:
        entt::registry _registry{};
        strings::pool_t _intern_pool;
        memory::allocator_t* _allocator;
        const session_options_t& _options;
        memory::frame_allocator_t<64 * 1024, 32> _intern_allocator;
    };

}
