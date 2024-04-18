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

    explicit splay_tree(std::function<int(tkey const &, tkey const &)> keys_comparer,
                        allocator *allocator = nullptr,
                        logger *logger = nullptr,
                        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
                        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    ~splay_tree() noexcept final = default;
    
    splay_tree(
        splay_tree<tkey, tvalue> const &other) = default;
    
    splay_tree<tkey, tvalue> &operator=(
        splay_tree<tkey, tvalue> const &other) = default;
    
    splay_tree(
        splay_tree<tkey, tvalue> &&other) noexcept = default;
    
    splay_tree<tkey, tvalue> &operator=(
        splay_tree<tkey, tvalue> &&other) noexcept = default;


private:

//    using node = binary_search_tree<tkey, tvalue>::node;

    void insert_inside(const tkey& key, const tvalue& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk) override;

    void insert_inside(const tkey& key, tvalue&& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk) override;

    template<typename universal_tvalue>
    void insert_inside_universal(const tkey& key, universal_tvalue&& val, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk);

    tvalue dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk) override;

    void splay(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk);


};

template<typename tkey, typename tvalue>
void splay_tree<tkey, tvalue>::splay(std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
{
    bool down, up;


//    stk.pop();

//    if(!stk.empty())
//    {
//        down = up = (*(stk.top()))->left_subtree == current_node;
//    }

    while(!stk.empty())
    {


//        if(stk.size() >= 2)
//        {
//            typename binary_search_tree<tkey, tvalue>::node* parent = *(stk.top());
//            stk.pop();
//            up = (*(stk.top()))->left_subtree == parent;
//            if(down && up)
//            {
//                binary_search_tree<tkey, tvalue>::double_right_rotation(*stk.top());
//
//            }
//            else if(!down && !up)
//            {
//                binary_search_tree<tkey, tvalue>::double_left_rotation(*stk.top());
//
//            }
//            else if(!down)
//            {
//                binary_search_tree<tkey, tvalue>::big_right_rotation(*stk.top());
//            }
//            else
//            {
//                binary_search_tree<tkey, tvalue>::big_left_rotation(*stk.top());
//            }
//
//        }

        while(stk.size() >= 3)
        {

            typename binary_search_tree<tkey, tvalue>::node* parent = *(stk.top());
            stk.pop();
            down = (*(stk.top()))->left_subtree == parent;

            typename binary_search_tree<tkey, tvalue>::node* grand = (*(stk.top()));
            stk.pop();
            up = (*(stk.top()))->left_subtree == grand;

            if(down && up)
            {
                binary_search_tree<tkey, tvalue>::double_right_rotation(*stk.top());

            }
            else if(!down && !up)
            {
                binary_search_tree<tkey, tvalue>::double_left_rotation(*stk.top());

            }
            else if(!down)
            {
                binary_search_tree<tkey, tvalue>::big_right_rotation(*stk.top());
            }
            else
            {
                binary_search_tree<tkey, tvalue>::big_left_rotation(*stk.top());
            }

        }

        if(stk.size() >= 2)
        {

            typename binary_search_tree<tkey, tvalue>::node* parent = *(stk.top());
            stk.pop();
            up = (*(stk.top()))->left_subtree == parent;

            if(up)
            {
                binary_search_tree<tkey, tvalue>::small_right_rotation(*stk.top());
            }
            else
            {
                binary_search_tree<tkey, tvalue>::small_left_rotation(*stk.top());
            }
        }
        else if(stk.size() == 1)
        {
            stk.pop();
        }




//        current_node = *(stk.top());
//
//        stk.pop();
//
//        if(!stk.empty())
//        {
//            down = up;
//            up = (*(stk.top()))->left_subtree == current_node;
//        }
    }

}



template<
    typename tkey,
    typename tvalue>
splay_tree<tkey, tvalue>::splay_tree(
        std::function<int(tkey const &, tkey const &)> keys_comparer,
        allocator *allocator,
        logger *logger,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) : binary_search_tree<tkey, tvalue>(keys_comparer, allocator, logger, insertion_strategy, disposal_strategy) {}



template<typename tkey, typename tvalue>
void splay_tree<tkey, tvalue>::insert_inside(const tkey &key, const tvalue &value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
{
    insert_inside_universal(key, value, stk);
}

template<typename tkey, typename tvalue>
void splay_tree<tkey, tvalue>::insert_inside(const tkey &key, tvalue&& value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
{
    insert_inside_universal(key, std::move(value), stk);
}


template<typename tkey, typename tvalue>
template<typename universal_tvalue>
void splay_tree<tkey, tvalue>::insert_inside_universal(const tkey &key, universal_tvalue &&val,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
{
    (*stk.top()) = reinterpret_cast<binary_search_tree<tkey, tvalue>::node*>(allocator_guardant::allocate_with_guard(sizeof(typename binary_search_tree<tkey, tvalue>::node), 1));
    try {
        allocator::construct(*stk.top(), key, std::forward<universal_tvalue>(val));
    }
    catch(...)
    {
        allocator_guardant::deallocate_with_guard(*stk.top());
        throw;
    }

    splay(stk);
}


template<typename tkey, typename tvalue>
tvalue splay_tree<tkey, tvalue>::dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**> &stk)
{
    splay(stk);

    tvalue res = binary_search_tree<tkey, tvalue>::_root->value;

    typename binary_search_tree<tkey, tvalue>::node* left = binary_search_tree<tkey, tvalue>::_root ->left_subtree;
    typename binary_search_tree<tkey, tvalue>::node* right = binary_search_tree<tkey, tvalue>::_root ->right_subtree;

    allocator::destruct(binary_search_tree<tkey, tvalue>::_root);
    allocator_guardant::deallocate_with_guard(binary_search_tree<tkey, tvalue>::_root);

    typename binary_search_tree<tkey, tvalue>::node* current_node = left;

    std::stack<typename binary_search_tree<tkey, tvalue>::node**> path;

    path.push(&current_node);

    while(current_node->right_subtree != nullptr)
    {

        current_node = current_node->right_subtree;
        path.push(&current_node);
    }

    splay(path);


    binary_search_tree<tkey, tvalue>::_root = left;
    binary_search_tree<tkey, tvalue>::_root->right_subtree = right;

    return res;


}


#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H