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
#include <compiler/defer.h>
#include <compiler/memory/system.h>

using namespace basecode::compiler;

int main(int argc, char** argv) {
    memory::initialize();
    defer(memory::shutdown());

    auto result = Catch::Session().run(argc, argv);
    return (result < 0xff ? result : 0xff);
}
