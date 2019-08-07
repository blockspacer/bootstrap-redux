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
#include <compiler/types.h>

namespace basecode::compiler::language::core::ast {

    struct node_t final {
        entity_t token{};
        entity_t parent{};
        entity_list_t comments{};
        entity_list_t directives{};
        entity_list_t annotations{};
    };

    ///////////////////////////////////////////////////////////////////////////

    struct module_t final {
        path_t path;
        std::string_view name{};
        entity_list_t children{};
    };

    ///////////////////////////////////////////////////////////////////////////

    enum class unary_operator_op_t {
        neg,        // foo := -16;
        bnot,       // foo := ~16;
        lnot,       // foo := !true;
    };

    struct unary_operator_t final {
        entity_t lhs{};
        unary_operator_op_t op{};
    };

    ///////////////////////////////////////////////////////////////////////////

    enum class binary_operator_op_t {
        eq,         // foo := 2 == 2;
        gt,         // foo := 2 > 1;
        lt,         // foo := 1 < 2;
        in,         // for _, v in [3]u8 {1, 2, 3} print("{v}\n");
        xor_,       // foo := $ff xor $ff;
        neq,        // foo := 1 != 2;
        add,        // foo := 10 + 10;
        sub,        // foo := 10 - 2;
        mul,        // foo := 2 * 2;
        div,        // foo := 10 / 2;
        mod,        // foo := 10 % 2;
        pow,        // foo := 8 ** 2;
        bor,        // foo := 16 | 2;
        lor,        // foo := true || false;
        shl,        // foo := 16 shl 2;
        shr,        // foo := 16 shr 2;
        rol,        // foo := 16 rol 2;
        ror,        // foo := 16 ror 2;
        gte,        // foo := 1 >= 1;
        lte,        // foo := 1 <= 1;
        sep,        // j, k, l := 1, 2, 3;
        bind,       // core::utf8 :: ns { };
        band,       // foo := 16 & 2;
        land,       // foo := true && false;
        assoc,      // foo: Pair := 10 => "ten";
    };

    struct binary_operator_t final {
        entity_t lhs{};
        entity_t rhs{};
        binary_operator_op_t op{};
    };

    ///////////////////////////////////////////////////////////////////////////

    struct assignment_operator_t final {
        entity_list_t lhs{};
        entity_list_t rhs{};
        binary_operator_op_t op{};
    };

    ///////////////////////////////////////////////////////////////////////////

    struct expression_t final {
        entity_t value{};
    };

    ///////////////////////////////////////////////////////////////////////////

    struct label_t final {
        entity_t identifier{};
    };

    ///////////////////////////////////////////////////////////////////////////

    struct identifier_t final {
        entity_t value{};
        entity_t scope{};
        entity_t block{};
    };

    ///////////////////////////////////////////////////////////////////////////

    struct value_sink_t final {};

    ///////////////////////////////////////////////////////////////////////////

    // N.B. at each scope, both partial qualified and naked identifier names
    //      should be added to the trie map.
    //
    // numbers
    // numbers::to_roman_numeral
    //          to_roman_numeral
    //
    struct scope_t final {
        entity_list_t children{};
        tsl::htrie_map<char, entity_t> identifiers{};
    };

    ///////////////////////////////////////////////////////////////////////////

    struct block_t final {
        entity_t scope{};
        entity_list_t children{};
    };

    ///////////////////////////////////////////////////////////////////////////

    enum class literal_type_t {
        number,
        string,
        boolean
    };

    struct literal_t final {
        literal_type_t type{};
    };

    ///////////////////////////////////////////////////////////////////////////

    enum class initializer_type_t {
        scalar,
        aggregate
    };

    struct initializer_t final {
        entity_t expr{};
        initializer_type_t type{};
    };

    ///////////////////////////////////////////////////////////////////////////

    struct line_comment_t final {
    };

    struct block_comment_t final {
    };

    ///////////////////////////////////////////////////////////////////////////

    struct annotation_t final {
        entity_t rhs{};
    };

    ///////////////////////////////////////////////////////////////////////////

    struct directive_t final {
        entity_t rhs{};
    };

    ///////////////////////////////////////////////////////////////////////////

    struct type_parameter_t final {
        entity_t identifier{};
        entity_t constraint{};
    };

    ///////////////////////////////////////////////////////////////////////////

    // result := if x == 1 break true;
    struct if_expression_t final {
        entity_t body{};
        entity_t predicate{};
        entity_t else_or_elseif{};
    };

    // foo::bar::baz :: ns {
    // };
    struct ns_expression_t final {
        entity_t expr{};
    };

    // for i, v in s32[] {1, 2, 3} print("{i} = {v}\n");
    // j:= 0; for i, v in 1..3 j +:= v;
    struct for_expression_t final {
        entity_t expr{};
        entity_t body{};
        entity_list_t vars{};
    };

    // x := 256;
    // f: bool;
    //
    // result := use f {
    //      // accessing x here is an error!
    //      if f break x;
    // };
    struct use_expression_t final {
        entity_t body{};
        entity_list_t vars{};
    };

    // y: s16 := -128;
    // x: u8 := cast<u8>(y);
    struct cast_expression_t final {
        entity_t expr{};
        entity_t type_parameter{};
    };

    // ch := #rune "j";
    // result := switch ch {
    //      case #rune "a" .. #rune "z" => {
    //          // do something
    //      };
    //      case => {
    //          // default
    //      };
    // };
    struct case_expression_t final {
        entity_t expr{};
        entity_t body{};
    };

    // fib :: proc(n: u64): u64 {
    //      if n < 2 return n;
    //      return fib(n - 2) + fib(n - 1);
    // };
    struct proc_expression_t final {
        entity_t body{};
        entity_list_t params{};
        entity_list_t return_params;
        entity_list_t type_parameters{};
    };

    // colors :: enum<u16> {
    //      red;
    //      green;
    //      blue;
    //      purple;
    // };
    struct enum_expression_t final {
        entity_list_t body{};
        entity_t type_parameter{};
    };

    // Point :: struct {
    //   x, y: f32;
    // };
    //
    // some_point: Point {x := 33.2, y := -16};
    // result := with some_point {
    //      print("x := {.x}, y := {.y}\n");
    //      break nil;
    // };
    struct with_expression_t final {
        entity_t expr{};
        entity_t body{};
    };

    // goto skip;
    //
    // print("not executed\n");
    //
    // skip: print("previous statement skipped\n");
    struct goto_expression_t final {
        entity_t expr{};
    };

    // i: s32;
    // result := while i < 10 {
    //   print("i = {i}\n");
    //   i +:= 1;
    // };
    struct while_expression_t final {
        entity_t expr{};
        entity_t body{};
    };

    //
    // mem := alloc(1024);
    // defer free(mem);
    //
    struct defer_expression_t final {
        entity_t expr{};
    };

    // two uses for break:
    //
    // // break to a label
    // i: s32;
    // while true {
    //
    //   i +:= 1;
    //   if i % 2 break skip;
    //   print("i = {i}\n");
    //
    // skip:
    //   if i == 20 break;  // exit the while loop
    // };
    //
    // // break returns a value to immediate outer scope
    //
    // result := if i == 20 break true;
    // assert(result == true);
    struct break_expression_t final {
        entity_t expr{};
    };

    // Sized_Integer :: union {
    //    byte: u8;
    //    word: u16;
    //    dword: u32;
    //    qword: u64;
    // };
    //
    // #assert(size_of(Sized_Integer) == 8);
    struct union_expression_t final {
        entity_t body{};
        entity_list_t type_parameters{};
    };

    // co_something :: @coroutine proc(n: s32): s32 {
    //      if n % 2 == 0 yield n;
    //      yield n * 2;
    // };
    struct yield_expression_t final {
        entity_list_t expr{};
    };

    // Vector3 :: struct {
    //      x, y, z: f32;
    // };
    //
    // #assert(size_of(Vector3) == 16);
    struct struct_expression_t final {
        entity_t body{};
        entity_list_t type_parameters{};
    };

    // Integer :: family(u8, s8, u16, s16, u32, s32, u64, s64);
    // Floating_Point :: family(f32, f64);
    //
    // // single return parameter
    // adder :: proc<T:Integer>(lhs: T, rhs: T): T {
    //      return lhs + rhs;
    // };
    //
    // // multiple return parameters, anonymous
    // divide_with_remainder :: proc<T:Integer>(lhs, rhs: T): T, T {
    //      return lhs/rhs, lhs%rhs;
    // };
    //
    // // multiple return parameters, named
    // divide_with_remainder :: proc<T:Floating_Point>(lhs, rhs: T): quotient: T, remainder: T {
    //      quotient := lhs/rhs;
    //      remainder := lhs%rhs;
    // };
    struct return_expression_t final {
        entity_list_t exprs{};
    };

    //
    // j: s32 := 5;
    //
    // switch j {
    //      case 0..5   => print("0 to 5\n");
    //      case 6..<10 => print("6 to 9\n");
    // };
    struct switch_expression_t final {
        entity_t expr{};
        entity_t body{};
    };

    // Integer :: family(u8, s8, u16, s16, u32, s32, u64, s64);
    // Floating_Point :: family(f32, f64);
    struct family_expression_t final {
        entity_list_t types{};
    };

    // core :: module("core");
    struct module_expression_t final {
        entity_t expr{};
    };

    // // bring symbols into current namespace
    // import(core::types);
    //
    // // bring symbols into named namespace
    // internal :: ns {
    //      import(core::types);
    // };
    //
    // - or -
    //
    // internal :: import(core::types);
    struct import_expression_t final {
        entity_t expr{};
    };

    // // skip remainder of statements and go back to
    // // start of loop
    //
    // for i, v in 0..100 {
    //      if i % 2 == 0 continue;
    //      print("i = {i}, v = {v}\n");
    // };
    struct continue_expression_t final {
        entity_t block{};
    };

    // y: s16 := -128;
    // x: u8 := transmute<u8>(y);
    struct transmute_expression_t final {
        entity_t expr{};
        entity_t type_parameter{};
    };

    // // mark a switch case a fall through
    // ch := #rune "j";
    // result := switch ch {
    //      case #rune "a" .. #rune "z" => {
    //          // do something
    //          fallthrough;
    //      };
    //      case => {
    //          // default
    //      };
    // };
    struct fallthrough_expression_t final {};

    ///////////////////////////////////////////////////////////////////////////

    struct statement_t final {
        entity_t expr{};
        entity_list_t labels{};
    };

}