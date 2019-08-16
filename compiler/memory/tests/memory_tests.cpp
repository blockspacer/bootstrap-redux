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
#include <compiler/memory/trace_allocator.h>

namespace basecode {

    using namespace std::literals;
    using namespace compiler;
    using namespace compiler::memory;

    TEST_CASE("slab_allocator_t") {
        trace_allocator_t tracer(memory::default_scratch_allocator());
        slab_allocator_t block1(&tracer, "block1", 64);

        void* blocks[128];
        for (size_t i = 0; i < 128; i++) {
            blocks[i] = block1.allocate();
        }

        for (auto p : blocks)
            REQUIRE(p);
    }

}