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

#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
#include <boost/stacktrace.hpp>
#include "trace_allocator.h"

using namespace std::literals;

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
            uint32_t align) {
        boost::stacktrace::stacktrace trace(2, 2);
        _stream->color(
            terminal::colors_t::default_color,
            terminal::colors_t::yellow);
        for (int32_t i = trace.size() - 1; i >= 0; --i) {
            if (i != trace.size() - 1) _stream->append(" -> "sv);
            const auto& frame = trace[i];
            _stream->append(frame.name());
        }
        _stream->append("\n"sv)->color_reset();

        auto p = _backing->allocate(size, align);
        return p;
    }

    void trace_allocator_t::deallocate(void* p) {
        boost::stacktrace::stacktrace trace(2, 2);
        _stream->color(
            terminal::colors_t::default_color,
            terminal::colors_t::green);
        for (int32_t i = trace.size() - 1; i >= 0; --i) {
            if (i != trace.size() - 1) _stream->append(" -> "sv);
            const auto& frame = trace[i];
            _stream->append(frame.name());
        }
        _stream->append("\n"sv)->color_reset();

        _backing->deallocate(p);
    }

    std::optional<uint32_t> trace_allocator_t::total_allocated() {
        return _backing->total_allocated();
    }

    std::optional<uint32_t> trace_allocator_t::allocated_size(void* p) {
        return _backing->allocated_size(p);
    }

}