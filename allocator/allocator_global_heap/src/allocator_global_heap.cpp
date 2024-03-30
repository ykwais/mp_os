#include <not_implemented.h>

#include "../include/allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger) : _logger(logger)
{
    debug_with_guard("constructor of global heap allocator");
}

allocator_global_heap::~allocator_global_heap()
{
    debug_with_guard("Call of destructor of global heap allocator");
}


allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept : _logger(other._logger)
{
    debug_with_guard("Call move constructor of global heap allocator");
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    debug_with_guard("Call move assign of global heap allocator");
    _logger = other._logger;
    return *this;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    size_t memory_for_user = value_size * values_count;

    debug_with_guard("global heap allocator become alloc " + std::to_string(memory_for_user) + " bytes for user");

    size_t necessary_memory = memory_for_user + sizeof(size_t) + sizeof(allocator*);

    void* allocated_memory;

    try
    {
        allocated_memory = ::operator new(necessary_memory);
    }
    catch (std::bad_alloc& ex)
    {
        error_with_guard("Bad alloc of global heap allocator while trying to alloc " + std::to_string(necessary_memory) + " bytes for user and meta");
        throw;
    }

    *reinterpret_cast<allocator**>(allocated_memory) = this;

    *reinterpret_cast<size_t*>((reinterpret_cast<allocator*>(allocated_memory)+1)) = necessary_memory;

    size_t* tmp = reinterpret_cast<size_t*>((reinterpret_cast<allocator*>(allocated_memory)+1));

    debug_with_guard("global heap allocator finished to allocate " + std::to_string(necessary_memory) + " bytes for user and meta");

    return tmp + 1;

}

void allocator_global_heap::deallocate(
    void *at)
{
    void* removable;

    debug_with_guard("Global heap allocator start to dealloc");

    try
    {
        size_t* block_of_size_t = reinterpret_cast<size_t*>(at) - 1;
        allocator* block_of_alloc_pointer = reinterpret_cast<allocator*>(block_of_size_t) - 1;
        if(block_of_alloc_pointer != this)
        {
            error_with_guard("incorrect void*");
            //throw std::logic_error("block doesn't belong to this allocator!");
        }

        removable = reinterpret_cast<void*>(block_of_alloc_pointer);
    }
    catch(std::logic_error& ex)
    {
        throw;
    }

    ::delete reinterpret_cast<size_t*>(removable);

}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
}

inline std::string allocator_global_heap::get_typename() const
{
    return "allocator_global_heap";

}