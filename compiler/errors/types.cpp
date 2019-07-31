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

#include "types.h"

namespace basecode::compiler::errors {

    static std::unordered_map<error_decl_key_t, error_decl_t> s_error_decls{
        {{.code = lexer::unable_to_convert_integer_value, .locale = "en_US"},
         {.code = "L001", .message = "unable to convert integer value {} because {}"}},

        {{.code = lexer::unable_to_narrow_integer_value, .locale = "en_US"},
         {.code = "L002", .message = "unable to narrow integer value"}},

        {{.code = lexer::unable_to_convert_floating_point_value, .locale = "en_US"},
         {.code = "L003", .message = "unable to convert floating point value {} because {}"}},

        {{.code = lexer::unable_to_narrow_floating_point_value, .locale = "en_US"},
         {.code = "L004", .message = "unable to narrow floating point value"}},

        {{.code = lexer::invalid_identifier_start_character, .locale = "en_US"},
         {.code = "L005", .message = "identifiers must start with _ or a letter; found: {}"}},

        {{.code = lexer::expected_identifier, .locale = "en_US"},
         {.code = "L006", .message = "expected identifier"}},

        {{.code = lexer::unexpected_end_of_input, .locale = "en_US"},
         {.code = "L007", .message = "unexpected end of input"}},

        {{.code = lexer::unexpected_decimal_point, .locale = "en_US"},
         {.code = "L008", .message = "unexpected decimal point"}},

        {{.code = lexer::expected_closing_single_quote, .locale = "en_US"},
         {.code = "L009", .message = "expected closing ' but found: {}"}},

        {{.code = lexer::expected_directive_prefix, .locale = "en_US"},
         {.code = "L010", .message = "expected directive prefix: #"}},

        {{.code = lexer::expected_annotation_prefix, .locale = "en_US"},
         {.code = "L011", .message = "expected annotation prefix: @"}},

        {{.code = lexer::exponent_notation_not_valid_for_integers, .locale = "en_US"},
         {.code = "L012", .message = "exponent notation is not valid for integer literals"}},

        {{.code = lexer::unexpected_letter_after_decimal_number_literal, .locale = "en_US"},
         {.code = "L013", .message = "unexpected letter immediately after decimal number"}},

        {{.code = lexer::expected_hex_literal_prefix, .locale = "en_US"},
         {.code = "L014", .message = "expected hex prefix: $"}},

        {{.code = lexer::unexpected_letter_after_hexadecimal_number_literal, .locale = "en_US"},
         {.code = "L015", .message = "unexpected letter immediately after hexadecimal number"}},

        {{.code = lexer::expected_octal_literal_prefix, .locale = "en_US"},
         {.code = "L016", .message = "expected octal prefix: @"}},

        {{.code = lexer::unexpected_letter_after_octal_number_literal, .locale = "en_US"},
         {.code = "L017", .message = "unexpected letter immediately after octal number"}},

        {{.code = lexer::expected_binary_literal_prefix, .locale = "en_US"},
         {.code = "L018", .message = "expected binary prefix: %"}},

        {{.code = lexer::unexpected_letter_after_binary_number_literal, .locale = "en_US"},
         {.code = "L019", .message = "unexpected letter or non-binary digit immediately after binary number"}},
         
        {{.code = lexer::expected_closing_block_literal, .locale = "en_US"},
         {.code = "L020", .message = "expected }} but found: {}"}},
    };

    error_decl_t* find_decl(error_code_t code) {
        std::locale locale;

        // XXX: this sucks.  come back and spend time on it.
        auto name = locale.name();
        if (name == "C")
            name = "en_US";

        error_decl_key_t key{name, code};
        auto it = s_error_decls.find(key);
        if (it == std::end(s_error_decls))
            return nullptr;
        return &it->second;
    }

}