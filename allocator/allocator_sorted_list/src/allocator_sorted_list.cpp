#include <not_implemented.h>

#include "../include/allocator_sorted_list.h"

allocator_sorted_list::~allocator_sorted_list()
{
    throw not_implemented("allocator_sorted_list::~allocator_sorted_list()", "your code should be here...");
}

//allocator_sorted_list::allocator_sorted_list(
//    allocator_sorted_list const &other)
//{
//    throw not_implemented("allocator_sorted_list::allocator_sorted_list(allocator_sorted_list const &)", "your code should be here...");
//}
//
//allocator_sorted_list &allocator_sorted_list::operator=(
//    allocator_sorted_list const &other)
//{
//    throw not_implemented("allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list const &)", "your code should be here...");
//}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list &&other) noexcept
{
    //log
    _trusted_memory = std::exchange(other._trusted_memory, nullptr);
    //log
}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list &&other) noexcept
{
    if(this == &other)
        return *this;
    std::swap(_trusted_memory,other._trusted_memory);
    return *this;
}

allocator_sorted_list::allocator_sorted_list(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    size_t real_size_for_allocator = space_size + _meta_allocator;

    if(parent_allocator == nullptr)
    {
        try
        {
            _trusted_memory = ::operator new(real_size_for_allocator);
        }
        catch(std::bad_alloc& ex)
        {
            //log
            throw;
        }
    }
    else
    {
        _trusted_memory = parent_allocator->allocate(real_size_for_allocator, 1);
    }

    std::byte* byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);

    *reinterpret_cast<class logger**>(byte_ptr) = logger;

    byte_ptr += sizeof(class logger*);

    *reinterpret_cast<allocator**>(byte_ptr) = parent_allocator;

    byte_ptr += sizeof(allocator*);

    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(byte_ptr) = allocate_fit_mode;

    byte_ptr += sizeof(allocator_with_fit_mode::fit_mode);

    *reinterpret_cast<size_t*>(byte_ptr) = space_size;

    byte_ptr += sizeof(size_t);

    std::mutex* mut = reinterpret_cast<std::mutex*>(byte_ptr);

    construct(mut);

    byte_ptr += sizeof(std::mutex);

    *reinterpret_cast<void**>(byte_ptr) = nullptr;

    //log
}

[[nodiscard]] void *allocator_sorted_list::allocate(
    size_t value_size,
    size_t values_count)
{

}

void allocator_sorted_list::deallocate(
    void *at)
{

}

inline void allocator_sorted_list::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    std::lock_guard lock(get_mutex());
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*);
    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(byte_ptr) = mode;
}

inline allocator *allocator_sorted_list::get_allocator() const
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*);
    return *reinterpret_cast<allocator**>(byte_ptr);
}

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info(size_t& full_size_avail) const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept", "your code should be here...");
}

inline logger *allocator_sorted_list::get_logger() const
{
    return *reinterpret_cast<logger**>(_trusted_memory);
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    return "allocator_sorted_list";
}

inline std::mutex& allocator_sorted_list::get_mutex() noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*)  + sizeof(fit_mode) + sizeof(size_t);
    return *reinterpret_cast<std::mutex*>(byte_ptr);
}

inline allocator_with_fit_mode::fit_mode& allocator_sorted_list::get_fit_mode() const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*);
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(byte_ptr);
}

inline size_t allocator_sorted_list::get_size_full() const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*) + sizeof(fit_mode);
    return *reinterpret_cast<size_t*>(byte_ptr);
}

std::string allocator_sorted_list::get_dump(char* at, size_t size)
{
    std::string result;
    for(size_t i = 0; i < size; ++i)
    {
        result += std::to_string(static_cast<int>(at[i])) + " ";
    }
    return result;
}

inline void** allocator_sorted_list::get_first_block() const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex);
    return reinterpret_cast<void**>(byte_ptr);
}

allocator_sorted_list::iterator_free_block::iterator_free_block() : _ptr_free(nullptr) {}

allocator_sorted_list::iterator_free_block::iterator_free_block(void *ptr) : _ptr_free(ptr)
{

}
