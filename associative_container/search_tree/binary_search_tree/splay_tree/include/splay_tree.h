#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class splay_tree final:
    public binary_search_tree<tkey, tvalue>
{

private:
    
    struct splay_node final:
        binary_search_tree<tkey, tvalue>::node
    {
        
    public:

        explicit splay_node(tkey const& key, tvalue const& value);

        explicit splay_node(tkey const& key, tvalue&& value);

        ~splay_node() noexcept override = default;
        
    };

public:
    
    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {
    
        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value);
        
    };

private:
    // region unused
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {
    
    public:
        
        explicit insertion_template_method(
            splay_tree<tkey, tvalue> *tree);
    
    private:
        
        // TODO: think about it!
        
    };
    
    class obtaining_template_method final:
        public binary_search_tree<tkey, tvalue>::obtaining_template_method
    {
    
    public:
        
        explicit obtaining_template_method(
            splay_tree<tkey, tvalue> *tree);
        
        // TODO: think about it!
        
    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {
    
    public:
        
        explicit disposal_template_method(
            splay_tree<tkey, tvalue> *tree);
        
        // TODO: think about it!
        
    };

    // endregion unused

public:
    
    explicit splay_tree(
        allocator *allocator = nullptr,
        logger *logger = nullptr);

public:
    
    ~splay_tree() noexcept final = default;
    
    splay_tree(
        splay_tree<tkey, tvalue> const &other);
    
    splay_tree<tkey, tvalue> &operator=(
        splay_tree<tkey, tvalue> const &other);
    
    splay_tree(
        splay_tree<tkey, tvalue> &&other) noexcept = default;
    
    splay_tree<tkey, tvalue> &operator=(
        splay_tree<tkey, tvalue> &&other) noexcept = default;
    
};

template<typename tkey, typename tvalue>
splay_tree<tkey, tvalue>::splay_node::splay_node(const tkey &key, tvalue &&value) : binary_search_tree<tkey, tvalue>::node(key, std::move(value)) {}

template<typename tkey, typename tvalue>
splay_tree<tkey, tvalue>::splay_node::splay_node(const tkey &key, const tvalue &value) : binary_search_tree<tkey, tvalue>::node(key, value) {}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value) {}

// region unused

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    splay_tree<tkey, tvalue> *tree)
{
    throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(splay_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    splay_tree<tkey, tvalue> *tree)
{
    throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(splay_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    splay_tree<tkey, tvalue> *tree)
{
    throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(splay_tree<tkey, tvalue> *)", "your code should be here...");
}

// endregion unused

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::splay_tree(
    allocator *allocator,
    logger *logger) {}



template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::splay_tree(
    splay_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue>::splay_tree(splay_tree<tkey, tvalue> const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue> &splay_tree<tkey, tvalue>::operator=(
    splay_tree<tkey, tvalue> const &other)
{
    throw not_implemented("template<typename tkey, typename tvalue> splay_tree<tkey, tvalue> &splay_tree<tkey, tvalue>::operator=(splay_tree<tkey, tvalue> const &)", "your code should be here...");
}



#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H