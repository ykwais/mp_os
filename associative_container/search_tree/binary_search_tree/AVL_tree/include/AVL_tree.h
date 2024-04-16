#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H

#include <binary_search_tree.h>
#include <stack>

template<
    typename tkey,
    typename tvalue>
class AVL_tree final:
    public binary_search_tree<tkey, tvalue>
{

private:
    
    struct node final:
        binary_search_tree<tkey, tvalue>::node
    {
    public:

        size_t _height;

    public:

        explicit node(tkey const& key , tvalue const& value);

        explicit node(tkey const& key , tvalue&& value);

        ~node() noexcept override = default;

    public:

        void update_height() noexcept
        {
            size_t left_height = binary_search_tree<tkey, tvalue>::node::left_subtree ? static_cast<node*>(binary_search_tree<tkey, tvalue>::node::left_subtree)->_height : 0;
            size_t right_height = binary_search_tree<tkey, tvalue>::node::right_subtree ? static_cast<node*>(binary_search_tree<tkey, tvalue>::node::right_subtree)->_height : 0;
            _height = std::max(left_height, right_height) + 1;
        }

        int get_balance() noexcept
        {
            size_t left_height = binary_search_tree<tkey, tvalue>::node::left_subtree ? static_cast<node*>(binary_search_tree<tkey, tvalue>::node::left_subtree)->_height : 0;
            size_t right_height = binary_search_tree<tkey, tvalue>::node::right_subtree ? static_cast<node*>(binary_search_tree<tkey, tvalue>::node::right_subtree)->_height : 0;

            return right_height - left_height;
        }

        
    };

public:
    
    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {
    
    public:
        
        size_t subtree_height;
    
    public:
        
        explicit iterator_data(
            unsigned int depth = 0,
            tkey const &key = tkey(),
            tvalue const &value = tvalue(),
            size_t subtree_height = 0);

    public:

        void update_iterator_data(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override
        {
            binary_search_tree<tkey, tvalue>::iterator_data::update_iterator_data(path);
            subtree_height = static_cast<node*>(*(path.top()))->_height;
        }

        iterator_data* clone_iterator_data() const override
        {
            return new iterator_data(*this);
        }

        
    };

private:

    // region unused

    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {
    
    public:
        
        explicit insertion_template_method(
            AVL_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
    
    private:
        
        // TODO: think about it!
        
    };
    
    class obtaining_template_method final:
        public binary_search_tree<tkey, tvalue>::obtaining_template_method
    {
    
    public:
        
        explicit obtaining_template_method(
            AVL_tree<tkey, tvalue> *tree);
        
        // TODO: think about it!
        
    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {
    
    public:
        
        explicit disposal_template_method(
            AVL_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
        // TODO: think about it!
        
    };

    // endregion unused

public:
    
    explicit AVL_tree(
        std::function<int(tkey const &, tkey const &)> keys_comparer,
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    ~AVL_tree() noexcept final = default;
    
    AVL_tree(
        AVL_tree<tkey, tvalue> const &other);
    
    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> const &other);
    
    AVL_tree(
        AVL_tree<tkey, tvalue> &&other) noexcept = default;
    
    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> &&other) noexcept = default;

public:

    void insert_inside(const tkey& key, const tvalue& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk) override;

    void insert_inside(const tkey& key, tvalue&& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk) override;

    template<typename universal_tvalue>
    void insert_inside_universal(const tkey& key, universal_tvalue&& val, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk);

public:

    // region of redefinition iterators

    class prefix_iterator : public binary_search_tree<tkey, tvalue>::prefix_iterator
    {
    public:
        explicit prefix_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
        : binary_search_tree<tkey, tvalue>::prefix_iterator(path, it_data) {}
    };

    class prefix_const_iterator : public binary_search_tree<tkey, tvalue>::prefix_const_iterator
    {
    public:
        explicit prefix_const_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
                : binary_search_tree<tkey, tvalue>::prefix_const_iterator(path, it_data) {}
    };

    class prefix_reverse_iterator : public binary_search_tree<tkey, tvalue>::prefix_reverse_iterator
    {
    public:
        explicit prefix_reverse_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
                : binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(path, it_data) {}
    };

    class prefix_const_reverse_iterator : public binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator
    {
    public:
        explicit prefix_const_reverse_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
                : binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(path, it_data) {}
    };

    class infix_iterator : public binary_search_tree<tkey, tvalue>::infix_iterator
    {
    public:
        explicit infix_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
                : binary_search_tree<tkey, tvalue>::infix_iterator(path, it_data) {}
    };

    class infix_const_iterator : public binary_search_tree<tkey, tvalue>::infix_const_iterator
    {
    public:
        explicit infix_const_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
                : binary_search_tree<tkey, tvalue>::infix_const_iterator(path, it_data) {}
    };

    class infix_reverse_iterator : public binary_search_tree<tkey, tvalue>::infix_reverse_iterator
    {
    public:
        explicit infix_reverse_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
                : binary_search_tree<tkey, tvalue>::infix_reverse_iterator(path, it_data) {}
    };

    class infix_const_reverse_iterator : public binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator
    {
    public:
        explicit infix_const_reverse_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
                : binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(path, it_data) {}
    };

    class postfix_iterator : public binary_search_tree<tkey, tvalue>::postfix_iterator
    {
    public:
        explicit postfix_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
                : binary_search_tree<tkey, tvalue>::postfix_iterator(path, it_data) {}
    };

    class postfix_const_iterator : public binary_search_tree<tkey, tvalue>::postfix_const_iterator
    {
    public:
        explicit postfix_const_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
                : binary_search_tree<tkey, tvalue>::postfix_const_iterator(path, it_data) {}
    };

    class postfix_reverse_iterator : public binary_search_tree<tkey, tvalue>::postfix_reverse_iterator
    {
    public:
        explicit postfix_reverse_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
                : binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(path, it_data) {}
    };

    class postfix_const_reverse_iterator : public binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator
    {
    public:
        explicit postfix_const_reverse_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
                : binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(path, it_data) {}
    };

// endregion of redefinition iterators

// region iterators requests definition

    prefix_iterator begin_prefix()  noexcept;

    prefix_iterator end_prefix()  noexcept;

    prefix_const_iterator cbegin_prefix() const noexcept;

    prefix_const_iterator cend_prefix() const noexcept;

    prefix_reverse_iterator rbegin_prefix()  noexcept;

    prefix_reverse_iterator rend_prefix()  noexcept;

    prefix_const_reverse_iterator crbegin_prefix() const noexcept;

    prefix_const_reverse_iterator crend_prefix() const noexcept;

    infix_iterator begin_infix()  noexcept;

    infix_iterator end_infix()  noexcept;

    infix_const_iterator cbegin_infix() const noexcept;

    infix_const_iterator cend_infix() const noexcept;

    infix_reverse_iterator rbegin_infix()  noexcept;

    infix_reverse_iterator rend_infix()  noexcept;

    infix_const_reverse_iterator crbegin_infix() const noexcept;

    infix_const_reverse_iterator crend_infix() const noexcept;

    postfix_iterator begin_postfix()  noexcept;

    postfix_iterator end_postfix()  noexcept;

    postfix_const_iterator cbegin_postfix() const noexcept;

    postfix_const_iterator cend_postfix() const noexcept;

    postfix_reverse_iterator rbegin_postfix()  noexcept;

    postfix_reverse_iterator rend_postfix()  noexcept;

    postfix_const_reverse_iterator crbegin_postfix() const noexcept;

    postfix_const_reverse_iterator crend_postfix() const noexcept;

    // endregion iterators requests definition
    
};

// region constructors

template<typename tkey, typename tvalue>
AVL_tree<tkey, tvalue>::node::node(const tkey &key, const tvalue &value) : binary_search_tree<tkey, tvalue>::node(key, value) , _height(1) {}

template<typename tkey, typename tvalue>
AVL_tree<tkey, tvalue>::node::node(const tkey &key, tvalue&& value) : binary_search_tree<tkey, tvalue>::node(key, std::move(value)), _height(1) {}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    size_t subtree_height):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value), subtree_height(subtree_height) {}

// endregion constructors


// region unused

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    AVL_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::insertion_template_method(tree, insertion_strategy)
{
    throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(AVL_tree<tkey, tvalue> *, typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    AVL_tree<tkey, tvalue> *tree)
{
    throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(AVL_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    AVL_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy)
{
    throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(AVL_tree<tkey, tvalue> *, typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy)", "your code should be here...");
}

// endregion unused

// region AVL rules + iterators definition

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) : binary_search_tree<tkey, tvalue>(keys_comparer, allocator, logger, insertion_strategy, disposal_strategy){}



template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    AVL_tree<tkey, tvalue> const &other) : binary_search_tree<tkey, tvalue>(other) {}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
    AVL_tree<tkey, tvalue> const &other)
{
    if(this != &other)
    {
        binary_search_tree<tkey, tvalue>::operator=(other);
    }
    return *this;
}


template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::prefix_iterator AVL_tree<tkey, tvalue>::begin_prefix()  noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    if(binary_search_tree<tkey, tvalue>::_root != nullptr)
    {
        stk.push(&(binary_search_tree<tkey, tvalue>::_root));
    }

    return prefix_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::prefix_iterator AVL_tree<tkey, tvalue>::end_prefix()  noexcept
{
    return AVL_tree<tkey, tvalue>::prefix_iterator();
}

template<
        typename tkey,
        typename tvalue>
typename AVL_tree<tkey, tvalue>::prefix_const_iterator AVL_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    if(binary_search_tree<tkey, tvalue>::_root != nullptr)
    {
        stk.push((&(binary_search_tree<tkey, tvalue>::_root)));
    }

    return prefix_const_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::prefix_const_iterator AVL_tree<tkey, tvalue>::cend_prefix() const noexcept
{
    return AVL_tree<tkey, tvalue>::prefix_const_iterator();
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::prefix_reverse_iterator AVL_tree<tkey, tvalue>::rbegin_prefix()  noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    if(binary_search_tree<tkey, tvalue>::_root != nullptr)
    {
        stk.push(&(binary_search_tree<tkey, tvalue>::_root));
    }

    return prefix_reverse_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::prefix_reverse_iterator AVL_tree<tkey, tvalue>::rend_prefix()  noexcept
{
    return AVL_tree<tkey, tvalue>::prefix_reverse_iterator();
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::prefix_const_reverse_iterator AVL_tree<tkey, tvalue>::crbegin_prefix() const noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stack;
    if(binary_search_tree<tkey, tvalue>::_root != nullptr)
    {
        stack.push(&(binary_search_tree<tkey, tvalue>::_root));
    }

    return prefix_const_reverse_iterator(stack, new iterator_data);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::prefix_const_reverse_iterator AVL_tree<tkey, tvalue>::crend_prefix() const noexcept
{
    return AVL_tree<tkey, tvalue>::prefix_const_reverse_iterator();
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::infix_iterator AVL_tree<tkey, tvalue>::begin_infix() noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    typename binary_search_tree<tkey, tvalue>::node** current_node = &(binary_search_tree<tkey, tvalue>::_root);
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = &((*current_node)->left_subtree);
    }

    return infix_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::infix_iterator AVL_tree<tkey, tvalue>::end_infix()  noexcept
{
    return AVL_tree<tkey, tvalue>::infix_iterator();
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::infix_const_iterator AVL_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    typename binary_search_tree<tkey, tvalue>::node** current_node = &(binary_search_tree<tkey, tvalue>::_root);
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = (&((*current_node)->left_subtree));
    }

    return infix_const_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::infix_const_iterator AVL_tree<tkey, tvalue>::cend_infix() const noexcept
{
    return AVL_tree<tkey, tvalue>::infix_const_iterator();
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::infix_reverse_iterator AVL_tree<tkey, tvalue>::rbegin_infix()  noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    typename binary_search_tree<tkey, tvalue>::node** current_node = &(binary_search_tree<tkey, tvalue>::_root);
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = &((*current_node)->right_subtree);
    }

    return infix_reverse_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::infix_reverse_iterator AVL_tree<tkey, tvalue>::rend_infix()  noexcept
{
    return AVL_tree<tkey, tvalue>::infix_reverse_iterator();
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::infix_const_reverse_iterator AVL_tree<tkey, tvalue>::crbegin_infix() const noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    typename binary_search_tree<tkey, tvalue>::node** current_node = &(binary_search_tree<tkey, tvalue>::_root);
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = &((*current_node)->right_subtree);
    }

    return infix_const_reverse_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::infix_const_reverse_iterator AVL_tree<tkey, tvalue>::crend_infix() const noexcept
{
    return AVL_tree<tkey, tvalue>::infix_const_reverse_iterator();
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::postfix_iterator AVL_tree<tkey, tvalue>::begin_postfix()  noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    typename binary_search_tree<tkey, tvalue>::node** current_node = &(binary_search_tree<tkey, tvalue>::_root);
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = (*current_node)->left_subtree != nullptr ? &((*current_node)->left_subtree) : &((*current_node)->right_subtree);
    }
    return postfix_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::postfix_iterator AVL_tree<tkey, tvalue>::end_postfix()  noexcept
{
    return AVL_tree<tkey, tvalue>::postfix_iterator();
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::postfix_const_iterator AVL_tree<tkey, tvalue>::cbegin_postfix() const noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    typename binary_search_tree<tkey, tvalue>::node** current_node = (&(binary_search_tree<tkey, tvalue>::_root));
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = (*current_node)->left_subtree != nullptr ? (&((*current_node)->left_subtree)) : (&((*current_node)->right_subtree));
    }
    return postfix_const_iterator(stk, new iterator_data);
}

template<typename tkey,typename tvalue>
typename AVL_tree<tkey, tvalue>::postfix_const_iterator AVL_tree<tkey, tvalue>::cend_postfix() const noexcept
{
    return AVL_tree<tkey, tvalue>::postfix_const_iterator();
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::postfix_reverse_iterator AVL_tree<tkey, tvalue>::rbegin_postfix()  noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    typename binary_search_tree<tkey, tvalue>::node** current_node = &(binary_search_tree<tkey, tvalue>::_root);
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = (*current_node)->right_subtree != nullptr ? &((*current_node)->right_subtree) : &((*current_node)->left_subtree);
    }
    return postfix_reverse_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::postfix_reverse_iterator AVL_tree<tkey, tvalue>::rend_postfix()  noexcept
{
    return AVL_tree<tkey, tvalue>::postfix_reverse_iterator();
}

template<typename tkey,typename tvalue>
typename AVL_tree<tkey, tvalue>::postfix_const_reverse_iterator AVL_tree<tkey, tvalue>::crbegin_postfix() const noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    typename binary_search_tree<tkey, tvalue>::node** current_node = &(binary_search_tree<tkey, tvalue>::_root);
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = (*current_node)->right_subtree != nullptr ? &((*current_node)->right_subtree) : &((*current_node)->left_subtree);
    }
    return postfix_const_reverse_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::postfix_const_reverse_iterator AVL_tree<tkey, tvalue>::crend_postfix() const noexcept
{
    return AVL_tree<tkey, tvalue>::postfix_const_reverse_iterator(std::stack<node**>(), nullptr);
}

// endregion AVL rules + iterators definition

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::insert_inside(const tkey &key, const tvalue &value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
{
    insert_inside_universal(key, value, stk);
}

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::insert_inside(const tkey &key, tvalue&& value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
{
    insert_inside_universal(key, std::move(value), stk);
}

template<typename tkey, typename tvalue>
template<typename universal_tvalue>
void AVL_tree<tkey, tvalue>::insert_inside_universal(const tkey &key, universal_tvalue &&val,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
{
    (*stk.top()) = static_cast<binary_search_tree<tkey, tvalue>::node*>(static_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node), 1)));
    try {
        allocator::construct(*stk.top(), key, std::forward<universal_tvalue>(val));
    }
    catch(...)
    {
        allocator_guardant::deallocate_with_guard(*stk.top());
        throw;
    }

    static_cast<node*>(*stk.top())->update_height();

    node* baby = static_cast<node*>(*stk.top());
    node* parent = nullptr;

    stk.pop();

    if(!stk.empty())
    {
        parent = static_cast<node*>(*stk.top());
    }

    while(!stk.empty())
    {
        static_cast<node*>(*stk.top())->update_height();

        int balance = static_cast<node*>(*stk.top())->get_balance();

        if(balance > 1 || balance < -1)
        {

            if(balance > 1 && static_cast<node*>(*stk.top())->right_subtree == parent && static_cast<node*>(*stk.top())->right_subtree->right_subtree == baby)
            {
                binary_search_tree<tkey, tvalue>::small_left_rotation(*stk.top());
                static_cast<node*>((*stk.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->update_height();
                static_cast<node*>((*stk.top()))->update_height();
            }
            else if(balance > 1)
            {
                binary_search_tree<tkey, tvalue>::big_left_rotation(*stk.top());
                static_cast<node*>((*stk.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->update_height();
                static_cast<node*>((*stk.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->update_height();
                static_cast<node*>((*stk.top()))->update_height();
            }
            else if(balance < 1 && static_cast<node*>(*stk.top())->right_subtree == parent && static_cast<node*>(*stk.top())->right_subtree->right_subtree == baby)
            {
                binary_search_tree<tkey, tvalue>::small_right_rotation(*stk.top());
                static_cast<node*>((*stk.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->update_height();
                static_cast<node*>((*stk.top()))->update_height();
            }
            else
            {
                binary_search_tree<tkey, tvalue>::big_right_rotation(*stk.top());
                static_cast<node*>((*stk.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->update_height();
                static_cast<node*>((*stk.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->update_height();
                static_cast<node*>((*stk.top()))->update_height();
            }

        }

        baby = parent;
        parent = static_cast<node*>(*stk.top());
        stk.pop();

    }





}



#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H