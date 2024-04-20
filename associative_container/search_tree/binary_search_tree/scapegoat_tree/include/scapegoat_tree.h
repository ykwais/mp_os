#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SCAPEGOAT_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SCAPEGOAT_TREE_H

#include <binary_search_tree.h>
#include <queue>

template<
    typename tkey,
    typename tvalue>
class scapegoat_tree final:
    public binary_search_tree<tkey, tvalue>
{

private:
    
    struct scapegoat_node final:
        binary_search_tree<tkey, tvalue>::node
    {
    public:

        size_t _count_nodes;

        explicit scapegoat_node(const tkey& key, const tvalue& value);

        explicit scapegoat_node(const tkey& key, tvalue&& value);

    public:

        void update_count_nodes() noexcept
        {
            size_t left = binary_search_tree<tkey, tvalue>::node::left_subtree == nullptr ? 0 : static_cast<scapegoat_node*>(binary_search_tree<tkey, tvalue>::node::left_subtree)->_count_nodes;
            size_t right = binary_search_tree<tkey, tvalue>::node::right_subtree == nullptr ? 0 : static_cast<scapegoat_node*>(binary_search_tree<tkey, tvalue>::node::right_subtree)->_count_nodes;
            _count_nodes = left + right + 1;
        }

        bool need_balance(double alpha) noexcept
        {
            size_t left = binary_search_tree<tkey, tvalue>::node::left_subtree == nullptr ? 0 : static_cast<scapegoat_node*>(binary_search_tree<tkey, tvalue>::node::left_subtree)->_count_nodes;
            size_t right = binary_search_tree<tkey, tvalue>::node::right_subtree == nullptr ? 0 : static_cast<scapegoat_node*>(binary_search_tree<tkey, tvalue>::node::right_subtree)->_count_nodes;

            return !(( _count_nodes * alpha >= left ) && (_count_nodes * alpha >= right));
        }


    };


public:
    
    explicit scapegoat_tree(
        std::function<int(tkey const &, tkey const &)> keys_comparer,
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        double alpha = 0.5,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    ~scapegoat_tree() noexcept final = default;
    
    scapegoat_tree(
        scapegoat_tree<tkey, tvalue> const &other) = default;
    
    scapegoat_tree<tkey, tvalue> &operator=(
        scapegoat_tree<tkey, tvalue> const &other) = default;
    
    scapegoat_tree(
        scapegoat_tree<tkey, tvalue> &&other) noexcept = default;
    
    scapegoat_tree<tkey, tvalue> &operator=(
        scapegoat_tree<tkey, tvalue> &&other) noexcept = default;

public:
    
    void setup_alpha(
        double alpha);


private:

    double _alpha;

    void insert_inside(const tkey& key, const tvalue& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk) override;

    void insert_inside(const tkey& key, tvalue&& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk) override;

    template<typename universal_tvalue>
    void insert_inside_universal(const tkey& key, universal_tvalue&& val, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk);

    void elems_to_vector(typename binary_search_tree<tkey, tvalue>::node** goat);

    void rebuilding_tree(typename binary_search_tree<tkey, tvalue>::node** top, std::vector<scapegoat_node*>::iterator begin, std::vector<scapegoat_node*>::iterator end);

    void check_after_set_new_alpha(typename binary_search_tree<tkey, tvalue>::node** root);

    tvalue dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stk) override;

};


template<typename tkey, typename tvalue>
scapegoat_tree<tkey, tvalue>::scapegoat_node::scapegoat_node(const tkey &key, const tvalue &value) : binary_search_tree<tkey, tvalue>::node(key, value), _count_nodes(1) {}

template<typename tkey, typename tvalue>
scapegoat_tree<tkey, tvalue>::scapegoat_node::scapegoat_node(const tkey &key, tvalue&& value) : binary_search_tree<tkey, tvalue>::node(key, std::move(value)), _count_nodes(1) {}


template<
    typename tkey,
    typename tvalue>
scapegoat_tree<tkey, tvalue>::scapegoat_tree(
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger,
    double alpha,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) : binary_search_tree<tkey, tvalue>(keys_comparer, allocator, logger, insertion_strategy, disposal_strategy)
{
    alpha = alpha < 0.5 ? 0.5 : alpha > 1 ? 1 : alpha;
    _alpha = alpha;
}


template<
    typename tkey,
    typename tvalue>
void scapegoat_tree<tkey, tvalue>::setup_alpha(
    double alpha)
{
    alpha = alpha < 0.5 ? 0.5 : alpha > 1 ? 1 : alpha;
    _alpha = alpha;

    check_after_set_new_alpha(&(binary_search_tree<tkey, tvalue>::_root));
}

template<typename tkey, typename tvalue>
void scapegoat_tree<tkey, tvalue>::insert_inside(const tkey &key, const tvalue &value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
{
    insert_inside_universal(key, value, stk);
}

template<typename tkey, typename tvalue>
void scapegoat_tree<tkey, tvalue>::insert_inside(const tkey &key, tvalue&& value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
{
    insert_inside_universal(key, std::move(value), stk);
}

template<typename tkey, typename tvalue>
template<typename universal_tvalue>
void scapegoat_tree<tkey, tvalue>::insert_inside_universal(const tkey &key, universal_tvalue &&val,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stk)
{
    (*stk.top()) = static_cast<binary_search_tree<tkey, tvalue>::node*>(static_cast<scapegoat_node*>(allocator_guardant::allocate_with_guard(sizeof(scapegoat_node), 1)));
    try {
        allocator::construct(*stk.top(), key, std::forward<universal_tvalue>(val));
    }
    catch(...)
    {
        allocator_guardant::deallocate_with_guard(*stk.top());
        throw;
    }

    typename binary_search_tree<tkey, tvalue>::node** goat = nullptr;

    while(!stk.empty())
    {
        static_cast<scapegoat_node*>(*stk.top())->update_count_nodes();
        bool need_rebalance = static_cast<scapegoat_node*>(*stk.top())->need_balance(_alpha);
        if(need_rebalance)
        {
            goat = stk.top();
        }
        stk.pop();
    }


    if(goat != nullptr)
    {
        elems_to_vector(goat);
    }

}

template<typename tkey, typename tvalue>
void scapegoat_tree<tkey, tvalue>::elems_to_vector(typename binary_search_tree<tkey, tvalue>::node **goat)
{
    std::vector<scapegoat_node*> vctr;

    std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;

    stk.push(goat);

    while((*(stk.top()))->left_subtree != nullptr)
    {
        stk.push(&((*stk.top())->left_subtree));
    }

    typename binary_search_tree<tkey, tvalue>::infix_iterator it(stk, new typename binary_search_tree<tkey, tvalue>::iterator_data);
    typename binary_search_tree<tkey, tvalue>::infix_iterator end(std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), nullptr);

    for(; it != end; ++it)
    {
        vctr.push_back(static_cast<scapegoat_node*>(*(it.get_node())));
    }

    auto begin = vctr.begin();
    auto endvec = vctr.end();

    rebuilding_tree(goat, begin, endvec);

}

template<typename tkey, typename tvalue>
void scapegoat_tree<tkey, tvalue>::rebuilding_tree(typename binary_search_tree<tkey, tvalue>::node **top, std::vector<scapegoat_node*>::iterator begin, std::vector<scapegoat_node*>::iterator end)
{
    size_t size = std::distance(begin, end);

    if(size > 0)
    {
        auto mid = begin + size/2;

        *top = *mid;

        (*top)->left_subtree = nullptr;
        (*top)->right_subtree = nullptr;

        rebuilding_tree(&((*top)->left_subtree), begin, mid);
        rebuilding_tree(&((*top)->right_subtree), mid + 1, end);

        static_cast<scapegoat_node*>(*top)->update_count_nodes();
    }

}

template<typename tkey, typename tvalue>
void scapegoat_tree<tkey, tvalue>::check_after_set_new_alpha(typename binary_search_tree<tkey, tvalue>::node ** root)
{
  if(*root != nullptr)
  {
      bool need = static_cast<scapegoat_node*>(*root)->need_balance(_alpha);
      if(need)
      {
          elems_to_vector(root);
      }
      else
      {
          check_after_set_new_alpha(&((*root)->left_subtree));
          check_after_set_new_alpha(&((*root)->right_subtree));
      }
  }
}

template<typename tkey, typename tvalue>
tvalue scapegoat_tree<tkey, tvalue>::dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**> &stk)
{

    tvalue res = (*stk.top())->value;

    typename binary_search_tree<tkey, tvalue>::node* current_node = *stk.top();

    if(current_node->left_subtree == nullptr && current_node->right_subtree == nullptr)
    {
        *stk.top() = nullptr;
        stk.pop();
    }
    else if(current_node->right_subtree == nullptr || current_node->left_subtree == nullptr)
    {
        typename binary_search_tree<tkey, tvalue>::node* update_node = current_node->right_subtree != nullptr ? (current_node->right_subtree) : (current_node->left_subtree);

        *stk.top() = update_node;

        //stk.pop();
    }
    else
    {
        std::queue<typename binary_search_tree<tkey, tvalue>::node**> que;

        typename binary_search_tree<tkey, tvalue>::node** update = &((*stk.top())->left_subtree);


        while((*update)->right_subtree != nullptr)
        {
            update = &((*update)->right_subtree);
            que.push(update);
        }

        typename binary_search_tree<tkey, tvalue>::node* previous_node = *stk.top();

        *(stk.top()) = *update;

        *update = (*update)->left_subtree;


        (*stk.top())->left_subtree = previous_node->left_subtree == *(stk.top()) ? *update : previous_node->left_subtree;
        (*stk.top())->right_subtree = previous_node->right_subtree;

        if(!que.empty())
        {
            stk.push(&((*stk.top())->left_subtree));
        }

        while(!que.empty())
        {
            if(que.size() == 1)//crutch
            {
                break;
            }

            stk.push(que.front());
            que.pop();
        }

    }


    allocator::destruct(current_node);
    allocator_guardant::deallocate_with_guard(current_node);

    typename binary_search_tree<tkey, tvalue>::node** goat = nullptr;




    while(!stk.empty())
    {
        static_cast<scapegoat_node*>(*stk.top())->update_count_nodes();
        bool need_rebalance = static_cast<scapegoat_node*>(*stk.top())->need_balance(_alpha);
        if(need_rebalance)
        {
            goat = stk.top();
        }
        stk.pop();
    }


    if(goat != nullptr)
    {
        elems_to_vector(goat);
    }

    return res;
}


#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SCAPEGOAT_TREE_H