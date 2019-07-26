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

#include <tsl/htrie_map.h>
#include <compiler/utf8/source_buffer.h>
#include "token.h"

namespace basecode::compiler::lexer {

    class lexer_t;

    using lexeme_tokenizer_t = std::function<bool (lexer_t*, result_t&)>;

    struct lexeme_t final {
        token_type_t type{};
        lexeme_tokenizer_t tokenizer{};
    };

    class lexer_t final {
    public:
        explicit lexer_t(utf8::source_buffer_t* buffer);

        bool tokenize(result_t& r);

    private:
        bool identifier(result_t& r);

        bool rune_literal(result_t& r);

        bool line_comment(result_t& r);

        bool block_comment(result_t& r);

        bool string_literal(result_t& r);

        bool directive_literal(result_t& r);

        bool annotation_literal(result_t& r);

        bool dec_number_literal(result_t& r);

        bool hex_number_literal(result_t& r);

        bool octal_number_literal(result_t& r);

        bool binary_number_literal(result_t& r);

    private:
        static tsl::htrie_map<char, lexeme_t> s_lexemes;

        utf8::source_buffer_t* _buffer{};
    };

}