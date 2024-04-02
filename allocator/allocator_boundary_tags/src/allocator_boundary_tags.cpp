#include <not_implemented.h>

#include "../include/allocator_boundary_tags.h"

allocator_boundary_tags::~allocator_boundary_tags()
{
    debug_with_guard("allocator deallocating");
    get_mutex().~mutex();
    deallocate_with_guard(_trusted_memory);
}


allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags &&other) noexcept
{
    _trusted_memory = std::exchange(other._trusted_memory, nullptr);
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags &&other) noexcept
{
    std::swap(_trusted_memory, other._trusted_memory);
    return *this;
}

allocator_boundary_tags::allocator_boundary_tags(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    size_t real_size_for_allocator = _size_allocator_meta + space_size;

    if(parent_allocator == nullptr)
    {
        try
        {
            _trusted_memory = ::operator new(real_size_for_allocator);
        }
        catch (std::bad_alloc& ex){
            error_with_guard("bad alloc");
            throw;
        }
    }
    else
    {
        _trusted_memory = parent_allocator->allocate(real_size_for_allocator, 1);
    }

    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);

    *reinterpret_cast<class logger**>(byte_ptr) = logger;

    byte_ptr += sizeof(class logger*);

    *reinterpret_cast<allocator**>(byte_ptr) = parent_allocator;

    byte_ptr += sizeof(allocator*);

    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(byte_ptr) = allocate_fit_mode;

    byte_ptr += sizeof(allocator_with_fit_mode::fit_mode);

    *reinterpret_cast<size_t*>(byte_ptr) = space_size;

    byte_ptr += sizeof(size_t);

    auto mut = reinterpret_cast<std::mutex*>(byte_ptr);

    construct(mut);

    byte_ptr += sizeof(std::mutex);

    *reinterpret_cast<void**>(byte_ptr) = nullptr;
}

[[nodiscard]] void *allocator_boundary_tags::allocate(
    size_t value_size,
    size_t values_count)
{

    std::lock_guard lock(get_mutex());

    size_t need_size_for_block = _size_load_block_meta + value_size * values_count;//мету уже здесь учитываем!!!

    void* previous_loaded_block;

    switch(get_fit_mode())
    {
        case allocator_with_fit_mode::fit_mode::first_fit:
            previous_loaded_block = get_first_suitable(need_size_for_block);
            break;
        case allocator_with_fit_mode::fit_mode::the_best_fit:
            previous_loaded_block = get_best_suitable(need_size_for_block);
            break;
        case allocator_with_fit_mode::fit_mode::the_worst_fit:
            previous_loaded_block = get_worst_suitable(need_size_for_block);
            break;
    }

    if(previous_loaded_block == nullptr)
    {
        error_with_guard("bad allocation for " + std::to_string(need_size_for_block) + " bytes");
        throw std::bad_alloc();
    }

    size_t size_free_block = get_next_free_size(previous_loaded_block);

    if( size_free_block > need_size_for_block) //это на случай, если самый подходящий блок памяти оказался больше
    {
        warning_with_guard("Allocator with boundary tags changed allocating block size to " + std::to_string(size_free_block));
        need_size_for_block = size_free_block;
    }

    void* free_block_start;

    if(previous_loaded_block == _trusted_memory)
    {
        free_block_start = reinterpret_cast<std::byte*>(_trusted_memory) + _size_allocator_meta;
    }
    else
    {
        free_block_start = reinterpret_cast<std::byte*>(previous_loaded_block) + get_size_current_load_block(previous_loaded_block) + _size_load_block_meta;
    }

    auto size_new_block = reinterpret_cast<size_t*>(free_block_start);

    *size_new_block = value_size * values_count;

    auto back_ptr = reinterpret_cast<void**>(size_new_block + 1);

    *back_ptr = previous_loaded_block;

    auto front_ptr = reinterpret_cast<void**>(back_ptr + 1);

    *front_ptr = previous_loaded_block == _trusted_memory ? *get_first_block() : get_next_load_block(previous_loaded_block);

    auto parent_ptr = reinterpret_cast<void**>(front_ptr + 1);

    *parent_ptr = _trusted_memory;

    void* next_block;

    if(previous_loaded_block == _trusted_memory)
    {
        next_block = *get_first_block();
    }
    else
    {
        next_block = get_next_load_block(previous_loaded_block);
    }

    if(next_block != nullptr)
    {
        auto byte_ptr = reinterpret_cast<std::byte*>(next_block);
        byte_ptr += sizeof(size_t);
        *reinterpret_cast<void**>(byte_ptr) = free_block_start;
    }

    if(previous_loaded_block == _trusted_memory)
    {
        *get_first_block() = free_block_start;
    }
    else
    {
        auto byte_ptr = reinterpret_cast<std::byte*>(previous_loaded_block);
        byte_ptr += sizeof(size_t) + sizeof(void*);
        *reinterpret_cast<void**>(byte_ptr) = free_block_start;
    }

    debug_with_guard("allocator allocating " + std::to_string(need_size_for_block) + " bytes");
    return reinterpret_cast<std::byte*>(free_block_start) + _size_load_block_meta;
}

void allocator_boundary_tags::deallocate(
    void *at)
{
    std::lock_guard lock(get_mutex());

    void* block_start = reinterpret_cast<std::byte*>(at) - _size_load_block_meta;

    void* parent_ptr = reinterpret_cast<std::byte*>(block_start) + sizeof(size_t) + 2*sizeof(void*);

    if(parent_ptr != _trusted_memory)
    {
        error_with_guard("invalid deallocating obj");
        throw std::logic_error("invalid deallocating obj");
    }

    void* previous = reinterpret_cast<std::byte*>(block_start) + sizeof(size_t);
    void* next = reinterpret_cast<std::byte*>(block_start) + sizeof(size_t) + sizeof(void*);

    if(previous == _trusted_memory)
    {
        *get_first_block() = next;
    }
    else
    {
        auto byte_ptr = reinterpret_cast<std::byte*>(previous);
        byte_ptr += sizeof(size_t) + sizeof(void*);
        *reinterpret_cast<void**>(byte_ptr) = next;
    }

    if(next != nullptr)
    {
        auto byte_ptr = reinterpret_cast<std::byte*>(next);
        byte_ptr += sizeof(size_t) + sizeof(void*);
        *reinterpret_cast<void**>(byte_ptr) = previous;
    }

    information_with_guard("allocator dealloc");
    //log

}

inline void allocator_boundary_tags::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*);
    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(byte_ptr) = mode;
}

inline allocator *allocator_boundary_tags::get_allocator() const
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*);
    return *reinterpret_cast<allocator**>(byte_ptr);
}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept", "your code should be here...");
}

inline logger *allocator_boundary_tags::get_logger() const
{
    return reinterpret_cast<logger*>(_trusted_memory);
}

inline std::string allocator_boundary_tags::get_typename() const noexcept
{
    return "allocator_boundary_tags";
}

inline std::mutex&  allocator_boundary_tags::get_mutex() noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*)  + sizeof(fit_mode) + sizeof(size_t);
    return *reinterpret_cast<std::mutex*>(byte_ptr);
}

inline allocator_with_fit_mode::fit_mode& allocator_boundary_tags::get_fit_mode() const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*);
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(byte_ptr);
}

void* allocator_boundary_tags::get_first_suitable(size_t need_size)  const noexcept //на предыдущий блок
{
    auto first_block = *get_first_block();

    if(first_block == nullptr)
    {
        void* end = reinterpret_cast<std::byte*>(_trusted_memory) + get_size_full();
        void* start = reinterpret_cast<std::byte*>(_trusted_memory) + _size_allocator_meta;

        size_t available_size = reinterpret_cast<std::byte*>(end) - reinterpret_cast<std::byte*>(start);
        if(available_size < need_size)
        {
            return nullptr;
        }
        return _trusted_memory;
    }


}

void* allocator_boundary_tags::get_worst_suitable(size_t need_size) const noexcept //на предыдущий блок
{
    return nullptr;
}

void* allocator_boundary_tags::get_best_suitable(size_t need_size)  const noexcept //на предыдущий блок
{
    return nullptr;
}

inline size_t allocator_boundary_tags::get_size_full() const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*) + sizeof(fit_mode);
    return *reinterpret_cast<size_t*>(byte_ptr);
}

inline size_t allocator_boundary_tags::get_size_current_load_block(void* current_block) const noexcept
{
    return *reinterpret_cast<size_t*>(current_block);
}

inline void* allocator_boundary_tags::get_next_load_block(void* current_block) const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(current_block);
    byte_ptr += sizeof(size_t) + sizeof(void*);
    return reinterpret_cast<void*>(byte_ptr);
}

inline void* allocator_boundary_tags::get_prev_load_block(void* current_block) const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(current_block);
    byte_ptr += sizeof(size_t);
    return reinterpret_cast<void*>(byte_ptr);
}

inline void* allocator_boundary_tags::get_parrent_for_current_load_block(void* current_block) const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(current_block);
    byte_ptr += sizeof(size_t) + sizeof(void*) + sizeof(void*);
    return reinterpret_cast<void*>(byte_ptr);
}

inline void** allocator_boundary_tags::get_first_block() const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*) + sizeof(fit_mode) + sizeof(size_t) + sizeof(std::mutex);
    return reinterpret_cast<void**>(byte_ptr);
}

inline size_t allocator_boundary_tags::get_next_free_size(void* loaded_block) const noexcept
{
    void* next_block;

    if(loaded_block == _trusted_memory)
    {
        next_block = get_first_block();
    }
    else
    {
        next_block = get_next_load_block(loaded_block);
    }

    if(next_block == nullptr)
    {
        void* end_adress_allocator = reinterpret_cast<std::byte*>(_trusted_memory) + get_size_full();
        void* end_adress_current_block = reinterpret_cast<std::byte*>(loaded_block) + get_size_current_load_block(loaded_block);
        return static_cast<size_t>(reinterpret_cast<std::byte*>(end_adress_allocator) - reinterpret_cast<std::byte*>(end_adress_current_block));
    }

    void* next_load_block = get_next_load_block(loaded_block);
    void* end_adress_current_block = reinterpret_cast<std::byte*>(loaded_block) + get_size_current_load_block(loaded_block);

    return static_cast<size_t>(reinterpret_cast<std::byte*>(next_load_block) - reinterpret_cast<std::byte*>(end_adress_current_block));
}
