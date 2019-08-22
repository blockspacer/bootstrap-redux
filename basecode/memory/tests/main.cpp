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

#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <basecode/defer.h>
#include <basecode/signals/hook.h>
#include <basecode/memory/system.h>
#include <basecode/errors/errors.h>

using namespace basecode;

int main(int argc, char** argv) {
    memory::initialize();
    context::initialize(memory::default_scratch_allocator());

    result_t r;
    if (!errors::initialize(r))  return 1;

    if (!signals::initialize(r)) return 1;

    defer({
        signals::shutdown();
        errors::shutdown();
        context::shutdown();
        memory::shutdown();
    });

    auto result = Catch::Session().run(argc, argv);
    return (result < 0xff ? result : 0xff);
}
