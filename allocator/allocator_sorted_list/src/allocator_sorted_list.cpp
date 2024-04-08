#include <not_implemented.h>

#include "../include/allocator_sorted_list.h"

allocator_sorted_list::~allocator_sorted_list()
{
    debug_with_guard("allocator killing");
    get_mutex().~mutex();
    deallocate_with_guard(_trusted_memory);
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list &&other) noexcept
{
    debug_with_guard("move copy start");
    _trusted_memory = std::exchange(other._trusted_memory, nullptr);
    debug_with_guard("move copy finish");
}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list &&other) noexcept
{
    debug_with_guard("assign move start");
    if(this == &other) {
        debug_with_guard("assign move finish");
        return *this;
    }
    std::swap(_trusted_memory,other._trusted_memory);
    debug_with_guard("assign move finish");
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
            error_with_guard("bad alloc for it allocator!!!");
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

    *reinterpret_cast<void**>(byte_ptr) = byte_ptr + sizeof(void*);

    byte_ptr += sizeof(void*);

    *reinterpret_cast<size_t*>(byte_ptr) = space_size - _meta_block;//////////////////////////

    byte_ptr += sizeof(size_t);

    *reinterpret_cast<void**>(byte_ptr) = nullptr;

    debug_with_guard("constructor finish");
}

[[nodiscard]] void *allocator_sorted_list::allocate(
    size_t value_size,
    size_t values_count)
{
    trace_with_guard("allocate memory start");
    size_t full_avai;
    get_blocks_info(full_avai);
    information_with_guard("current available memory: " + std::to_string(full_avai));
    information_with_guard("allocator before allocated: current condition of blocks: " + get_info_in_string(get_blocks_info(full_avai)));


    std::lock_guard lock(get_mutex());

    size_t need_size_for_block = value_size * values_count/* + _meta_block*/;

    void* previous_block;

    switch (get_fit_mode())
    {
        case allocator_sorted_list::fit_mode::first_fit:
            previous_block = get_first_suitable(need_size_for_block);
            break;
        case allocator_sorted_list::fit_mode::the_best_fit:
            previous_block = get_best_suitable(need_size_for_block);
            break;
        case allocator_sorted_list::fit_mode::the_worst_fit:
            previous_block = get_worst_suitable(need_size_for_block);
            break;
    }

    if(previous_block == nullptr)
    {
        error_with_guard("bad allocation for " + std::to_string(need_size_for_block) + " bytes");
        throw std::bad_alloc();
    }

    void* current_block;

    if(previous_block == _trusted_memory)
    {
        current_block = *get_first_block(_trusted_memory);
    }
    else
    {
        current_block = get_ptr_from_block(previous_block);
    }

    void* next_block = get_ptr_from_block(current_block);

    size_t size_free_block = get_size_block(current_block);

    if(size_free_block < need_size_for_block + _meta_block)
    {
        warning_with_guard("Allocator with boundary tags changed allocating block size to " + std::to_string(size_free_block));
        need_size_for_block = size_free_block;
    }

    auto size_new_block = reinterpret_cast<size_t*>(current_block);

    *size_new_block = need_size_for_block/* - _meta_block*/;

    auto parent_ptr = reinterpret_cast<void**>(size_new_block + 1);

    *parent_ptr = _trusted_memory;

    if(next_block == nullptr)
    {
        auto byte_ptr = reinterpret_cast<std::byte *>(current_block);
        byte_ptr += _meta_block + get_size_block(current_block);
        *get_ptr_previous(previous_block) = reinterpret_cast<void *>(byte_ptr);
        *reinterpret_cast<size_t *>(byte_ptr) =
                reinterpret_cast<std::byte *>(reinterpret_cast<std::byte *>(_trusted_memory) + _meta_allocator +
                                              get_size_full()) -
                reinterpret_cast<std::byte *>(byte_ptr + sizeof(size_t) + sizeof(void *));
        byte_ptr += sizeof(size_t);
        *reinterpret_cast<void **>(byte_ptr) = nullptr;
    }
    else
    {
        if(size_free_block - need_size_for_block > _meta_block)
        {
            auto byte_ptr = reinterpret_cast<std::byte*>(current_block) + _meta_block + get_size_block(current_block);
            *reinterpret_cast<size_t*>(byte_ptr) = size_free_block - need_size_for_block - _meta_block;
            byte_ptr += sizeof(size_t);
            *reinterpret_cast<void**>(byte_ptr) = next_block;
            next_block = reinterpret_cast<std::byte*>(current_block) + _meta_block + get_size_block(current_block);
        }
        *get_ptr_previous(previous_block) = next_block;
    }

    trace_with_guard("allocator allocating " + std::to_string(need_size_for_block) + " bytes");
    trace_with_guard("allocate memory finish");
    get_blocks_info(full_avai);
    information_with_guard("current available memory: " + std::to_string(full_avai));
    information_with_guard("allocator deallocated: current condition of blocks: " + get_info_in_string(get_blocks_info(full_avai)));


    return reinterpret_cast<std::byte*>(current_block) + _meta_block;
}

void allocator_sorted_list::deallocate(
    void *at)
{
    trace_with_guard("deallocate memory start");
    size_t full_avai;
    get_blocks_info(full_avai);
    information_with_guard("current available memory: " + std::to_string(full_avai));
    information_with_guard("allocator deallocated: current condition of blocks: " + get_info_in_string(get_blocks_info(full_avai)));


    std::lock_guard lock(get_mutex());

    void* block_start = reinterpret_cast<std::byte*>(at) - _meta_block;

    if(get_ptr_from_block(block_start) != _trusted_memory)
    {
        error_with_guard("You wanna delete a block that does not belong to this allocator");
        throw std::logic_error("You wanna delete a block that does not belong to this allocator");
    }

    debug_with_guard("condition of block before deallocate: " + get_dump(reinterpret_cast<char*>(at), get_size_block(block_start)));

    void* previous_free = get_previous_for_loaded(block_start);

    void* next_free;

    if(previous_free == _trusted_memory)
    {
        next_free = *get_first_block(_trusted_memory);
    }
    else
    {
        next_free = get_ptr_from_block(previous_free);
    }

    if(next_free != nullptr)//////////////////////////////////////////отсюда проверять!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        if(reinterpret_cast<void*>(reinterpret_cast<std::byte*>(block_start) + _meta_block + get_size_block(block_start)) == next_free)
        {
            auto byte_ptr = reinterpret_cast<std::byte*>(block_start);
            *reinterpret_cast<size_t*>(byte_ptr) = get_size_block(block_start) + _meta_block + get_size_block(next_free);
            byte_ptr += sizeof(size_t);
            *reinterpret_cast<void**>(byte_ptr) = get_ptr_from_block(next_free);
            next_free = get_ptr_from_block(next_free);
        }

    }
    *get_ptr_previous(previous_free) = block_start;
    *get_ptr_previous(block_start) = next_free;


    size_t tmp;
    information_with_guard("allocator deallocated: current condition of blocks: " + get_info_in_string(get_blocks_info(tmp)));

    trace_with_guard("deallocate memory finish");

    get_blocks_info(full_avai);
    information_with_guard("current available memory: " + std::to_string(full_avai));
    information_with_guard("allocator deallocated: current condition of blocks: " + get_info_in_string(get_blocks_info(full_avai)));

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

std::string allocator_sorted_list::get_info_in_string(const std::vector<allocator_test_utils::block_info>& vec) noexcept
{
    std::ostringstream str;
    for(auto& it : vec)
    {
        if(it.is_block_occupied)
        {
            str << "<occup>";
        }
        else
            str << "<avail>";

        str << "<"+ std::to_string(it.block_size) + "> | ";
    }
    return str.str();
}

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info(size_t& full_size_avail) const noexcept
{
    full_size_avail = 0;

    std::vector<allocator_test_utils::block_info> result;
    void* prev = _trusted_memory;

    if(begin_for_free_iter() == end_for_free_iter())
    {
        auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory) + _meta_allocator;
        auto byte_end = reinterpret_cast<std::byte*>(_trusted_memory) + _meta_allocator + get_size_full();
        while(reinterpret_cast<void*>(byte_ptr) != byte_end)
        {
            size_t occup_size = get_size_block(byte_ptr);
            result.push_back({occup_size, true});
            byte_ptr += _meta_block + occup_size;
        }
    }

    for(auto it = begin_for_free_iter(), end = end_for_free_iter(); it != end; ++it)
    {

        size_t meta = prev == _trusted_memory ? _meta_allocator : _meta_block + get_size_block(prev);

//        if(it.get_ptr_free_block() != reinterpret_cast<void*>(reinterpret_cast<std::byte*>(prev) + meta))
//        {
        auto byte_ptr = reinterpret_cast<std::byte*>(prev) + meta;

        while(reinterpret_cast<void*>(byte_ptr) != it.get_ptr_free_block())
        {
            size_t occup_size = get_size_block(byte_ptr);
            result.push_back({occup_size, true});
            byte_ptr += _meta_block + occup_size;
        }
        //}

        full_size_avail += it.size();
        result.push_back({it.size(), false});

        prev = it.get_ptr_free_block();

        if(*reinterpret_cast<void**>(reinterpret_cast<std::byte*>(it.get_ptr_free_block()) + sizeof(size_t)) == nullptr)
        {
            //full_size_avail += get_size_block(it.get_ptr_free_block());
            //result.push_back({get_size_block(it.get_ptr_free_block()), false});
            if(reinterpret_cast<std::byte*>(_trusted_memory) + _meta_allocator + get_size_full() != reinterpret_cast<std::byte*>(it.get_ptr_free_block()) + _meta_block +
                                                                                                    it.size())
            {
                auto byte_ptr = reinterpret_cast<std::byte*>(it.get_ptr_free_block()) + _meta_block + it.size();
                auto byte_end = reinterpret_cast<std::byte*>(_trusted_memory) + _meta_allocator + get_size_full();
                while(reinterpret_cast<void*>(byte_ptr) != byte_end)
                {
                    size_t occup_size = get_size_block(byte_ptr);
                    result.push_back({occup_size, true});
                    byte_ptr += _meta_block + occup_size;
                }
            }
        }

    }


    return result;

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

void* allocator_sorted_list::get_first_suitable(size_t need_size)  const noexcept
{
    void* result = _trusted_memory;

    for(auto it = begin_for_free_iter(), end = end_for_free_iter(); it != end; ++it)
    {

        if(it.size() >= need_size)
        {
            return result;
        }
        result = it.get_ptr_free_block();
    }
    return nullptr;
}

void* allocator_sorted_list::get_worst_suitable(size_t need_size) const noexcept
{
    size_t max = 0;
    void* result = nullptr;
    void* prev = _trusted_memory;
    for(auto it = begin_for_free_iter(), end = end_for_free_iter(); it != end; ++it)
    {
        size_t current_size = it.size();
        if(current_size >= need_size && current_size > max)
        {
            max = current_size;
            result = prev;
        }
        prev = it.get_ptr_free_block();
    }

    return result;
}

void* allocator_sorted_list::get_best_suitable(size_t need_size)  const noexcept
{
    size_t max = get_size_full();
    void* result = nullptr;
    void* prev = _trusted_memory;
    for(auto it = begin_for_free_iter(), end = end_for_free_iter(); it != end; ++it)
    {
        size_t current_size = it.size();
        if(current_size >= need_size && current_size < max)
        {
            max = current_size;
            result = prev;
        }
        prev = it.get_ptr_free_block();
    }
    return result;
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

inline void** allocator_sorted_list::get_first_block(void* ptr) noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(ptr);
    byte_ptr += sizeof(logger*) + sizeof(allocator*) + sizeof(allocator_with_fit_mode::fit_mode) +sizeof(size_t) + sizeof(std::mutex);
    return reinterpret_cast<void**>(byte_ptr);
}

inline void* allocator_sorted_list::get_ptr_from_block(void* current) noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(current);
    byte_ptr += sizeof(size_t);
    return *reinterpret_cast<void**>(byte_ptr);
}

inline void** allocator_sorted_list::get_ptr_previous(void* ptr) const noexcept
{
    if(ptr == _trusted_memory)
    {
        auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
        byte_ptr += sizeof(logger*) + sizeof(allocator*) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(size_t) + sizeof(std::mutex);
        return reinterpret_cast<void**>(byte_ptr);
    }

    auto byte_ptr = reinterpret_cast<std::byte*>(ptr);
    byte_ptr += sizeof(size_t);
    return reinterpret_cast<void**>(byte_ptr);
}

inline void* allocator_sorted_list::get_previous_for_loaded(void* loaded_ptr) const noexcept
{
    void* prev = _trusted_memory;

    for(auto it = begin_for_free_iter(), end = end_for_free_iter(); it != end; ++it)
    {
        ptrdiff_t diff = reinterpret_cast<std::byte*>(it.get_ptr_free_block()) - reinterpret_cast<std::byte*>(loaded_ptr);

        if(diff > 0)
        {
            return prev;
        }

        prev = it.get_ptr_free_block();
    }
    return prev;
}

//inline void* allocator_sorted_list::get_next_free_block(void* current_block) noexcept
//{
//    return get_ptr_from_block(current_block);
//}

inline size_t allocator_sorted_list::get_size_block(void* current_block) noexcept
{
    return *reinterpret_cast<size_t*>(current_block);
}

inline void** allocator_sorted_list::get_parrent_for_current_load_block(void* current_block) const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(current_block);
    byte_ptr += sizeof(size_t);
    return reinterpret_cast<void**>(byte_ptr);
}

allocator_sorted_list::iterator_free_block::iterator_free_block() : _ptr_free(nullptr) {}

allocator_sorted_list::iterator_free_block::iterator_free_block(void *ptr) : _ptr_free(*get_first_block(ptr)) {}

bool allocator_sorted_list::iterator_free_block::operator==(const allocator_sorted_list::iterator_free_block& oth) const noexcept
{
    return _ptr_free == oth._ptr_free;
}

bool allocator_sorted_list::iterator_free_block::operator!=(const allocator_sorted_list::iterator_free_block& oth) const noexcept
{
    return !(*this == oth);
}

allocator_sorted_list::iterator_free_block& allocator_sorted_list::iterator_free_block::operator++() noexcept
{
    _ptr_free = get_ptr_from_block(_ptr_free);
    return *this;
}

allocator_sorted_list::iterator_free_block allocator_sorted_list::iterator_free_block::operator++(int) noexcept
{
    auto temporary = *this;
    ++(*this);
    return temporary;
}

allocator_sorted_list::iterator_free_block allocator_sorted_list::begin_for_free_iter() const noexcept
{
    return {_trusted_memory};
}

allocator_sorted_list::iterator_free_block allocator_sorted_list::end_for_free_iter() const noexcept
{
    return {};
}

size_t allocator_sorted_list::iterator_free_block::size()
{
    return get_size_block(_ptr_free);
}

void* allocator_sorted_list::iterator_free_block::get_ptr_free_block() const noexcept
{
    return _ptr_free;
}