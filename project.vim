" ----------------------------------------------------------------------------
" ____                               _
" |  _\                             | |
" | |_)| __ _ ___  ___  ___ ___   __| | ___ TM
" |  _< / _` / __|/ _ \/ __/ _ \ / _` |/ _ \
" | |_)| (_| \__ \  __/ (_| (_) | (_| |  __/
" |____/\__,_|___/\___|\___\___/ \__,_|\___|
"
"       C O M P I L E R  P R O J E C T
"
" Copyright (C) 2019 Jeff Panici
" All rights reserved.
"
" This software source file is licensed under the terms of MIT license.
" For details, please read the LICENSE file.
"
" ----------------------------------------------------------------------------
"cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=/usr/local/opt/llvm/bin/clang -DCMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/clang++ -G "Ninja" -Wno-dev /Users/jeff/src/basecode-lang/bootstrap-redux
"
function! Project_UpdateTags()
    UpdateTags! -R bc/ compiler/ tests/
endfunction

function! Project_Cscope()
    copen 15
    AsyncRun cscope -Rb
endfunction

function! Project_CMakeGenerate()
    copen 15
    AsyncRun cd build/debug/clang && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=/usr/local/opt/llvm/bin/clang -DCMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/clang++ -G "Ninja" -Wno-dev /Users/jeff/src/basecode-lang/bootstrap-redux
endfunction

function! Project_BuildTestLexer()
    copen 15
    AsyncRun cd build/debug/clang && cmake --build . --target test-lexer -- -j12
endfunction

function! Project_RunTestLexer()
    copen 15
    AsyncRun cd build/debug/clang/bin && ./test-lexer
endfunction

command! ProjectCscope :call Project_Cscope()
command! ProjectUpdateTags :call Project_UpdateTags()
command! ProjectCmakeGenerate :call Project_CMakeGenerate()
command! ProjectBuildTestLexer :call Project_BuildTestLexer()
command! ProjectRunTestLexer :call Project_RunTestLexer()
