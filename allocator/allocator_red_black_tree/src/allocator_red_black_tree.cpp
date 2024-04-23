#include <not_implemented.h>

#include "../include/allocator_red_black_tree.h"

allocator_red_black_tree::~allocator_red_black_tree()
{
    debug_with_guard("allocator killing");
    get_mutex().~mutex();
    deallocate_with_guard(_trusted_memory);
}


allocator_red_black_tree::allocator_red_black_tree(
    allocator_red_black_tree &&other) noexcept
{
    _trusted_memory = std::exchange(other._trusted_memory, nullptr);
}

allocator_red_black_tree &allocator_red_black_tree::operator=(
    allocator_red_black_tree &&other) noexcept
{
    if(this != &other)
    {
        std::swap(_trusted_memory, other._trusted_memory);
    }
    return *this;
}

allocator_red_black_tree::allocator_red_black_tree(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    size_t real_size_for_allocator = space_size + _meta_allocator;

    if(parent_allocator == nullptr)
    {
        try {
            _trusted_memory = ::operator new(real_size_for_allocator);
        }
        catch(std::bad_alloc& ex)
        {
            error_with_guard("bad allocation for allocator from global heap!!!");
            throw;
        }
    }
    else
    {
       try {
           _trusted_memory = parent_allocator->allocate(real_size_for_allocator, 1);

       }
       catch(...)
       {
           error_with_guard("bad allocation for allocator from parent allocator!!!");
           throw;
       }
    }

    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);

    *reinterpret_cast<class logger**>(byte_ptr) = logger;

    byte_ptr += sizeof(class logger);

    *reinterpret_cast<allocator**>(byte_ptr) = parent_allocator;

    byte_ptr += sizeof(allocator*);

    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(byte_ptr) = allocate_fit_mode;

    byte_ptr += sizeof(allocator_with_fit_mode::fit_mode);

    *reinterpret_cast<size_t*>(byte_ptr) = space_size;

    byte_ptr += sizeof(size_t);

    auto mut = reinterpret_cast<std::mutex*>(byte_ptr);

    construct(mut);

    byte_ptr += sizeof(std::mutex);

    *reinterpret_cast<void**>(byte_ptr) == byte_ptr + sizeof(void*);

    byte_ptr += sizeof(void*);

    auto first_free_block = reinterpret_cast<void*>(byte_ptr);

    get_occup_color(first_free_block).occupied = false;
    get_occup_color(first_free_block)._color = color::BLACK;
    get_back(first_free_block) = nullptr;
    get_forward(first_free_block) = nullptr;
    get_parent(first_free_block) = nullptr;
    get_left(first_free_block) = nullptr;
    get_right(first_free_block) = nullptr;

}

[[nodiscard]] void *allocator_red_black_tree::allocate(
    size_t value_size,
    size_t values_count)
{
    std::lock_guard lock(get_mutex());

    size_t need_size = value_size * values_count;

    void* suitable_free;

    switch(get_fit_mode())
    {
        case allocator_with_fit_mode::fit_mode::first_fit:
            //suitable_free = get_first_suitable(need_size);
            break;
        case allocator_with_fit_mode::fit_mode::the_best_fit:
            //suitable_free = get_best_suitable(need_size);
            break;
        case allocator_with_fit_mode::fit_mode::the_worst_fit:
            //suitable_free = get_worst_suitable(need_size);
            break;
    }

    if(suitable_free == nullptr)
    {
        //log
        throw std::bad_alloc();
    }





}

void allocator_red_black_tree::deallocate(
    void *at)
{
    throw not_implemented("void allocator_red_black_tree::deallocate(void *)", "your code should be here...");
}

inline void allocator_red_black_tree::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    std::lock_guard lock(get_mutex());
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*);
    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(byte_ptr) = mode;
}

std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept
{

}

inline allocator *allocator_red_black_tree::get_allocator() const
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*);
    return *reinterpret_cast<allocator**>(byte_ptr);
}

inline logger *allocator_red_black_tree::get_logger() const
{
    return *reinterpret_cast<logger**>(_trusted_memory);
}

inline std::string allocator_red_black_tree::get_typename() const noexcept
{
    return "allocator_red_black_tree";
}

inline std::mutex& allocator_red_black_tree::get_mutex() noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);
    byte_ptr += sizeof(logger*) + sizeof(allocator*)  + sizeof(fit_mode) + sizeof(size_t);
    return *reinterpret_cast<std::mutex*>(byte_ptr);
}

inline allocator_with_fit_mode::fit_mode& allocator_red_black_tree::get_fit_mode() const noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(_trusted_memory);

    byte_ptr += sizeof(logger*) + sizeof(allocator*);

    return *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(byte_ptr);
}

inline size_t allocator_red_black_tree::get_size_full(void* trusted)noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(trusted);

    byte_ptr += sizeof(logger*) + sizeof(allocator*) + sizeof(fit_mode);

    return *reinterpret_cast<size_t*>(byte_ptr);
}

inline size_t allocator_red_black_tree::get_size_block(void* current_block, void* trusted_mem) noexcept
{
    if(get_forward(current_block) == nullptr)
    {
        return reinterpret_cast<std::byte*>(trusted_mem) + _meta_allocator + get_size_full(trusted_mem) - reinterpret_cast<std::byte*>(current_block) - _meta_occup_block;
    }
    return reinterpret_cast<std::byte*>(get_forward(current_block)) - reinterpret_cast<std::byte*>(current_block) - _meta_occup_block;

}

void*& allocator_red_black_tree::get_forward(void* current_block) noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(current_block);

    byte_ptr += sizeof(occup_color_block) + sizeof(void*);

    return *reinterpret_cast<void**>(byte_ptr);
}

void*& allocator_red_black_tree::get_back(void* current_block) noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(current_block);

    byte_ptr += sizeof(occup_color_block);

    return *reinterpret_cast<void**>(byte_ptr);
}

allocator_red_black_tree::occup_color_block& allocator_red_black_tree::get_occup_color(void* current_block) noexcept
{
    return *reinterpret_cast<occup_color_block*>(current_block);
}

void*& allocator_red_black_tree::get_parent(void* current_block) noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(current_block);

    byte_ptr += sizeof(occup_color_block) + 2*sizeof(void*);

    return *reinterpret_cast<void**>(byte_ptr);
}

void*& allocator_red_black_tree::get_left(void* current_block) noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(current_block);

    byte_ptr += sizeof(occup_color_block) + 3 * sizeof(void*);

    return *reinterpret_cast<void**>(byte_ptr);
}

void*& allocator_red_black_tree::get_right(void* current_block) noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(current_block);

    byte_ptr += sizeof(occup_color_block) + 4 * sizeof(void*);

    return *reinterpret_cast<void**>(byte_ptr);
}

void** allocator_red_black_tree::get_first_block(void* trusted_mem) noexcept
{
    auto byte_ptr = reinterpret_cast<std::byte*>(trusted_mem);

    byte_ptr += sizeof(logger*) + sizeof(allocator*) + sizeof(fit_mode) + sizeof(size_t) + sizeof(std::mutex);

    return reinterpret_cast<void**>(byte_ptr);
}

bool allocator_red_black_tree::is_occup(void* current_block) noexcept
{
    return get_occup_color(current_block).occupied;
}

void allocator_red_black_tree::update_parent_ptr(void* current_block, void* new_parent) noexcept
{
    if(get_parent(current_block) == nullptr)
    {
        *get_first_block(_trusted_memory) = new_parent;
    }
    else
    {
        if(is_left_subtree(current_block, get_parent(current_block)))
        {
            get_left(get_parent(current_block)) = new_parent;
        }
        else
        {
            get_right(get_parent(current_block)) = new_parent;
        }
    }
}

bool allocator_red_black_tree::is_left_subtree(void* child, void* parent) noexcept
{
    return get_left(parent) == child;
}


void allocator_red_black_tree::small_right_rotate(void* joint) noexcept
{
    if(get_left(joint) != nullptr)
    {
        void* left_son = get_left(joint);

        update_parent_ptr(joint, left_son);

        get_parent(left_son) = get_parent(joint);

        void* right_son_of_left_son_of_joint = get_right(left_son);

        get_right(left_son) = joint;

        get_parent(joint) = left_son;

        get_left(joint) = right_son_of_left_son_of_joint;

        get_parent(right_son_of_left_son_of_joint) = joint;

    }
}

void allocator_red_black_tree::small_left_rotate(void* joint) noexcept
{
    if(get_right(joint)!= nullptr)
    {
        void* right_son = get_right(joint);

        update_parent_ptr(joint, right_son);

        get_parent(right_son) = get_parent(joint);

        void* left_son_of_right_son_of_joint = get_left(right_son);

        get_left(right_son) = joint;

        get_parent(joint) = right_son;

        get_right(joint) = left_son_of_right_son_of_joint;

        get_parent(left_son_of_right_son_of_joint) = joint;
    }
}

void allocator_red_black_tree::big_right_rotate(void* joint) noexcept
{
    if(get_left(joint) != nullptr && get_right(get_left(joint)) != nullptr)
    {
        void* node = get_left(joint);

        small_left_rotate(node);
        small_right_rotate(node);
    }
}

void allocator_red_black_tree::big_left_rotate(void *joint) noexcept
{
    if(get_right(joint) != nullptr && get_left(get_right(joint)) != nullptr)
    {
        void* node = get_right(joint);

        small_right_rotate(node);
        small_left_rotate(node);
    }
}

void allocator_red_black_tree::print_tree(void *block, size_t depth)
{
    if (block != nullptr)
    {
        print_tree(get_right(block), depth + 1);
        for (size_t i = 0; i < depth; ++i)
        {
            std::cout << '\t';
        }

        std::string color = get_occup_color(block)._color == color::RED ? "RED" : "BLACK";

        std::cout << get_size_block(block, _trusted_memory) << " " << color << " " << get_occup_color(block).occupied << std::endl;

        print_tree(get_left(block), depth + 1);
    }
}


void allocator_red_black_tree::insert_rb_tree(void* current_block) noexcept
{
    void* top = *get_first_block(_trusted_memory);
    void* parent = nullptr;

    while(top != nullptr)
    {
        if(get_size_block(current_block, _trusted_memory) < get_size_block(top, _trusted_memory))
        {
            parent = top;
            top = get_left(top);
        }
        else
        {
            parent = top;
            top = get_right(top);
        }
    }

    get_parent(current_block) = parent;
    get_left(current_block) = nullptr;
    get_right(current_block) = nullptr;
    get_forward(current_block) = nullptr;
    get_back(current_block) = nullptr;
    get_occup_color(current_block).occupied = false;
    get_occup_color(current_block)._color = color::RED;

    if(parent == nullptr)
    {
        *get_first_block(_trusted_memory) = current_block;
    }
    else
    {
        if(get_size_block(current_block, _trusted_memory) >= get_size_block(parent, _trusted_memory))
        {
            get_right(parent) = current_block;
        }
        else
        {
            get_left(parent) = current_block;
        }
    }

    bool need_to_continue = true;
    bool up = is_left_subtree(current_block, parent);

    while(need_to_continue)
    {
        if(get_occup_color(parent)._color == color::BLACK)
        {
            break;
        }
        if(get_occup_color(parent)._color == color::RED)
        {
            void* grand = get_parent(parent);

            if(get_right(grand) == parent)
            {
                if((get_left(grand) == nullptr || get_occup_color(get_left(grand))._color == color::BLACK) && !up)
                {
                    get_occup_color(grand)._color = color::RED;
                    small_left_rotate(grand);
                    get_occup_color(get_parent(grand))._color = color::BLACK;
                    break;
                }
                else if((get_left(grand) == nullptr || get_occup_color(get_left(grand))._color == color::BLACK) && up)
                {
                    get_occup_color(grand)._color = color::RED;
                    big_left_rotate(grand);
                    get_occup_color(get_parent(grand))._color = color::BLACK;
                    break;
                }
                else if(get_occup_color(get_left(grand))._color == color::RED)
                {
                    get_occup_color(grand)._color = color::RED;
                    get_occup_color(get_left(grand))._color = color::BLACK;
                    get_occup_color(get_right(grand))._color = color::BLACK;
                    current_block = grand;
                }
            }
            else if(get_left(grand) == parent)
            {
                if((get_right(grand) == nullptr || get_occup_color(get_right(grand))._color == color::BLACK) && up)
                {
                    get_occup_color(grand)._color = color::RED;
                    small_right_rotate(grand);
                    get_occup_color(get_parent(grand))._color = color::BLACK;
                    break;
                }
                else if((get_right(grand) == nullptr || get_occup_color(get_right(grand))._color == color::BLACK) && !up)
                {
                    get_occup_color(grand)._color = color::RED;
                    big_right_rotate(grand);
                    get_occup_color(get_parent(grand))._color = color::BLACK;
                    break;
                }
                else if(get_occup_color(get_right(grand))._color == color::RED)
                {
                    get_occup_color(grand)._color = color::RED;
                    get_occup_color(get_left(grand))._color = color::BLACK;
                    get_occup_color(get_right(grand))._color = color::BLACK;
                    current_block = grand;
                }
            }
        }

        if(get_parent(current_block) == nullptr)
        {
            get_occup_color(current_block)._color = color::BLACK;
            break;
        }

        parent = get_parent(current_block);
        up = is_left_subtree(current_block, parent);
    }
}


void allocator_red_black_tree::remove_rb_tree(void *current_block) noexcept
{
    void* parent;
    bool need_check_upper = false;

    if(get_right(current_block) == nullptr && get_left(current_block) == nullptr)
    {
        parent = get_parent(current_block);
        update_parent_ptr(current_block, nullptr);
        need_check_upper = get_occup_color(current_block)._color == color::BLACK;
    }
    else if(get_right(current_block) == nullptr || get_left(current_block) == nullptr)
    {
        void* button_node = get_right(current_block) != nullptr  ? get_right(current_block) : get_left(current_block);

        get_occup_color(button_node)._color = color::BLACK;

        update_parent_ptr(current_block, button_node);
        get_parent(button_node) = get_parent(current_block);

    }
    else
    {
        void* button_node = get_left(current_block);

        while(get_right(button_node) != nullptr)
        {
            button_node = get_right(button_node);
        }

        need_check_upper = get_left(button_node) == nullptr && get_occup_color(button_node)._color == color::BLACK;

        parent = get_parent(button_node);

        if(get_occup_color(button_node)._color == color::BLACK && get_left(button_node) != nullptr)
        {
            get_occup_color(get_left(button_node))._color = color::BLACK;
        }

        update_parent_ptr(current_block, button_node);
        get_right(button_node) = get_right(current_block);
        get_parent(get_right(button_node)) = button_node;

        if(get_parent(button_node) != current_block)
        {
            update_parent_ptr(button_node, get_left(button_node));
            if(get_left(button_node) != nullptr)
            {
                get_parent(get_left(button_node)) = get_parent(button_node);
            }
            get_left(button_node) = get_left(current_block);
            get_parent(get_left(button_node)) = button_node;
        }
        else
        {
            parent = button_node;
        }

        get_occup_color(button_node)._color = get_occup_color(current_block)._color;
        get_parent(button_node) = get_parent(current_block);

    }

    if(need_check_upper)
    {
        rebalance(parent);
    }
}

void allocator_red_black_tree::rebalance(void* parent, void* deleted)
{

}



