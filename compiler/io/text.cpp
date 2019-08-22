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

#include <fstream>
#include <fmt/format.h>
#include "text.h"

namespace basecode::io::text {

    bool read(
            result_t& r,
            const path_t& path,
            std::iostream& stream) {
        try {
            std::ifstream file(path.string(), std::ios::in);
            if (file.is_open()) {
                stream << file.rdbuf();
                file.close();
            }
        } catch (std::exception& e) {
            r.error(
                "P001",
                fmt::format("unable to read_text from file: {}", e.what()));
            return false;
        }
        return true;
    }

    bool write(
            result_t& r,
            const path_t& path,
            const std::iostream& stream) {
        try {
            std::ofstream file(path.string(), std::ios::out);
            if (file.is_open()) {
                file << stream.rdbuf();
                file.close();
            }
        } catch (std::exception& e) {
            r.error(
                "P001",
                fmt::format("unable to write_text to file: {}", e.what()));
            return false;
        }
        return true;
    }

    bool write(
            result_t& r,
            const path_t& path,
            const std::string& text) {
        try {
            std::ofstream file(path.string(), std::ios::out);
            if (file.is_open()) {
                file << text;
                file.close();
            }
        } catch (std::exception& e) {
            r.error(
                "P001",
                fmt::format("unable to write_text to file: {}", e.what()));
            return false;
        }
        return true;
    }

}