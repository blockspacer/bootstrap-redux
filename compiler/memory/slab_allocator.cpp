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

#include <utility>
#include "system.h"
#include "slab_allocator.h"

namespace basecode::compiler::memory {

    slab_allocator_t::slab_allocator_t(
            allocator_t* backing,
            std::string name,
            uint32_t size,
            uint32_t align) : _name(std::move(name)),
                              _backing(backing),
                              _buffer_size(size),
                              _buffer_align(align) {
        _maximum_buffers = (os_page_size() - sizeof(slab_t)) / _buffer_size;
    }

    slab_allocator_t::~slab_allocator_t() {
        const auto page_size = os_page_size();

        while (_front) {
            auto slab = _front;
            remove(slab);
            _backing->deallocate(slab->page);
            _total_allocated -= page_size;
        }

        assert(_total_allocated == 0);
    }

    void slab_allocator_t::grow() {
        const auto page_size = os_page_size();

        auto mem = (char*)_backing->allocate(page_size);
        _total_allocated += page_size;

        slab_t* slab = (slab_t*)mem + page_size - sizeof(slab_t);
        slab->next = slab->prev = slab;
        slab->buffer_count = 0;
        slab->page = mem;
        slab->free_list = mem;

        void* last_buffer = mem + (_buffer_size * (_maximum_buffers - 1));
        for (auto p = mem; p < last_buffer; p += _buffer_size)
            *((void **)p) = p + _buffer_size;

        move_front(slab);
    }

    void* slab_allocator_t::allocate(
            uint32_t size,
            uint32_t align) {
        if (!_front || _front->buffer_count == _maximum_buffers)
            grow();

        auto buffer = _front->free_list;
        _front->free_list = *((void**)buffer);
        _front->buffer_count++;

        if (_front->buffer_count == _maximum_buffers)
            move_back(_front);

        return buffer;
    }

    void slab_allocator_t::remove(slab_t* slab) {
        slab->next->prev = slab->prev;
        slab->prev->next = slab->next;

        if (_front == slab) {
            if (slab->prev == slab)
                _front = nullptr;
            else
                _front = slab->prev;
        }

        if (_back == slab) {
            if (slab->next == slab)
                _back = nullptr;
            else
                _back = slab->next;
        }
    }

    void slab_allocator_t::deallocate(void* p) {
        const auto page_size = os_page_size();
        const auto slab_size = page_size - sizeof(slab_t);

        auto current = _front;
        while (current) {
            if (p >= current->page
            &&  p <= ((char*)current->page) + slab_size) {
                break;
            }
            current = current->next;
        }

        assert(current);

        slab_t* slab = (slab_t*)current->page + slab_size;
        *((void**)p) = slab->free_list;
        slab->free_list = p;
        slab->buffer_count--;

        if (slab->buffer_count == 0) {
            remove(slab);
            _backing->deallocate(slab->page);
            _total_allocated -= page_size;
        }

        if (slab->buffer_count == _maximum_buffers - 1)
            move_front(slab);
    }

    void slab_allocator_t::move_back(slab_t* slab) {
        if (_back == slab) return;

        remove(slab);

        if (_front == nullptr) {
            slab->prev = slab;
            slab->next = slab;
            _front = slab;
        } else {
            slab->prev = _front;
            _front->next = slab;

            slab->next = _back;
            _back->prev = slab;
        }

        _back = slab;
    }

    void slab_allocator_t::move_front(slab_t* slab) {
        if (_front == slab) return;

        remove(slab);

        if (_front == nullptr) {
            slab->prev = slab;
            slab->next = slab;
            _back = slab;
        }
        else {
            slab->prev = _front;
            _front->next = slab;

            slab->next = _back;
            _back->prev = slab;
        }

        _front = slab;
    }

    std::optional<uint32_t> slab_allocator_t::total_allocated() {
        return _total_allocated;
    }

    std::optional<uint32_t> slab_allocator_t::allocated_size(void*) {
        return _buffer_size;
    }

}