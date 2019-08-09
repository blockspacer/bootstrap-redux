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

#include "trace_allocator.h"

namespace basecode::compiler::memory {

    trace_allocator_t::trace_allocator_t(allocator_t* backing) : _backing(backing) {
        _stream_factory.enabled(true);
        _stream = _stream_factory.use_memory_buffer(_buffer);
    }

    trace_allocator_t::~trace_allocator_t() {
        fmt::print("{}\n", _stream->format());
    }

    void* trace_allocator_t::allocate(
            uint32_t size,
            uint32_t align,
            const char* file_name,
            const char* function_name,
            int line_number) {
        auto p = _backing->allocate(
            size,
            align,
            __FILE__,
            __FUNCTION__,
            __LINE__);
        _stream
            ->color(terminal::colors_t::default_color, terminal::colors_t::green)
            ->append(fmt::format(
                "{}: {}@{} allocate: size = {}, align = {}, p = {}, _backing->total_allocated() = {}\n",
                file_name ? file_name : __FILE__,
                function_name ? function_name : __FUNCTION__,
                line_number,
                size,
                align,
                p,
                *_backing->total_allocated()))
            ->color_reset();
        return p;
    }

    void trace_allocator_t::deallocate(
            void* p,
            const char* file_name,
            const char* function_name,
            int line_number) {
        _backing->deallocate(p, __FILE__, __FUNCTION__, __LINE__);
        _stream
            ->color(terminal::colors_t::default_color, terminal::colors_t::yellow)
            ->append(fmt::format(
                "{}: {}@{} deallocate: p = {}, backing->total_allocated() = {}\n",
                file_name ? file_name : __FILE__,
                function_name ? function_name : __FUNCTION__,
                line_number,
                p,
                *_backing->total_allocated()
            ))
            ->color_reset();
    }

    std::optional<uint32_t> trace_allocator_t::total_allocated() {
        return _backing->total_allocated();
    }

    std::optional<uint32_t> trace_allocator_t::allocated_size(void* p) {
        return _backing->allocated_size(p);
    }

}