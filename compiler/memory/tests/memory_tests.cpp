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

#include <fmt/format.h>
#include <catch2/catch.hpp>
#include <compiler/memory/system.h>
#include <compiler/memory/slab_allocator.h>

namespace basecode {

    using namespace std::literals;
    using namespace compiler;
    using namespace compiler::memory;

    TEST_CASE("slab_allocator_t") {
        slab_allocator_t slabbers(memory::default_scratch_allocator());

        auto data1 = slabbers.allocate(slab_allocator_t::make_size(1, 32));
        REQUIRE(data1);

        auto data2 = slabbers.allocate(slab_allocator_t::make_size(1, 32));
        REQUIRE(data2);
    }

}