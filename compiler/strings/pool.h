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

#include <string>
#include <string_view>
#include <unordered_map>
#include <compiler/data/hash_table.h>
#include <compiler/memory/allocator.h>

namespace basecode::compiler::strings {

    class pool_t final {
    public:
        explicit pool_t(
            memory::allocator_t* allocator,
            uint32_t block_size = 64*1024);

        std::string_view intern(std::string_view value);

        std::string_view intern(const std::string& value);

    private:
        uint32_t _block_size;
        memory::allocator_t* _allocator;
        data::hash_table_t<std::string_view, void*> _index;
    };

}
