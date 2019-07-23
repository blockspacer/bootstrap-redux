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

#include <unordered_map>
#include "hook.h"

namespace basecode::compiler::signals {

    static std::unordered_map<int, action_t> s_actions{};

    static void on_signal(int sig) {
        auto it = s_actions.find(sig);
        if (it == std::end(s_actions))
            return;
        for (auto handler : it->second.handlers) {
            if (!(*handler)())
                return;
        }
    }

    bool hook(int sig, handler_t* handler) {
        action_t* action = nullptr;

        auto it = s_actions.find(sig);
        if (it == std::end(s_actions)) {
            auto rc = s_actions.emplace(sig, action_t{});
            if (rc.second) {
                action = &rc.first->second;
                action->sigact.sa_flags = 0;
                sigemptyset(&action->sigact.sa_mask);
                action->sigact.sa_handler = on_signal;
                sigaction(sig, &action->sigact, nullptr);
            } else {
                return false;
            }
        } else {
            action = &it->second;
        }

        action->handlers.push_back(handler);

        return true;
    }

}