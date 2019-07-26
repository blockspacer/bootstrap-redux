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

#include <stack>
#include <string_view>
#include <compiler/types.h>
#include "rune.h"

namespace basecode::compiler::utf8 {

    class reader_t final {
    public:
        explicit reader_t(std::string_view slice);

        void push_mark();

        size_t pop_mark();

        size_t current_mark();

        bool seek(size_t index);

        void restore_top_mark();

        rune_t curr(result_t& r);

        rune_t next(result_t& r);

        rune_t prev(result_t& r);

        bool move_prev(result_t& r);

        bool move_next(result_t& r);

        [[nodiscard]] bool eof() const;

        [[nodiscard]] size_t pos() const;

        [[nodiscard]] std::string_view make_slice(size_t offset, size_t length) const;

    private:
        rune_t read(result_t& r, uint8_t& width);

    private:
        size_t _index{};
        std::string_view _slice;
        std::stack<size_t> _mark_stack{};
        std::stack<uint8_t> _width_stack{};
    };

}