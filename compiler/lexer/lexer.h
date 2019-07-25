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

#include <compiler/utf8/source_buffer.h>

namespace basecode::compiler::lexer {

    class lexer_t;

    using lexeme_tokenizer_t = std::function<bool (lexer_t*, result_t&)>;

    enum class prefix_type_t {
        none,
        conditional,
        non_conditional
    };

    struct lexeme_t final {
        token_type_t type{};
        prefix_type_t prefix{};
    };

    class lexer_t final {
    public:
        explicit lexer_t(utf8::source_buffer_t* buffer);

        bool tokenize(result_t& r);

    private:
        bool tokenize_hex_number(result_t& r);

        bool tokenize_octal_number(result_t& r);

        bool tokenize_binary_number(result_t& r);

    private:
        static tsl::htrie_map<char, lexeme_t> s_lexemes;
        static tsl::htrie_map<char, uint8_t> s_custom_lexemes;
        static std::array<lexeme_tokenizer_t, 3> s_custom_tokenizers;

        utf8::source_buffer_t* _buffer{};
    };

}