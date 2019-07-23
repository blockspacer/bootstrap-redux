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

#include "auto_reset_event.h"

namespace basecode::compiler::threading {

    auto_reset_event_t::auto_reset_event_t(bool initial) : _flag(initial) {
    }

    void auto_reset_event_t::set() {
        std::lock_guard<std::mutex> lock(_protect);
        _flag = true;
        _signal.notify_all();
    }

    void auto_reset_event_t::reset() {
        std::lock_guard<std::mutex> lock(_protect);
        _flag = false;
    }

    bool auto_reset_event_t::wait_one() {
        std::unique_lock<std::mutex> lock(_protect);
        while (!_flag)
            _signal.wait(lock);
        _flag = false;
        return true;
    }

    bool auto_reset_event_t::wait_one(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(_protect);
        if (!_flag) _signal.wait_for(lock, timeout);
        if (_flag) {
            _flag = false;
            return true;
        } else {
            return false;
        }
    }

}