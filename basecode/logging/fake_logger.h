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

#include <basecode/data/array.h>
#include <basecode/data/string.h>
#include "logger.h"

namespace basecode::logging {

    struct fake_log_entry_t final {
        fake_log_entry_t(
            log_level_t l,
            data::string_t m): level(l),
                               message(std::move(m)) {
        }

        log_level_t level;
        data::string_t message;
    };

    using fake_log_entry_list_t = data::array_t<fake_log_entry_t>;

    class fake_logger_t : public logger_t {
    public:
        fake_logger_t();

        explicit fake_logger_t(memory::allocator_t* allocator);

        void clear();

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        fake_log_entry_t& operator[] (size_t index);

    protected:
        void on_log(log_level_t level, const std::string& message) override;

    private:
        fake_log_entry_list_t _entries;
    };

}