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

#include <compiler/memory/system.h>
#include "session.h"

namespace basecode::compiler::workspace {

    session_t::session_t(
            const session_options_t& options) : _options(options),
                                                _interned_strings(
                                                    memory::default_allocator(),
                                                    _options.intern_string_block_size) {
    }

    std::string_view session_t::intern(std::string_view value) {
        return _interned_strings.intern(value);
    }

    std::string_view session_t::intern(const std::string& value) {
        return _interned_strings.intern(value);
    }

}
