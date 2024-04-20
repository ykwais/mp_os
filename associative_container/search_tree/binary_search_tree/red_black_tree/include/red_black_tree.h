#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class red_black_tree final:
    public binary_search_tree<tkey, tvalue>
{

public:
    
    enum class node_color
    {
        RED,
        BLACK
    };

private:
    
    struct node final:
        binary_search_tree<tkey, tvalue>::node
    {
    public:
        
        node_color _color;


    public:

        explicit node(const tkey& key, const tvalue& value);

        explicit node(const tkey& key, tvalue&& value);

        ~node() noexcept override = default;

    public:

        void update_color(node_color new_color)
        {
            _color = new_color;
        }

        node_color get_color()/////////////////??????TODO
        {
            return _color;
        }

    };

public:
    
    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {
    
    public:
        
        node_color _color;
    
    public:
        
        explicit iterator_data(
            unsigned int depth = 0,
            tkey const &key = tkey(),
            tvalue const &value = tvalue(),
            node_color color = node_color::RED);

    iterator_data* clone_iterator_data() const override
    {
        return new iterator_data(*this);
    }

    void update_iterator_data(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override
    {
        if(!path.empty()) {
            binary_search_tree<tkey, tvalue>::iterator_data::key = (*(path.top()))->key;
            binary_search_tree<tkey, tvalue>::iterator_data::value = (*(path.top()))->value;
            binary_search_tree<tkey, tvalue>::iterator_data::depth = path.size() - 1;
            _color = static_cast<node *>(*(path.top()))->_color;
        }
    }

        
    };

private:

    // region unused
    
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {
    
    public:
        
        explicit insertion_template_method(
            red_black_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
    
    private:
        
        // TODO: think about it!
        
    };
    
    class obtaining_template_method final:
        public binary_search_tree<tkey, tvalue>::obtaining_template_method
    {
    
    public:
        
        explicit obtaining_template_method(
            red_black_tree<tkey, tvalue> *tree);
        
        // TODO: think about it!
        
    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {
    
    public:
        
        explicit disposal_template_method(
            red_black_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
        // TODO: think about it!
        
    };

    // endregion unused

public:
    
    explicit red_black_tree(
        std::function<int(tkey const &, tkey const &)> keys_comparer,
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    ~red_black_tree() noexcept final = default;
    
    red_black_tree(
        red_black_tree<tkey, tvalue> const &other);
    
    red_black_tree<tkey, tvalue> &operator=(
        red_black_tree<tkey, tvalue> const &other);
    
    red_black_tree(
        red_black_tree<tkey, tvalue> &&other) noexcept = default;
    
    red_black_tree<tkey, tvalue> &operator=(
        red_black_tree<tkey, tvalue> &&other) noexcept = default;

private:

    void insert_inside(const tkey& key, const tvalue& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk) override;

    void insert_inside(const tkey& key, tvalue&& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk) override;

    template<typename universal_tvalue>
    void insert_inside_universal(const tkey& key, universal_tvalue&& val, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk);

    tvalue dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk) override;

    void print_rb(binary_search_tree<tkey, tvalue>::node* root, int level) const noexcept;

public:

    void print_tree() const noexcept
    {
        print_rb(binary_search_tree<tkey, tvalue>::_root, 0);
    }


public:

    class prefix_iterator : public binary_search_tree<tkey, tvalue>::prefix_iterator {
    public:
        explicit prefix_iterator( const std::stack<typename binary_search_tree<tkey, tvalue>::node **> path = std::stack<typename binary_search_tree<tkey, tvalue>::node **>(),
                iterator_data *it_data = nullptr) : binary_search_tree<tkey, tvalue>::prefix_iterator(path, it_data){}
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




    
};

template<typename tkey, typename tvalue>
red_black_tree<tkey, tvalue>::node::node(const tkey &key, tvalue&& value) : binary_search_tree<tkey, tvalue>::node(key, value), _color(node_color::RED)  {}

template<typename tkey, typename tvalue>
red_black_tree<tkey, tvalue>::node::node(const tkey &key, const tvalue &value) : binary_search_tree<tkey, tvalue>::node(key, value), _color(node_color::RED) {}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    typename red_black_tree<tkey, tvalue>::node_color color):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value), _color(color) {}

// region unused

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    red_black_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::insertion_template_method(tree, insertion_strategy)
{
    throw not_implemented("template<typename tkey, typename tvalue> red_black_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(red_black_tree<tkey, tvalue> *, typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    red_black_tree<tkey, tvalue> *tree)
{
    throw not_implemented("template<typename tkey, typename tvalue> red_black_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(red_black_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    red_black_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>::disposal_template_method(tree, disposal_strategy)
{
    throw not_implemented("template<typename tkey, typename tvalue> red_black_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(red_black_tree<tkey, tvalue> *, typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy)", "your code should be here...");
}

// endregion unused

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) : binary_search_tree<tkey, tvalue>(keys_comparer, allocator, logger, insertion_strategy, disposal_strategy) {}


template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    red_black_tree<tkey, tvalue> const &other) : binary_search_tree<tkey, tvalue>(other){}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(
    red_black_tree<tkey, tvalue> const &other)
{
    if(this != &other)
    {
        binary_search_tree<tkey, tvalue>::operator=(other);
    }
    return *this;
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::prefix_iterator red_black_tree<tkey, tvalue>::begin_prefix()  noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    if(binary_search_tree<tkey, tvalue>::_root != nullptr)
    {
        stk.push(&(binary_search_tree<tkey, tvalue>::_root));
    }

    return prefix_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::prefix_iterator red_black_tree<tkey, tvalue>::end_prefix()  noexcept
{
    return red_black_tree<tkey, tvalue>::prefix_iterator();
}

template<
        typename tkey,
        typename tvalue>
typename red_black_tree<tkey, tvalue>::prefix_const_iterator red_black_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    if(binary_search_tree<tkey, tvalue>::_root != nullptr)
    {
        stk.push((&(binary_search_tree<tkey, tvalue>::_root)));
    }

    return prefix_const_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::prefix_const_iterator red_black_tree<tkey, tvalue>::cend_prefix() const noexcept
{
    return red_black_tree<tkey, tvalue>::prefix_const_iterator();
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::prefix_reverse_iterator red_black_tree<tkey, tvalue>::rbegin_prefix()  noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
    if(binary_search_tree<tkey, tvalue>::_root != nullptr)
    {
        stk.push(&(binary_search_tree<tkey, tvalue>::_root));
    }

    return prefix_reverse_iterator(stk, new iterator_data);
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::prefix_reverse_iterator red_black_tree<tkey, tvalue>::rend_prefix()  noexcept
{
    return red_black_tree<tkey, tvalue>::prefix_reverse_iterator();
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::prefix_const_reverse_iterator red_black_tree<tkey, tvalue>::crbegin_prefix() const noexcept
{
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stack;
    if(binary_search_tree<tkey, tvalue>::_root != nullptr)
    {
        stack.push(&(binary_search_tree<tkey, tvalue>::_root));
    }

    return prefix_const_reverse_iterator(stack, new iterator_data);
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::prefix_const_reverse_iterator red_black_tree<tkey, tvalue>::crend_prefix() const noexcept
{
    return red_black_tree<tkey, tvalue>::prefix_const_reverse_iterator();
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::infix_iterator red_black_tree<tkey, tvalue>::begin_infix() noexcept
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
typename red_black_tree<tkey, tvalue>::infix_iterator red_black_tree<tkey, tvalue>::end_infix()  noexcept
{
    return red_black_tree<tkey, tvalue>::infix_iterator();
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::infix_const_iterator red_black_tree<tkey, tvalue>::cbegin_infix() const noexcept
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
typename red_black_tree<tkey, tvalue>::infix_const_iterator red_black_tree<tkey, tvalue>::cend_infix() const noexcept
{
    return red_black_tree<tkey, tvalue>::infix_const_iterator();
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::infix_reverse_iterator red_black_tree<tkey, tvalue>::rbegin_infix()  noexcept
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
typename red_black_tree<tkey, tvalue>::infix_reverse_iterator red_black_tree<tkey, tvalue>::rend_infix()  noexcept
{
    return red_black_tree<tkey, tvalue>::infix_reverse_iterator();
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::infix_const_reverse_iterator red_black_tree<tkey, tvalue>::crbegin_infix() const noexcept
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
typename red_black_tree<tkey, tvalue>::infix_const_reverse_iterator red_black_tree<tkey, tvalue>::crend_infix() const noexcept
{
    return red_black_tree<tkey, tvalue>::infix_const_reverse_iterator();
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::postfix_iterator red_black_tree<tkey, tvalue>::begin_postfix()  noexcept
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
typename red_black_tree<tkey, tvalue>::postfix_iterator red_black_tree<tkey, tvalue>::end_postfix()  noexcept
{
    return red_black_tree<tkey, tvalue>::postfix_iterator();
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::postfix_const_iterator red_black_tree<tkey, tvalue>::cbegin_postfix() const noexcept
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
typename red_black_tree<tkey, tvalue>::postfix_const_iterator red_black_tree<tkey, tvalue>::cend_postfix() const noexcept
{
    return red_black_tree<tkey, tvalue>::postfix_const_iterator();
}

template<typename tkey, typename tvalue>
typename red_black_tree<tkey, tvalue>::postfix_reverse_iterator red_black_tree<tkey, tvalue>::rbegin_postfix()  noexcept
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
typename red_black_tree<tkey, tvalue>::postfix_reverse_iterator red_black_tree<tkey, tvalue>::rend_postfix()  noexcept
{
    return red_black_tree<tkey, tvalue>::postfix_reverse_iterator();
}

template<typename tkey,typename tvalue>
typename red_black_tree<tkey, tvalue>::postfix_const_reverse_iterator red_black_tree<tkey, tvalue>::crbegin_postfix() const noexcept
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
typename red_black_tree<tkey, tvalue>::postfix_const_reverse_iterator red_black_tree<tkey, tvalue>::crend_postfix() const noexcept
{
    return red_black_tree<tkey, tvalue>::postfix_const_reverse_iterator(std::stack<node**>(), nullptr);
}


template<typename tkey, typename tvalue>
void red_black_tree<tkey, tvalue>::insert_inside(const tkey &key, const tvalue &value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
{
    insert_inside_universal(key, value, stk);
}

template<typename tkey, typename tvalue>
void red_black_tree<tkey, tvalue>::insert_inside(const tkey &key, tvalue&& value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
{
    insert_inside_universal(key, std::move(value), stk);
}

template<typename tkey, typename tvalue>
template<typename universal_tvalue>
void red_black_tree<tkey, tvalue>::insert_inside_universal(const tkey &key, universal_tvalue &&val,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
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

    static_cast<node*>(*stk.top())->update_color(node_color::RED);

    node* current_node = static_cast<node*>(*stk.top());

    stk.pop();

    if(stk.empty())
    {
        current_node->update_color(node_color::BLACK);
        print_rb(binary_search_tree<tkey, tvalue>::_root, 0);
        std::cout << "\n-------------------------------------------------\n" << std::endl;
        return;
    }

    bool down;
    bool up;

    if(!stk.empty())
    {
        down = static_cast<node*>(*(stk.top()))->left_subtree == current_node;
        up = static_cast<node*>(*(stk.top()))->left_subtree == current_node;
    }

    while(!stk.empty())
    {


        if(static_cast<node*>(*stk.top())->_color == node_color::BLACK)
        {
            break;
        }

        if(static_cast<node*>(*stk.top())->_color == node_color::RED)
        {
            node* parent = static_cast<node*>(*stk.top());
            stk.pop();

            node* grand = static_cast<node*>(*stk.top());

            if(grand->right_subtree == parent)
            {
                if((grand->left_subtree == nullptr || static_cast<node*>(grand->left_subtree)->_color == node_color::BLACK) && !up)//сюда же дописать черного дядю
                {
                    grand->update_color(node_color::RED);
                    binary_search_tree<tkey, tvalue>::small_left_rotation(*stk.top());
                    static_cast<node*>(*stk.top())->update_color(node_color::BLACK);
                    break;
                }
                else if((grand->left_subtree == nullptr || static_cast<node*>(grand->left_subtree)->_color == node_color::BLACK) && up)
                {
                    grand->update_color(node_color::RED);
                    binary_search_tree<tkey, tvalue>::big_left_rotation(*stk.top());
                    static_cast<node*>(*stk.top())->update_color(node_color::BLACK);
                    break;

                }
                else if(static_cast<node*>(grand->left_subtree)->_color == node_color::RED)
                {
                    grand->update_color(node_color::RED);
                    static_cast<node*>(grand->left_subtree)->update_color(node_color::BLACK);
                    static_cast<node*>(grand->right_subtree)->update_color(node_color::BLACK);
                }

            }
            else if(grand->left_subtree == parent)
            {
                if((grand->right_subtree == nullptr || static_cast<node*>(grand->right_subtree)->_color == node_color::BLACK) && up)//сюда же дописать черного дядю
                {
                    grand->update_color(node_color::RED);
                    binary_search_tree<tkey, tvalue>::small_right_rotation(*stk.top());
                    static_cast<node*>(*stk.top())->update_color(node_color::BLACK);
                    break;
                }
                else if((grand->right_subtree == nullptr || static_cast<node*>(grand->right_subtree)->_color == node_color::BLACK) && !up)
                {
                    grand->update_color(node_color::RED);
                    binary_search_tree<tkey, tvalue>::big_right_rotation(*stk.top());
                    static_cast<node*>(*stk.top())->update_color(node_color::BLACK);
                    break;
                }
                else if(static_cast<node*>(grand->right_subtree)->_color == node_color::RED)
                {
                    grand->update_color(node_color::RED);
                    static_cast<node*>(grand->left_subtree)->update_color(node_color::BLACK);
                    static_cast<node*>(grand->right_subtree)->update_color(node_color::BLACK);
                }
            }


        }

        current_node = static_cast<node*>(*stk.top());

        if(stk.size() == 1 && static_cast<node*>(*stk.top())->_color != node_color::BLACK)
        {
            static_cast<node*>(*stk.top())->_color = node_color::BLACK;
            break;
        }

        stk.pop();


        if(!stk.empty()) {
            up = static_cast<node *>(*(stk.top()))->left_subtree == current_node;
        }

    }


    print_rb(binary_search_tree<tkey, tvalue>::_root, 0);
    std::cout << "\n-------------------------------------------------\n" << std::endl;
}

template<typename tkey, typename tvalue>
tvalue red_black_tree<tkey, tvalue>::dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**> &stk)
{

    tvalue res = (*stk.top())->value;

    typename binary_search_tree<tkey, tvalue>::node* current_node = *stk.top();

    if(static_cast<node*>(current_node)->_color == node_color::RED && current_node->left_subtree == nullptr && current_node->right_subtree == nullptr)
    {
        *stk.top() = nullptr;
        stk.pop();
    }


    allocator::destruct(current_node);
    allocator_guardant::deallocate_with_guard(current_node);




    return res;

}


template<typename tkey, typename tvalue>
void red_black_tree<tkey, tvalue>::print_rb(typename binary_search_tree<tkey, tvalue>::node* root, int level) const noexcept
{
    if(root == nullptr)
    {
        return;
    }

    print_rb(root->right_subtree, level + 1);
    std::string color = static_cast<node*>(root)->_color == node_color::BLACK ? "BLACK" : "RED";
    for(int i = 0, end = level; i != end; ++i)
    {
        std::cout << "    ";
    }
    std::cout << root->key << " " << root->value << " " << color << std::endl;
    print_rb(root->left_subtree, level + 1);
}





#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
