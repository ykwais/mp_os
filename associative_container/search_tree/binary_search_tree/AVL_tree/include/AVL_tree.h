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
        
        size_t _height;
        
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
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    ~AVL_tree() noexcept final;
    
    AVL_tree(
        AVL_tree<tkey, tvalue> const &other);
    
    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> const &other);
    
    AVL_tree(
        AVL_tree<tkey, tvalue> &&other) noexcept;
    
    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> &&other) noexcept;

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
    
};

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    size_t subtree_height):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value), subtree_height(subtree_height) {}


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

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy)
{
    throw not_implemented("template<typename tkey, typename tvalue> AVL_tree<tkey, tvalue>::AVL_tree(allocator *, logger *, typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy, typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy)", "your code should be here...");
}

// endregion unused

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::~AVL_tree() noexcept
{

}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    AVL_tree<tkey, tvalue> const &other)
{

}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
    AVL_tree<tkey, tvalue> const &other)
{

}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    AVL_tree<tkey, tvalue> &&other) noexcept
{

}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
    AVL_tree<tkey, tvalue> &&other) noexcept
{

}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H