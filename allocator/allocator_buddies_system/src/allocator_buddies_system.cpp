#include <not_implemented.h>

#include "../include/allocator_buddies_system.h"

allocator_buddies_system::~allocator_buddies_system()
{
    debug_with_guard("allocator killing");
    get_mutex().~mutex();
    deallocate_with_guard(_trusted_memory);
}

allocator_buddies_system::allocator_buddies_system(
    allocator_buddies_system &&other) noexcept
{
    //log
    _trusted_memory = std::exchange(other._trusted_memory, nullptr);
    //log
}

allocator_buddies_system &allocator_buddies_system::operator=(
    allocator_buddies_system &&other) noexcept
{
    if(this == &other)
    {
        //log
        return *this;
    }
    std::swap(_trusted_memory, other._trusted_memory);
    //log
    return *this;
}

allocator_buddies_system::allocator_buddies_system(
    size_t space_size_power_of_two,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if(space_size_power_of_two < _minimal_k)
    {
        throw std::logic_error("you have entered a size smaller than the metadata of the occupied block");
    }

    size_t real_size = size_by_power_2(space_size_power_of_two) + _meta_allocator;
    if(parent_allocator == nullptr)
    {
        try
        {
            _trusted_memory = ::operator new(real_size);
        }
        catch(std::bad_alloc& ex)
        {
            //log
            throw;
        }
    }
    else
    {
        _trusted_memory = parent_allocator->allocate(real_size, 1);
    }

    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);

    *reinterpret_cast<class logger**>(byte_ptr) = logger;

    byte_ptr += sizeof(class logger*);

    *reinterpret_cast<allocator**>(byte_ptr) = parent_allocator;

    byte_ptr += sizeof(allocator*);

    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(byte_ptr) = allocate_fit_mode;

    byte_ptr += sizeof(allocator_with_fit_mode::fit_mode);

    *reinterpret_cast<unsigned char*>(byte_ptr) = space_size_power_of_two;

    byte_ptr += sizeof(unsigned char);

    auto mut = reinterpret_cast<std::mutex*>(byte_ptr);

    construct(mut);

    byte_ptr += sizeof(std::mutex);

    auto first_block = reinterpret_cast<one_byte_field*>(byte_ptr);

    first_block->is_occup = false;
    first_block->size = space_size_power_of_two - _minimal_k;

}

[[nodiscard]] void *allocator_buddies_system::allocate(
    size_t value_size,
    size_t values_count)
{
    //log
    std::lock_guard lock(get_mutex());

    size_t need_size = values_count * value_size + _meta_ocuupied_block;

    void* free_block;

    switch(get_fit_mode())
    {
        case allocator_with_fit_mode::fit_mode::first_fit:
            free_block = get_first_suitable(need_size);
            break;
        case allocator_with_fit_mode::fit_mode::the_best_fit:
            free_block = get_best_suitable(need_size);
            break;
        case allocator_with_fit_mode::fit_mode::the_worst_fit:
            free_block = get_worst_suitable(need_size);
            break;
    }

    if(free_block == nullptr)
    {
        //log "Bad allocation by twin_allocator for " + std::to_string(need_size) + " bytes"
        throw std::bad_alloc();
    }

    while(get_size_block(free_block) >= need_size * 2)
    {
        auto first_twin = reinterpret_cast<one_byte_field*>(free_block);
        --first_twin->size;
        auto second_twin = reinterpret_cast<one_byte_field*>(get_twin(free_block));
        second_twin->is_occup = false;
        second_twin->size = first_twin->size;
    }

    if(get_size_block(free_block) != need_size)
    {
        //log realloc
    }

    auto first_twin = reinterpret_cast<one_byte_field*>(free_block);
    first_twin->is_occup = true;

    *reinterpret_cast<void**>(first_twin + 1) = _trusted_memory;

    //log
    //log

    return reinterpret_cast<std::byte*>(free_block) + _meta_ocuupied_block;
}

void allocator_buddies_system::deallocate(
    void *at)
{
    std::lock_guard lock(get_mutex());

    void* current_block = reinterpret_cast<std::byte*>(at) - _meta_ocuupied_block;

    if(*reinterpret_cast<void**>(reinterpret_cast<std::byte*>(at) - sizeof(void*)) != _trusted_memory )
    {
        //log
        std::logic_error("not this allocator!");
    }

    size_t current_block_size = get_size_block(current_block) - _meta_ocuupied_block;

    //get dump

    reinterpret_cast<one_byte_field*>(current_block)->is_occup = false;

    void* twin = get_twin(current_block);

    while(get_size_block(current_block) < get_size_full() && get_size_block(current_block) == get_size_block(twin) && !is_busy(twin))
    {
        void* insert_ptr = current_block < twin ? current_block : twin;

        auto current_meta = reinterpret_cast<one_byte_field*>(insert_ptr);
        ++current_meta->size;

        current_block = insert_ptr;
        twin = get_twin(current_block);
    }

    //log
    //log


}

inline void allocator_buddies_system::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    std::lock_guard lock(get_mutex());
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*);
    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(byte_ptr) = mode;
}

std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info(size_t& full_size_avail) const noexcept
{
    std::vector<allocator_test_utils::block_info> result;

    for(auto it = begin_for_iter_twin(), end = end_for_iter_twin(); it != end; ++it)
    {
        result.push_back({it.size(), it.is_block_ocuppied()});
    }

    return result;
}

inline logger *allocator_buddies_system::get_logger() const
{
    return *reinterpret_cast<logger**>(_trusted_memory);
}

inline std::string allocator_buddies_system::get_typename() const noexcept
{
    return "allocator_buddies_system";
}

inline allocator *allocator_buddies_system::get_allocator() const
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*);
    return *reinterpret_cast<allocator**>(byte_ptr);
}

size_t allocator_buddies_system::size_by_power_2(size_t power_of_2) noexcept
{
    constexpr const size_t number = 1;
    return number << power_of_2;
}

size_t allocator_buddies_system::next_power_2(size_t number) noexcept
{
    size_t power = 1;
    while (power < number) {
        power <<= 1;
    }

    return power;
}

inline std::mutex& allocator_buddies_system::get_mutex() noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*)  + sizeof(fit_mode) + sizeof(unsigned char);
    return *reinterpret_cast<std::mutex*>(byte_ptr);
}

void* allocator_buddies_system::get_first_suitable(size_t need_size)  const noexcept
{

    for(auto it = begin_for_iter_twin(), end = end_for_iter_twin(); it != end; ++it)
    {
        if(!it.is_block_ocuppied() && it.size() >= need_size)
        {
            return it.get_ptr_iterator_twin();
        }
    }
    return nullptr;
}

void* allocator_buddies_system::get_worst_suitable(size_t need_size) const noexcept
{
    size_t maxi = 0;
    void* result = nullptr;

    for(auto it = begin_for_iter_twin(), end = end_for_iter_twin(); it != end; ++it)
    {
        if(!it.is_block_ocuppied() && it.size() >= need_size && it.size() >= maxi)
        {
            maxi = it.size();
            result = it.get_ptr_iterator_twin();
        }
    }

    return result;
}

void* allocator_buddies_system::get_best_suitable(size_t need_size)  const noexcept
{
    size_t maximum = get_size_full();
    void* result = nullptr;

    for(auto it = begin_for_iter_twin(), end = end_for_iter_twin(); it != end; ++it)
    {
        if(!it.is_block_ocuppied() && it.size() >= need_size && it.size() <= maximum)
        {
            maximum = it.size();
            result = it.get_ptr_iterator_twin();
        }
    }

    return result;

}

inline size_t allocator_buddies_system::get_size_full() const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);

    byte_ptr += sizeof(logger*) + sizeof(allocator*) + sizeof(fit_mode);

    unsigned char tmp = *reinterpret_cast<unsigned char*>(byte_ptr);

    return size_by_power_2(tmp);
}

inline allocator_with_fit_mode::fit_mode& allocator_buddies_system::get_fit_mode() const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);

    byte_ptr += sizeof(logger*) + sizeof(allocator*);

    return *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(byte_ptr);
}

inline size_t allocator_buddies_system::get_size_block(void* current_block) noexcept
{
    auto byte_ptr = reinterpret_cast<one_byte_field*>(current_block);

    unsigned char tmp = byte_ptr->size;

    return size_by_power_2(tmp + _minimal_k);
}

void* allocator_buddies_system::get_twin(void* current_block) noexcept
{
    size_t relative_distance = reinterpret_cast<std::byte*>(current_block) - (reinterpret_cast<std::byte*>(_trusted_memory) + _meta_allocator);

    size_t distance_bro = relative_distance ^ (static_cast<size_t>(1u) << (reinterpret_cast<one_byte_field*>(current_block)->size + _minimal_k));

    return reinterpret_cast<void*>(reinterpret_cast<std::byte*>(_trusted_memory) + _meta_allocator + distance_bro);
}

bool allocator_buddies_system::is_busy(void* current_block) noexcept
{
    return reinterpret_cast<one_byte_field*>(current_block)->is_occup;
}

allocator_buddies_system::iterator_twin::iterator_twin() : _ptr_block(nullptr) {}

allocator_buddies_system::iterator_twin::iterator_twin(void* ptr) : _ptr_block(ptr) {}

size_t allocator_buddies_system::iterator_twin::size() const noexcept
{
    return get_size_block(_ptr_block);
}

void* allocator_buddies_system::iterator_twin::get_ptr_iterator_twin() const noexcept
{
    return _ptr_block;
}

bool allocator_buddies_system::iterator_twin::is_block_ocuppied() const noexcept
{
    return is_busy(_ptr_block);
}

allocator_buddies_system::iterator_twin allocator_buddies_system::begin_for_iter_twin() const noexcept
{
    return {reinterpret_cast<std::byte*>(_trusted_memory) + _meta_allocator};
}

allocator_buddies_system::iterator_twin allocator_buddies_system::end_for_iter_twin() const noexcept
{
    return {reinterpret_cast<std::byte*>(_trusted_memory) + _meta_allocator + get_size_full()};
}

allocator_buddies_system::iterator_twin& allocator_buddies_system::iterator_twin::operator++() noexcept
{
    _ptr_block = reinterpret_cast<std::byte*>(_ptr_block) + get_size_block(_ptr_block);
    return *this;
}

allocator_buddies_system::iterator_twin allocator_buddies_system::iterator_twin::operator++(int) noexcept
{
    auto temporary = *this;
    ++(*this);
    return temporary;
}

bool allocator_buddies_system::iterator_twin::operator==(const allocator_buddies_system::iterator_twin &oth) const noexcept
{
    return _ptr_block == oth._ptr_block;
}

bool allocator_buddies_system::iterator_twin::operator!=(const allocator_buddies_system::iterator_twin &oth) const noexcept
{
    return !(*this == oth);
}