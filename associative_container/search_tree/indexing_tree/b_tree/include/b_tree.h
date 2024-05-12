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

        void update_index(size_t new_index)
        {
            _index = new_index;
        }

    };

    class infix_const_iterator final
    {

        std::stack<std::pair<btree_node**, size_t>> _path;

        size_t _index;

    public:

        bool operator==( infix_const_iterator const &other) const noexcept;

        bool operator!=(infix_const_iterator const &other) const noexcept;

        infix_const_iterator& operator++();

        infix_const_iterator operator++(int);

        infix_const_iterator& operator--();

        infix_const_iterator operator--(int);

        //std::tuple<size_t, size_t, tkey const &, tvalue &> operator*() const;

        pair_node_tree& operator*() const noexcept;

        pair_node_tree* operator->() const noexcept;

        size_t amount_pairs() const noexcept;

        bool is_term_node() const noexcept;

        explicit infix_const_iterator(const std::stack<std::pair<btree_node**, size_t>>& path = std::stack<std::pair<btree_node**, size_t>>(), size_t index = 0);

        void update_index(size_t new_index)
        {
            _index = new_index;
        }


    };

    friend class infix_iterator;
    friend class infix_const_iterator;

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

    void update(tkey const& key, tvalue const& value);

    void update(tkey const& key, tvalue&& value);

public:

    explicit b_tree(
        size_t t,
        std::function<int(tkey const &, tkey const &)> keys_comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        logger *logger = nullptr);

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

    btree_node* remove_from_btree_node(btree_node* current_node, size_t index, bool need_remove_left = true) noexcept;

    void insert_inside(pair_node_tree&& data, std::stack<std::pair<btree_node**, size_t>>& path);

    void split_node(std::stack<std::pair<btree_node**, size_t>>& path, btree_node*& node, size_t& index);

    tvalue dispose_inside(std::stack<std::pair<btree_node**, size_t>>& path, size_t& index) noexcept;

    void rebalance_node(std::stack<std::pair<btree_node**, size_t>>& path, btree_node*& node, size_t& index);

    void check_tree(btree_node *node, size_t depth);

    static bool exist(btree_node* node, size_t index) noexcept;




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
        while(!_path.empty() && _index == (*(_path.top().first))->_virtual_size)
        {
            _index = _path.top().second;
            _path.pop();
        }
    }
    else
    {
        _path.push(std::make_pair(&((*(_path.top().first))->_subtrees[_index + 1]), _index + 1));
        _index = 0;
        while(!is_term_node())
        {
            _path.push(std::make_pair(&((*(_path.top().first))->_subtrees[_index]), _index));
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
            _path.push(std::make_pair(&((*(_path.top().first))->_subtrees[_index]), _index));
            _index = (*(_path.top().first))->_virtual_size;
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
    return ((*(_path.top().first))->_pairs_of_node[_index]);
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::pair_node_tree* b_tree<tkey, tvalue>::infix_iterator::operator->() const noexcept
{
    return &((*(_path.top().first))->_pairs_of_node[_index]);
}

template<typename tkey, typename tvalue>
bool b_tree<tkey, tvalue>::infix_iterator::is_term_node() const noexcept
{
    return (_path.empty()) || (*(_path.top().first) == nullptr)  || ((*(_path.top().first))->_subtrees[0] == nullptr);
}

template<typename tkey, typename tvalue>
size_t b_tree<tkey, tvalue>::infix_iterator::amount_pairs() const noexcept
{
    return *(_path.top().first)->_virtual_size;
}


template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(const std::stack<std::pair<btree_node**, size_t>>& path , size_t index) : _path(path), _index(index) {}

template<typename tkey, typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_iterator::operator==(typename b_tree::infix_const_iterator const &other) const noexcept
{
    return (_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.size() == other._path.size() && *(_path.top().first) == *(other._path.top().first) && _index == other._index);
}

template<typename tkey,typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_iterator::operator!=(typename b_tree::infix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<typename tkey,typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator &b_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
    if(_path.empty())
    {
        return *this;
    }

    if(is_term_node())
    {
        _index += 1;
        while(!_path.empty() && _index == (*(_path.top().first))->_virtual_size)
        {
            _index = _path.top().second;
            _path.pop();
        }
    }
    else
    {
        _path.push(std::make_pair(&((*(_path.top().first))->_subtrees[_index + 1]), _index + 1));
        _index = 0;
        while(!is_term_node())
        {
            _path.push(std::make_pair(&((*(_path.top().first))->_subtrees[_index]), _index));
            _index = 0;
        }
    }

    return *this;

}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::infix_const_iterator::operator++(int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::infix_const_iterator& b_tree<tkey, tvalue>::infix_const_iterator::operator--()
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
            _path.push(std::make_pair(&((*(_path.top().first))->_subtrees[_index]), _index));
            _index = (*(_path.top().first))->_virtual_size;
        }
        --_index;
    }
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::infix_const_iterator::operator--(int)
{
    auto tmp = *this;
    --(*this);
    return tmp;
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::pair_node_tree& b_tree<tkey, tvalue>::infix_const_iterator::operator*() const noexcept
{
    return ((*(_path.top().first))->_pairs_of_node[_index]);
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::pair_node_tree* b_tree<tkey, tvalue>::infix_const_iterator::operator->() const noexcept
{
    return &((*(_path.top().first))->_pairs_of_node[_index]);
}

template<typename tkey, typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_iterator::is_term_node() const noexcept
{
    return (_path.empty()) || (*(_path.top().first) == nullptr)  || ((*(_path.top().first))->_subtrees[0] == nullptr);
}

template<typename tkey, typename tvalue>
size_t b_tree<tkey, tvalue>::infix_const_iterator::amount_pairs() const noexcept
{
    return *(_path.top().first)->_virtual_size;
}


template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::insert(tkey const &key, tvalue const &value)///////////////////////////
{
    auto path = find_path(key);

    if(path.second.second == true )
    {
        throw std::logic_error("Attempt to insert already existing key inside the tree.");
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

    if(path.second.second == true )
    {
        throw std::logic_error("Attempt to insert already existing key inside the tree.");
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

//        new_node->_pairs_of_node[0] = /*std::move(data);*/data;

        allocator::construct(&(new_node->_pairs_of_node[0]), std::move(data));

        ++new_node->_virtual_size;

        _root = new_node;

        //check_tree(_root,0);

        return;
    }

    auto [index, found] = find_index(data.first, *path.top().first);

    if(found)
    {
        allocator::destruct(&((*(path.top().first))->_pairs_of_node[index].second));
        allocator::construct(&((*(path.top().first))->_pairs_of_node[index].second), std::move(data.second));
        //check_tree(_root,0);
        return;
    }

    insert_in_btree_node(*path.top().first, nullptr, std::move(data), index);

    btree_node* node = *path.top().first;

    while(!path.empty() && (*path.top().first)->_virtual_size > _max_keys_in_node)
    {
        split_node(path, node, index);
    }

    //check_tree(_root,0);

}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::split_node(std::stack<std::pair<btree_node**, size_t>> &path, btree_node *&node,size_t& index)
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
    auto path = find_path(key);

    if(path.second.second == false)
    {
        throw std::logic_error("You wanna find non existing value!");
    }
    else
    {
        return ((*(path.first.top().first))->_pairs_of_node[path.second.first]).second;
    }
}

template<typename tkey, typename tvalue>
tvalue b_tree<tkey, tvalue>::dispose(tkey const &key)
{
    auto path = find_path(key);

    if(path.second.second == false )
    {
        throw std::logic_error("Attempt to dispose non existing value!");
    }

//    check_tree(_root, 0);

    return dispose_inside(path.first, path.second.first);
}

template<typename tkey, typename tvalue>
tvalue b_tree<tkey, tvalue>::dispose_inside(std::stack<std::pair<btree_node**, size_t>> &path,size_t& index) noexcept
{
    tvalue res = (*(path.top().first))->_pairs_of_node[index].second;

    if(!is_terma(*path.top().first))
    {
        btree_node* tmp = *path.top().first;

        path.push(std::make_pair(&(*path.top().first)->_subtrees[index], index));

        while(!is_terma(*path.top().first))
        {
            path.push(std::make_pair(&(*path.top().first)->_subtrees[(*path.top().first)->_virtual_size], (*path.top().first)->_virtual_size ));

        }

        std::swap(tmp->_pairs_of_node[index], (*path.top().first)->_pairs_of_node[(*path.top().first)->_virtual_size - 1]);
        index = (*path.top().first)->_virtual_size - 1;
    }

    remove_from_btree_node(*path.top().first, index);

    btree_node* node = *path.top().first;

    while(!path.empty() && (*path.top().first)->_virtual_size < _min_keys_in_node)
    {
        rebalance_node(path, node, index);
    }

    if(node == nullptr)
    {
        return {};
    }

//    auto result = find_path(node->_pairs_of_node[index].first);
//
//    return (*(result.first.top().first))->_pairs_of_node[result.second.first].second;

    //check_tree(_root, 0);

    return res;
}


template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::rebalance_node(std::stack<std::pair<btree_node**, size_t>> &path, b_tree::btree_node *&node, size_t &index)
{
    if(path.empty())
    {
        return;
    }
    if(path.size() == 1)
    {
        if((*path.top().first)->_virtual_size == 0)
        {
            destroy_btree_node(*path.top().first);
            *path.top().first = nullptr;
            node = nullptr;
        }
        path.pop();
        return;
    }
    else
    {
        auto pair = path.top();
        auto current_node = *pair.first;
        path.pop();

        bool need_key = true;

        if(pair.second > 0)
        {
            btree_node* bro = (*path.top().first)->_subtrees[pair.second - 1];

            if(bro->_virtual_size > _min_keys_in_node)
            {
                auto tmp = (*path.top().first)->_pairs_of_node[pair.second - 1];
                std::swap((*path.top().first)->_pairs_of_node[pair.second - 1], bro->_pairs_of_node[bro->_virtual_size - 1]);

                auto tmp_ptr = remove_from_btree_node(bro, bro->_virtual_size -1, false);

                std::swap(current_node->_subtrees[0], tmp_ptr);

                insert_in_btree_node(current_node, tmp_ptr, std::move(tmp), 0);

                ++index;

                need_key = false;
            }
        }

        if(need_key && pair.second < (*path.top().first)->_virtual_size)
        {
            btree_node* bro = (*path.top().first)->_subtrees[pair.second + 1];

            if (bro->_virtual_size > _min_keys_in_node)
            {
                auto tmp = (*path.top().first)->_pairs_of_node[pair.second];
                std::swap((*path.top().first)->_pairs_of_node[pair.second], bro->_pairs_of_node[0]);

                auto tmp_ptr = remove_from_btree_node(bro, 0);

                insert_in_btree_node(current_node, tmp_ptr, std::move(tmp), current_node->_virtual_size);

                need_key = false;
            }
        }

        if (need_key)
        {
            btree_node* parent = *path.top().first;
            btree_node* left = pair.second > 0 ? parent->_subtrees[pair.second - 1] : current_node;
            btree_node* right = pair.second > 0 ? current_node : parent->_subtrees[pair.second + 1];
            size_t parent_index = pair.second > 0 ? pair.second - 1 : pair.second;
            if (pair.second > 0)
            {
                node = left;
                index += left->_virtual_size + 1;
            }

            auto tmp_data = parent->_pairs_of_node[parent_index];

            insert_in_btree_node(left, right->_subtrees[0], std::move(tmp_data), left->_virtual_size);

            for (size_t i = 0; i < right->_virtual_size; ++i)
            {
                insert_in_btree_node(left, right->_subtrees[i + 1], std::move(right->_pairs_of_node[i]), left->_virtual_size);
            }

            remove_from_btree_node(parent, parent_index, false);

            destroy_btree_node(right);

            if (path.size() == 1 && (*path.top().first)->_virtual_size == 0)
            {
                path.pop();
                destroy_btree_node(parent);
                _root = left;
            }
        }


    }
}

template<typename tkey,typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> b_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    if(search_tree<tkey, tvalue>::_keys_comparer(upper_bound, lower_bound) < 0)
    {
        throw std::logic_error("upper less lower!");
    }
    if(_root == nullptr)
    {
        return std::vector<typename associative_container<tkey, tvalue>::key_value_pair>();
    }


    auto [stk, pair2] = find_path(lower_bound);

    size_t index = pair2.first;

    btree_node* node = *(stk.top().first);

    infix_iterator begin(stk, index);

    if(!exist(node, index))
    {
        auto [inner_index, found] = find_index(lower_bound, node);

        if(inner_index > node->_virtual_size - 1)
        {
            inner_index = node->_virtual_size - 1;
        }

        begin.update_index(inner_index);
    }

    while(begin != end_infix() && search_tree<tkey, tvalue>::_keys_comparer(lower_bound, begin->first))
    {
        ++begin;
    }

    if(begin == end_infix())
    {
        return std::vector<typename associative_container<tkey, tvalue>::key_value_pair>();
    }

    bool found_ones = false;

    if(!search_tree<tkey, tvalue>::_keys_comparer(begin->first, lower_bound))
    {
        if(!lower_bound_inclusive)
        {
            ++begin;
        }
        else
        {
            found_ones = true;
        }
    }

    auto [stk2, pair3] = find_path(upper_bound);

    btree_node* node_end = *(stk2.top().first);

    size_t index_end = pair3.first;

    infix_iterator end(stk2, index_end);

    if (!exist(node_end, index_end))
    {
        auto [inner_ind, found] = find_index(lower_bound, node_end);
        if (inner_ind > node_end->_virtual_size - 1)
        {
            inner_ind = node_end->_virtual_size - 1;
        }

        end.update_index(inner_ind);
    }

    while (begin != end && search_tree<tkey, tvalue>::_keys_comparer(end->first, upper_bound))
    {
        --end;
    }

    if (!search_tree<tkey, tvalue>::_keys_comparer(upper_bound, end->first))
    {
        if (!upper_bound_inclusive)
        {
            if (begin != end)
                --end;
        } else
        {
            found_ones = true;
        }
    }

    if (begin != end)
        found_ones = true;

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> result;

    while(begin != end)
    {
        result.emplace_back(begin->first, begin->second);
        ++begin;
    }
    result.emplace_back(begin->first, begin->second);

    return result;

}


template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::update(const tkey &key, tvalue const& value)
{
    auto path = find_path(key);

    if(path.second.second == false )
    {
        throw std::logic_error("Attempt to update not existing key inside the tree ");
    }
    else
    {
        pair_node_tree pair(key, std::move(value));
        insert_inside(std::move(pair), path.first);
    }
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::update(const tkey &key, tvalue &&value)
{
    auto path = find_path(key);

    if(path.second.second == false )
    {
        throw std::logic_error("Attempt to update not existing key inside the tree ");
    }
    else
    {
        pair_node_tree pair(key, std::move(value));
        insert_inside(std::move(pair), path.first);
    }
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    size_t t,
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger) : search_tree<tkey, tvalue>(keys_comparer, logger, allocator), _t(t), _root(nullptr), _size(0)
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
    b_tree<tkey, tvalue> const &other) : search_tree<tkey, tvalue>(other), _size(other._size), _t(other._t)

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

    }
    return *this;
}

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    b_tree<tkey, tvalue> &&other) noexcept : search_tree<tkey, tvalue>(other), _root(other._root), _t(other._t), _size(other._size)
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
typename b_tree<tkey, tvalue>::btree_node* b_tree<tkey, tvalue>::copy_subtree(b_tree::btree_node *parent, b_tree::btree_node *copirka)
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
    if(_root == nullptr)
    {
        return infix_const_iterator();
    }

    std::stack<std::pair<btree_node**, size_t>> stk;

    stk.push(std::make_pair(&(_root), 0));

    while((*(stk.top().first))->_virtual_size > 0 && (*(stk.top().first))->_subtrees[0] != nullptr)
    {
        stk.push(std::make_pair(&((*(stk.top().first))->_subtrees[0]), 0));
    }

    return infix_const_iterator(stk, 0);
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::cend_infix() const noexcept
{
    return infix_const_iterator();
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
        return std::make_pair(begin , false);//>
    }
    else
    {
        return std::make_pair(begin + 1, false);//<
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

//    check_tree(_root, 0);

    return parrot;
}


template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::check_tree(btree_node *node, size_t depth)
{
	if (node == nullptr)
		return;
	if (!is_terma(node))
	{
		check_tree(node->_subtrees[0], depth + 1);
	}
    std::string tab;
    tab.reserve(depth);
	for (size_t j = 0; j < depth; ++j)
	{
		tab += '\t';
	}
	std::cout << tab << std::to_string(node->_pairs_of_node[0].first) << std::endl;
	for(size_t i = 1; i < node->_virtual_size; ++i)
	{
		if (!search_tree<tkey, tvalue>::_keys_comparer((node->_pairs_of_node[i - 1]).first, (node->_pairs_of_node[i]).first))
			throw std::logic_error("B");
		if (!is_terma(node))
		{
			check_tree(node->_subtrees[i], depth + 1);
		}
		std::cout << tab << std::to_string(node->_pairs_of_node[i].first) << std::endl;
	}

	if (!is_terma(node))
	{
		check_tree(node->_subtrees[node->_virtual_size], depth + 1);
	}

	if (depth == 0)
		std::cout << std::endl << std:: endl;
}

template<typename tkey, typename tvalue>
bool b_tree<tkey, tvalue>::exist(b_tree::btree_node *node, size_t index) noexcept
{
    return node != nullptr && index < (node->_virtual_size + 1);
}




#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H