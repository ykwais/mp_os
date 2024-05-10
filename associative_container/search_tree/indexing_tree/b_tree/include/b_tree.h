#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H

#include <search_tree.h>
#include <allocator_guardant.h>
#include <logger_guardant.h>

template<
    typename tkey,
    typename tvalue>
class b_tree:
    public search_tree<tkey, tvalue>
{

protected:

    enum class insertion_of_existent_key_attempt_strategy
    {
        update_value,
        throw_an_exception
    };

    class insertion_of_existent_key_attempt_exception final:
            public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit insertion_of_existent_key_attempt_exception(
                tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

    enum class disposal_of_nonexistent_key_attempt_strategy
    {
        do_nothing,
        throw_an_exception
    };


    class disposal_of_nonexistent_key_attempt_exception final:
            public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit disposal_of_nonexistent_key_attempt_exception(
                tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };


    class obtaining_of_nonexistent_key_attempt_exception final:
            public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit obtaining_of_nonexistent_key_attempt_exception(
                tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

public:
    using pair_node_tree = std::pair<tkey, tvalue>;

private:

    size_t _t;

    static size_t _min_keys_in_node;

    static size_t _max_keys_in_node;

protected:

    struct btree_node
    {
        size_t _virtual_size;

        btree_node** _subtrees;

        pair_node_tree* _pairs_of_node;

        explicit btree_node(pair_node_tree* pairs, btree_node** subtrees, size_t size);

        ~btree_node() noexcept;

    };


    mutable btree_node* _root;

    insertion_of_existent_key_attempt_strategy _insert_exist_strategy;

    disposal_of_nonexistent_key_attempt_strategy _dispose_nonexist_strategy;

    size_t _size;

public:

    class infix_iterator final
    {

        std::stack<std::pair<btree_node**, size_t>> _path;

        size_t _index;

    public:

        bool operator==( infix_iterator const &other) const noexcept;

        bool operator!=(infix_iterator const &other) const noexcept;

        infix_iterator& operator++();

        infix_iterator operator++(int);

        infix_iterator& operator--();

        infix_iterator operator--(int);

        //std::tuple<size_t, size_t, tkey const &, tvalue &> operator*() const;

        pair_node_tree& operator*() const noexcept;

        pair_node_tree* operator->() const noexcept;

        size_t amount_pairs() const noexcept;

        bool is_term_node() const noexcept;

        explicit infix_iterator(const std::stack<std::pair<btree_node**, size_t>>& path = std::stack<std::pair<btree_node**, size_t>>(), size_t index = 0);

    };

    class infix_const_iterator final
    {

    public:

        bool operator==(
            infix_const_iterator const &other) const noexcept;

        bool operator!=(
            infix_const_iterator const &other) const noexcept;

        infix_const_iterator &operator++();

        infix_const_iterator operator++(
            int not_used);

        std::tuple<size_t, size_t, tkey const &, tvalue const &> operator*() const;

    };

public:

    void insert(
        tkey const &key,
        tvalue const &value) override;

    void insert(
        tkey const &key,
        tvalue &&value) override;

    tvalue const &obtain(
        tkey const &key) override;

    tvalue dispose(
        tkey const &key) override;

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) override;

public:

    explicit b_tree(
        size_t t,
        std::function<int(tkey const &, tkey const &)> keys_comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename b_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = b_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename b_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = b_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

    b_tree(
        b_tree<tkey, tvalue> const &other);

    b_tree<tkey, tvalue> &operator=(
        b_tree<tkey, tvalue> const &other);

    b_tree(
        b_tree<tkey, tvalue> &&other) noexcept;

    b_tree<tkey, tvalue> &operator=(
        b_tree<tkey, tvalue> &&other) noexcept;

    ~b_tree();

public:

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    infix_const_iterator cbegin_infix() const noexcept;

    infix_const_iterator cend_infix() const noexcept;

private:

    btree_node* copy_subtree(btree_node* parent, btree_node* copirka);

    void destroy_subtree(btree_node* node);

    std::pair<size_t, bool> find_index(const tkey& key, btree_node* node) const noexcept;

    std::pair<std::stack<std::pair<btree_node**, size_t>>, std::pair<size_t,bool>> find_path(const tkey& key) const noexcept;

    bool is_terma(btree_node* node) const noexcept;

    void insert_in_btree_node(btree_node* current_node, btree_node* right_node, pair_node_tree&& pair, size_t index) noexcept;

    btree_node* remove_from_btree_node(btree_node* current_node, size_t index, bool need_remove_left) noexcept;

    void insert_inside(pair_node_tree&& data, std::stack<std::pair<btree_node**, size_t>>& path);

    void split_node(std::stack<std::pair<btree_node**, size_t>>& path, btree_node*& node, size_t index);


protected:

    btree_node* create_btree_node(size_t t) const;

    void destroy_btree_node(btree_node* node);

};

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::btree_node::btree_node(pair_node_tree* pairs, btree_node** subtrees, size_t size): _subtrees(subtrees), _virtual_size(0), _pairs_of_node(pairs)
{

    for(int i = 0, end = _max_keys_in_node + 2; i != end; ++i)
    {
        _subtrees[i] = nullptr;
    }
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::btree_node::~btree_node() noexcept
{
    _virtual_size = 0;
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::infix_iterator::infix_iterator(const std::stack<std::pair<btree_node**, size_t>>& path , size_t index) : _path(path), _index(index) {}

template<typename tkey, typename tvalue>
bool b_tree<tkey, tvalue>::infix_iterator::operator==(typename b_tree::infix_iterator const &other) const noexcept
{
    return (_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.size() == other._path.size() && *(_path.top().first) == *(other._path.top().first) && _index == other._index);
}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_iterator::operator!=(
    typename b_tree::infix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<typename tkey,typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator &b_tree<tkey, tvalue>::infix_iterator::operator++()
{
    if(_path.empty())
    {
        return *this;
    }

    if(is_term_node())
    {
        _index += 1;
        while(!_path.empty() && _index == *(_path.top().first)->_virtual_size)
        {
            _index = _path.top().second;
            _path.pop();
        }
    }
    else
    {
        _path.push(std::make_pair(*(_path.top().first)->subtrees[_index + 1], _index + 1));
        _index = 0;
        while(!is_term_node())
        {
            _path.push(std::make_pair(*(_path.top().first)->subtrees[_index], _index));
            _index = 0;
        }
    }

    return *this;

}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::infix_iterator::operator++(int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::infix_iterator& b_tree<tkey, tvalue>::infix_iterator::operator--()
{
    if(_path.empty())
    {
        return *this;
    }

    if(is_term_node())
    {
        if(_index == 0)
        {
            while(!_path.empty() && _index == 0)
            {
                _index = _path.top().second;
                _path.pop();
            }

        }

        --_index;

    }
    else
    {
        while(!is_term_node())
        {
            _path.push(std::make_pair(&(*(_path.top().first)->subtrees[_index]), _index));
            _index = *(_path.top().first)->_virtual_size;
        }
        --_index;
    }
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::infix_iterator::operator--(int)
{
    auto tmp = *this;
    --(*this);
    return tmp;
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::pair_node_tree& b_tree<tkey, tvalue>::infix_iterator::operator*() const noexcept
{
    return (*(_path.top().first)->_pairs_of_node[_index]);
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::pair_node_tree* b_tree<tkey, tvalue>::infix_iterator::operator->() const noexcept
{
    return &(*(_path.top().first)->_pairs_of_node[_index]);
}

template<typename tkey, typename tvalue>
bool b_tree<tkey, tvalue>::infix_iterator::is_term_node() const noexcept
{
    return (_path.empty()) || (*(_path.top().first) == nullptr)  || (*(_path.top().first)->subtrees[0] == nullptr);
}

template<typename tkey, typename tvalue>
size_t b_tree<tkey, tvalue>::infix_iterator::amount_pairs() const noexcept
{
    return *(_path.top().first)->_virtual_size;
}



template<typename tkey, typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_iterator::operator==(
    b_tree::infix_const_iterator const &other) const noexcept
{

}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_iterator::operator!=(
    b_tree::infix_const_iterator const &other) const noexcept
{
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator &b_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::infix_const_iterator::operator++(
    int not_used)
{
}

template<
    typename tkey,
    typename tvalue>
std::tuple<size_t, size_t, tkey const &, tvalue const &> b_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::insert(tkey const &key, tvalue const &value)///////////////////////////
{
    auto path = find_path(key);

    if(path.second.second == true && _insert_exist_strategy == insertion_of_existent_key_attempt_strategy::throw_an_exception)
    {
        throw insertion_of_existent_key_attempt_exception(key);
    }
    else
    {
        pair_node_tree pair(key, value);
        insert_inside(std::move(pair), path.first);
    }

}

template<typename tkey,typename tvalue>
void b_tree<tkey, tvalue>::insert(tkey const &key,tvalue &&value)
{
    auto path = find_path(key);

    if(path.second.second == true && _insert_exist_strategy == insertion_of_existent_key_attempt_strategy::throw_an_exception)
    {
        throw insertion_of_existent_key_attempt_exception(key);
    }
    else
    {
        pair_node_tree pair(key, std::move(value));
        insert_inside(std::move(pair), path.first);
    }

}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::insert_inside(b_tree::pair_node_tree &&data, std::stack<std::pair<btree_node **, size_t>> &path)
{
    if(_root == nullptr)
    {
        btree_node* new_node = create_btree_node(_t);

        new_node->_pairs_of_node[0] = std::move(data);

        ++new_node->_virtual_size;

        _root = new_node;

        return;
    }

    auto [index, found] = find_index(data.first, *path.top().first);

//    auto tmp = pair_node_tree() ;
//
//    auto temp = tmp.first;



    if(found)
    {
        allocator::destruct(&((*(path.top().first))->_pairs_of_node[index].second));////////////////////////////////////////////////////////////////////////////////////////////
        allocator::construct(&((*(path.top().first))->_pairs_of_node[index].second), std::move(data.second));
        return;
    }

    insert_in_btree_node(*path.top().first, nullptr, std::move(data), index);

    btree_node* node = *path.top().first;

    while(!path.empty() && (*path.top().first)->_virtual_size > _max_keys_in_node)
    {
        split_node(path, node, index);
    }

}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::split_node(std::stack<std::pair<btree_node**, size_t>> &path, b_tree::btree_node *&node,size_t index)
{
    btree_node* new_node = create_btree_node(_t);

    if(path.size() == 1)
    {
        try
        {
            btree_node* tmp = create_btree_node(_t);

            _root = std::exchange(tmp, _root);

            _root->_subtrees[0] = tmp;

            path.pop();

            path.push(std::make_pair(&_root, 0));
            path.push(std::make_pair(&((*path.top().first)->_subtrees[0]), 0));
        }

        catch(...)
        {
            destroy_btree_node(new_node);
            throw;
        }
    }

    size_t mediana = (*path.top().first)->_virtual_size / 2;

    auto pair = path.top();

    auto ptr_node = *pair.first;

    path.pop();

    insert_in_btree_node(*path.top().first, new_node, std::move(ptr_node->_pairs_of_node[mediana - 1]), pair.second);

    new_node->_subtrees[0] = is_terma(ptr_node) ? nullptr : ptr_node->_subtrees[mediana];

    new_node->_virtual_size = ptr_node->_virtual_size - mediana;

    for(size_t i = mediana; i < ptr_node->_virtual_size; ++i)
    {
        allocator::construct(&(new_node->_pairs_of_node[i-mediana]),  std::move(ptr_node->_pairs_of_node[i]));
        new_node->_subtrees[i-mediana + 1] = ptr_node->_subtrees[i+1];
    }

    if(is_terma(ptr_node))
    {
        new_node->_subtrees[0] = nullptr;
    }

    ptr_node->_virtual_size = mediana - 1;

    if(node == ptr_node)///////////////////////////////////////
    {
        if(index == mediana - 1)
        {
            node = *path.top().first;
            index = pair.second;
        }
        else if(index >= mediana)
        {
            node = new_node;
            index -= mediana;
        }
    }

}


template<
    typename tkey,
    typename tvalue>
tvalue const &b_tree<tkey, tvalue>::obtain(
    tkey const &key)
{
    std::cout<<"todo"<<std::endl;
    return tvalue();
}

template<
    typename tkey,
    typename tvalue>
tvalue b_tree<tkey, tvalue>::dispose(
    tkey const &key)
{
    std::cout<<"todo"<<std::endl;
    return tvalue();
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> b_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    std::cout<<"todo"<<std::endl;
    return std::vector<typename associative_container<tkey, tvalue>::key_value_pair>();

}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    size_t t,
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger,
    typename b_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename b_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) : search_tree<tkey, tvalue>(keys_comparer, logger, allocator), _t(t), _root(nullptr), _size(0), _insert_exist_strategy(insertion_strategy), _dispose_nonexist_strategy(disposal_strategy)
{
    if(_t < 2)
    {
        throw std::logic_error("Incorrect t!");
    }
    _min_keys_in_node = _t - 1;
    _max_keys_in_node = 2 * _t - 1;
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    b_tree<tkey, tvalue> const &other) : search_tree<tkey, tvalue>(other), _size(other._size), _t(other._t), _insert_exist_strategy(other._insert_exist_strategy), _dispose_nonexist_strategy(other._dispose_nonexist_strategy)

{
    _max_keys_in_node = other._max_keys_in_node;
    _min_keys_in_node = other._min_keys_in_node;
    _root = copy_subtree(nullptr, other._root);
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue> &b_tree<tkey, tvalue>::operator=(b_tree<tkey, tvalue> const &other)
{
    if(this != &other)
    {
        btree_node* tmp = copy_subtree(nullptr, other._root);
        destoroy_subtree(_root);
        _root = tmp;
        search_tree<tkey, tvalue>::operator=(other);
        _t = other._t;
        _size = other._size;
        _max_keys_in_node = other._max_keys_in_node;
        _min_keys_in_node = other._min_keys_in_node;
        _insert_exist_strategy = other._insert_exist_strategy;
        _dispose_nonexist_strategy = other._dispose_nonexist_strategy;
    }
    return *this;
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    b_tree<tkey, tvalue> &&other) noexcept : search_tree<tkey, tvalue>(other), _root(other._root), _t(other._t), _size(other._size), _insert_exist_strategy(other._insert_exist_strategy), _dispose_nonexist_strategy(other._dispose_nonexist_strategy)
{
    _max_keys_in_node = other._max_keys_in_node;
    _min_keys_in_node = other._min_keys_in_node;
    other._root = nullptr;
    other._size = 0;
    other._allocator = nullptr;
    other._logger = nullptr;
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue> &b_tree<tkey, tvalue>::operator=(
    b_tree<tkey, tvalue> &&other) noexcept
{
    if(this != &other)
    {
        search_tree<tkey, tvalue>::operator=(other);
        std::swap(_size, other._size);
        std::swap(_root, other._root);
        std::swap(_t, other._t);
        std::swap(_max_keys_in_node, other._max_keys_in_node);
        std::swap(_min_keys_in_node, other._min_keys_in_node);
        std::swap(_insert_exist_strategy, other._insert_exist_strategy);
        std::swap(_dispose_nonexist_strategy, other._dispose_nonexist_strategy);
    }
    return *this;
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::~b_tree()
{
    destroy_subtree(_root);
}

template<typename tkey, typename tvalue>
typename b_tree<tkey, tvalue>::btree_node* b_tree<tkey, tvalue>::copy_subtree(b_tree::btree_node *parent, b_tree::btree_node *copirka)// TODO mb remove parent...
{
    if(copirka == nullptr)
    {
        return nullptr;
    }

    btree_node* new_node = create_btree_node(_t);

    new_node->_virtual_size = copirka->_virtual_size;

    for(int i = 0; i < copirka->_virtual_size; ++i)
    {
        allocator::construct(new_node->_pairs_of_node + i, copirka->_pairs_of_node[i]);
    }

    size_t i = 0;

    try
    {
        for(size_t end = copirka->_virtual_size + 1; i != end; ++i)
        {
            new_node->_subtrees[i] = copy_subtree(new_node, copirka->_subtrees[i]);
        }
    }
    catch(...)
    {
        for(size_t j = 0; j < i; ++j)
        {
            destroy_subtree(new_node->_subtrees[j]);
        }
    }

    return new_node;

}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::destroy_subtree(b_tree::btree_node *node)
{
    if(node == nullptr)
    {
        return;
    }

    for(size_t i = 0; i <= node->_virtual_size; ++i)
    {
        destroy_subtree(node->_subtrees[i]);
    }

    destroy_btree_node(node);
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::btree_node* b_tree<tkey, tvalue>::create_btree_node(size_t t) const
{
    auto pairs_of_node = reinterpret_cast<pair_node_tree*>(search_tree<tkey,tvalue>::allocate_with_guard(sizeof(pair_node_tree), _max_keys_in_node  + 1));

    auto subtreess = reinterpret_cast<b_tree<tkey, tvalue>::btree_node**>(search_tree<tkey,tvalue>::allocate_with_guard(sizeof(b_tree<tkey, tvalue>::btree_node*), _max_keys_in_node + 2));

    auto node = reinterpret_cast<btree_node*>(search_tree<tkey, tvalue>::allocate_with_guard(sizeof(btree_node), 1));

    allocator::construct(node, pairs_of_node, subtreess, t);

    return node;
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::destroy_btree_node(b_tree::btree_node *node)
{
    for(size_t i = 0; i < node->_virtual_size; ++i)
    {
        allocator::destruct(node->_pairs_of_node + i);
    }

    search_tree<tkey, tvalue>::deallocate_with_guard(node->_pairs_of_node);
    search_tree<tkey, tvalue>::deallocate_with_guard(node->_subtrees);
    allocator::destruct(node);
    search_tree<tkey, tvalue>::deallocate_with_guard(node);
}




template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::begin_infix() const noexcept
{
    if(_root == nullptr)
    {
        return infix_iterator();
    }

    std::stack<std::pair<btree_node**, size_t>> stk;

    stk.push(std::make_pair(&(_root), 0));

    while(*(stk.top().first)->_virtual_size > 0 && *(stk.top().first)->subtrees[0] != nullptr)
    {
        stk.push(std::make_pair(&(*(stk.top().first)->subtrees[0]), 0));
    }

    return infix_iterator(stk, 0);

}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::end_infix() const noexcept
{
    return infix_iterator();
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::cbegin_infix() const noexcept
{

}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::cend_infix() const noexcept
{

}


template<typename tkey, typename tvalue>
size_t b_tree<tkey, tvalue>::_min_keys_in_node = 1;

template<typename tkey, typename tvalue>
size_t b_tree<tkey, tvalue>::_max_keys_in_node = 3;

template<typename tkey, typename tvalue>
bool b_tree<tkey, tvalue>::is_terma(b_tree::btree_node *node) const noexcept
{
    return node == nullptr || node->_subtrees[0] == nullptr;
}

template<typename tkey, typename tvalue>
std::pair<size_t, bool> b_tree<tkey, tvalue>::find_index(const tkey &key, b_tree::btree_node *node) const noexcept
{
    size_t begin = 0;
    size_t end = node->_virtual_size;

    while(end > begin)
    {
        size_t middle = begin +  (end - begin)/2;

        int cmp = search_tree<tkey, tvalue>::_keys_comparer(key, node->_pairs_of_node[middle].first);

        if(cmp > 0)
        {
            begin = middle + 1;
        }
        else if(cmp < 0)
        {
            end = middle;
        }
        else
        {
            return std::make_pair(middle, true);
        }

    }

    if(begin == node->_virtual_size)
    {
        return std::make_pair(node->_virtual_size, false);
    }

    if(search_tree<tkey, tvalue>::_keys_comparer(key, node->_pairs_of_node[begin].first))
    {
        return std::make_pair(begin + 1, false);//>
    }
    else
    {
        return std::make_pair(begin, false);//<
    }

}

template<typename tkey, typename tvalue>
std::pair<std::stack<std::pair<typename b_tree<tkey, tvalue>::btree_node**, size_t>>, std::pair<size_t,bool>> b_tree<tkey, tvalue>::find_path(const tkey &key) const noexcept
{
    if(_root == nullptr)
    {
        std::stack<std::pair<b_tree<tkey, tvalue>::btree_node**, size_t>> stk;
        stk.push(std::make_pair(&_root, size_t(0)));
        return std::make_pair(std::move(stk), std::make_pair(size_t(1), false));
    }
    else
    {
        std::stack<std::pair<b_tree<tkey, tvalue>::btree_node**, size_t>> stk;

        stk.push(std::make_pair(&_root, size_t(0)));

        auto [index, found] = find_index(key, *(stk.top().first));

        while(!found && !is_terma(*(stk.top().first)))
        {
            //stk.push(std::make_pair(&(*stk.top().first->_subtrees[index]), index));
            stk.push(std::make_pair(&((*stk.top().first)->_subtrees[index]), index));
            auto tmp = find_index(key, *(stk.top().first));
            index = tmp.first;
            found = tmp.second;
        }

        return found ? std::make_pair(std::move(stk), std::make_pair(index, found)) : std::make_pair(std::move(stk), std::make_pair((*stk.top().first)->_virtual_size + 1 , found));

    }

}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::insert_in_btree_node(btree_node* current_node, btree_node* right_node, pair_node_tree&& pair, size_t index) noexcept
{
    for(size_t i = current_node->_virtual_size; i > index; --i)
    {
        allocator::construct(&(current_node->_pairs_of_node[i]), std::move(current_node->_pairs_of_node[i-1]));
        allocator::destruct(&(current_node->_pairs_of_node[i-1]));
        current_node->_subtrees[i+1] = current_node->_subtrees[i];
    }
    allocator::construct(&(current_node->_pairs_of_node[index]), std::move(pair));
    current_node->_subtrees[index + 1] = right_node;
    current_node->_virtual_size += 1;
}

template<typename tkey, typename tvalue>
typename b_tree<tkey, tvalue>::btree_node* b_tree<tkey, tvalue>::remove_from_btree_node(btree_node *current_node, size_t index, bool need_remove_left) noexcept
{
    btree_node* parrot = need_remove_left ? current_node->_subtrees[index] : current_node->_subtrees[index + 1];

    bool is_bottom = is_terma(current_node);

    for(size_t i = index; i < current_node->_virtual_size - 1; ++i)
    {
        std::swap(current_node->_pairs_of_node[i], current_node->_pairs_of_node[i+1]);
        if(need_remove_left)
        {
            current_node->_subtrees[i] = current_node->_subtrees[i+1];
        }
        else
        {
            current_node->_subtrees[i+1] = current_node->_subtrees[i+2];
        }

    }

    if(need_remove_left)
    {
        current_node->_subtrees[current_node->_virtual_size - 1] = current_node->_subtrees[current_node->_virtual_size];
    }

    allocator::destruct(&(current_node->_pairs_of_node[current_node->_virtual_size - 1]));

    current_node->_virtual_size -= 1;

    if(is_bottom && index == 0 && need_remove_left)
    {
        parrot = nullptr;
    }

    if(is_bottom)
    {
        current_node->_subtrees[0] = nullptr;
    }

    return parrot;
}


template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::insertion_of_existent_key_attempt_exception(const tkey &key) :
    std::logic_error("Attempt to insert already existing key inside the tree."), _key(key) {}


template<typename tkey, typename tvalue>
tkey const &b_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception(
        tkey const &key):
        std::logic_error("Attempt to obtain a value by non-existing key from the tree."), _key(key) {}

template<typename tkey,typename tvalue>
tkey const &b_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
        typename tkey,
        typename tvalue>
b_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception(
        tkey const &key):
        std::logic_error("Attempt to dispose a value by non-existing key from the tree."), _key(key) {}

template<
        typename tkey,
        typename tvalue>
tkey const &b_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}



#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H