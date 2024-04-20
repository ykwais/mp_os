#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H

#include <list>
#include <stack>
#include <vector>
#include <logger.h>
#include <logger_guardant.h>
#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>
#include <search_tree.h>
#include <memory>

template<
    typename tkey,
    typename tvalue>
class binary_search_tree:
    public search_tree<tkey, tvalue>
{

protected:
    
    struct node
    {
    
    public:
        
        tkey key;
        
        tvalue value;
        
        node *left_subtree;
        
        node *right_subtree;
    
    public:
        
        explicit node(
            tkey const &key,
            tvalue const &value);
        
        explicit node(
            tkey const &key,
            tvalue &&value);

        virtual ~node() = default;
        
    };

public:
    
    // region iterators definition
    
    struct iterator_data
    {
    
    public:
        
        unsigned int depth;
        
        tkey key;
        
        tvalue value;
    
    public:
    
        explicit iterator_data(
            unsigned int depth = 0 ,
            tkey const &key = tkey() ,
            tvalue const &value = tvalue());

    public:

         virtual void update_iterator_data(std::stack<node**>& path)
         {
             if(!path.empty())
             {
                 depth = path.size() - 1;
                 key = (*(path.top()))->key;
                 value = (*(path.top()))->value;
             }
         }

         virtual iterator_data* clone_iterator_data() const
         {
             return new iterator_data(*this);
         }

         virtual ~iterator_data() = default;

        
    };
    
    class prefix_iterator
    {
    protected:


        std::unique_ptr<iterator_data> _it_data;
        std::stack<node**> _path;


        iterator_data* clone_data() const
        {
            return _it_data.get() ? _it_data->clone_iterator_data() : nullptr;
        }
    
    public:
        
        explicit prefix_iterator(const std::stack<node**>& path, iterator_data* it_data = nullptr);

        prefix_iterator(const prefix_iterator& oth);

        prefix_iterator(prefix_iterator&& other) noexcept = default;

        prefix_iterator& operator=(const prefix_iterator& oth);

        prefix_iterator& operator=(prefix_iterator&& oth) noexcept = default;

        virtual ~prefix_iterator() = default;
    
    public:
        
        bool operator==(
            prefix_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_iterator const &other) const noexcept;
        
        prefix_iterator &operator++();
        
        prefix_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class prefix_const_iterator
    {
    protected:

        std::unique_ptr<iterator_data> _it_data;
        std::stack<node**> _path;



        iterator_data* clone_data() const
        {
            return _it_data.get() ? _it_data->clone_iterator_data() : nullptr;
        }
    
    public:
        
        explicit prefix_const_iterator( const std::stack<node**>& path, iterator_data* it_data = nullptr);

        prefix_const_iterator(const prefix_const_iterator& oth);

        prefix_const_iterator(prefix_const_iterator&& oth) noexcept = default;

        prefix_const_iterator& operator=(const prefix_const_iterator& oth);

        prefix_const_iterator& operator=(prefix_const_iterator&& oth) noexcept = default;

        virtual ~prefix_const_iterator() = default;
    
    public:
        
        bool operator==(
            prefix_const_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_const_iterator const &other) const noexcept;
        
        prefix_const_iterator &operator++();
        
        prefix_const_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class prefix_reverse_iterator
    {

        std::unique_ptr<iterator_data> _it_data;
        std::stack<node**> _path;



        iterator_data* clone_data() const
        {
            return _it_data.get() ? _it_data->clone_iterator_data() : nullptr;
        }
    
    public:
        
        explicit prefix_reverse_iterator(const std::stack<node**>& path, iterator_data* it_data = nullptr);

        prefix_reverse_iterator(const prefix_reverse_iterator& oth);

        prefix_reverse_iterator(prefix_reverse_iterator&& oth) noexcept = default;

        prefix_reverse_iterator& operator=(const prefix_reverse_iterator& oth);

        prefix_reverse_iterator& operator=(prefix_reverse_iterator&& oth) noexcept = default;

        virtual ~prefix_reverse_iterator() = default;
    
    public:
        
        bool operator==(
            prefix_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_reverse_iterator const &other) const noexcept;
        
        prefix_reverse_iterator &operator++();
        
        prefix_reverse_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class prefix_const_reverse_iterator
    {

        std::unique_ptr<iterator_data> _it_data;
        std::stack<node**> _path;



        iterator_data* clone_data() const
        {
            return _it_data.get() ? _it_data->clone_iterator_data() : nullptr;
        }
    
    public:
        
        explicit prefix_const_reverse_iterator(const std::stack<node**>& path, iterator_data* it_data = nullptr);

        prefix_const_reverse_iterator(const prefix_const_reverse_iterator& oth);

        prefix_const_reverse_iterator(prefix_const_reverse_iterator&& oth) noexcept = default;

        prefix_const_reverse_iterator& operator=(const prefix_const_reverse_iterator& oth);

        prefix_const_reverse_iterator& operator=(prefix_const_reverse_iterator&& oth) noexcept = default;

        virtual ~prefix_const_reverse_iterator() = default;
    
    public:
        
        bool operator==(
            prefix_const_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_const_reverse_iterator const &other) const noexcept;
        
        prefix_const_reverse_iterator &operator++();
        
        prefix_const_reverse_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class infix_iterator
    {

        std::unique_ptr<iterator_data> _it_data;
        std::stack<node**> _path;



        iterator_data* clone_data() const
        {
            return _it_data.get() ? _it_data->clone_iterator_data() : nullptr;
        }
    
    public:
        
        explicit infix_iterator(const std::stack<node**>& path, iterator_data* it_data = nullptr);

        infix_iterator(const infix_iterator& oth);

        infix_iterator(infix_iterator&& oth) noexcept = default;

        infix_iterator& operator=(const infix_iterator& oth);

        infix_iterator& operator=(infix_iterator&& oth) noexcept = default;

        virtual ~infix_iterator() = default;

        node** get_node()
        {
            return _path.top();
        }

    
    public:
        
        bool operator==(
            infix_iterator const &other) const noexcept;
        
        bool operator!=(
            infix_iterator const &other) const noexcept;
        
        infix_iterator &operator++();
        
        infix_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class infix_const_iterator
    {

        std::unique_ptr<iterator_data> _it_data;
        std::stack<node**> _path;



        iterator_data* clone_data() const
        {
            return _it_data.get() ? _it_data->clone_iterator_data() : nullptr;
        }
    
    public:
        
        explicit infix_const_iterator(const std::stack<node**>& path, iterator_data* it_data = nullptr);

        infix_const_iterator(const infix_const_iterator& oth);

        infix_const_iterator(infix_const_iterator&& oth) noexcept = default;

        infix_const_iterator& operator=(const infix_const_iterator& oth);

        infix_const_iterator& operator=(infix_const_iterator&& oth) noexcept = default;

        virtual ~infix_const_iterator() = default;

    
    public:
        
        bool operator==(
            infix_const_iterator const &other) const noexcept;
        
        bool operator!=(
            infix_const_iterator const &other) const noexcept;
        
        infix_const_iterator &operator++();
        
        infix_const_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class infix_reverse_iterator
    {
        std::unique_ptr<iterator_data> _it_data;
        std::stack<node**> _path;



        iterator_data* clone_data() const
        {
            return _it_data.get() ? _it_data->clone_iterator_data() : nullptr;
        }
    
    public:
        
        explicit infix_reverse_iterator(const std::stack<node**>& path, iterator_data* it_data = nullptr);

        infix_reverse_iterator(const infix_reverse_iterator& oth);

        infix_reverse_iterator(infix_reverse_iterator&& oth) noexcept = default;

        infix_reverse_iterator& operator=(const infix_reverse_iterator& oth);

        infix_reverse_iterator& operator=(infix_reverse_iterator&& oth) noexcept = default;

        virtual ~infix_reverse_iterator() = default;
    
    public:
        
        bool operator==(
            infix_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            infix_reverse_iterator const &other) const noexcept;
        
        infix_reverse_iterator &operator++();
        
        infix_reverse_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class infix_const_reverse_iterator
    {
        std::unique_ptr<iterator_data> _it_data;
        std::stack<node**> _path;


        iterator_data* clone_data() const
        {
            return _it_data.get() ? _it_data->clone_iterator_data() : nullptr;
        }
    
    public:
        
        explicit infix_const_reverse_iterator(const std::stack<node**>& path, iterator_data* it_data = nullptr);

        infix_const_reverse_iterator(const infix_const_reverse_iterator& oth);

        infix_const_reverse_iterator(infix_const_reverse_iterator&& oth) noexcept = default;

        infix_const_reverse_iterator& operator=(const infix_const_reverse_iterator& oth);

        infix_const_reverse_iterator& operator=(infix_const_reverse_iterator&& oth) noexcept = default;

        virtual ~infix_const_reverse_iterator() = default;
    
    public:
        
        bool operator==(
            infix_const_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            infix_const_reverse_iterator const &other) const noexcept;
        
        infix_const_reverse_iterator &operator++();
        
        infix_const_reverse_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class postfix_iterator
    {

        std::unique_ptr<iterator_data> _it_data;
        std::stack<node**> _path;


        iterator_data* clone_data() const
        {
            return _it_data.get() ? _it_data->clone_iterator_data() : nullptr;
        }
    
    public:
        
        explicit postfix_iterator(const std::stack<node**>& path, iterator_data* it_data = nullptr);

        postfix_iterator(const postfix_iterator& oth);

        postfix_iterator(postfix_iterator&& oth) noexcept = default;

        postfix_iterator& operator=(const postfix_iterator& oth);

        postfix_iterator& operator=(postfix_iterator&& oth) noexcept = default;

        virtual ~postfix_iterator() = default;
    
    public:
        
        bool operator==(
            postfix_iterator const &other) const noexcept;
        
        bool operator!=(
            postfix_iterator const &other) const noexcept;
        
        postfix_iterator &operator++();
        
        postfix_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class postfix_const_iterator
    {

        std::unique_ptr<iterator_data> _it_data;
        std::stack<node**> _path;


        iterator_data* clone_data() const
        {
            return _it_data.get() ? _it_data->clone_iterator_data() : nullptr;
        }
    
    public:
        
        explicit postfix_const_iterator(const std::stack<node**>& path, iterator_data* it_data = nullptr);

        postfix_const_iterator(const postfix_const_iterator& oth);

        postfix_const_iterator(postfix_const_iterator&& oth) noexcept = default;

        postfix_const_iterator& operator=(const postfix_const_iterator& oth);

        postfix_const_iterator& operator=(postfix_const_iterator&& oth) noexcept = default;

        virtual ~postfix_const_iterator() = default;
    
    public:
        
        bool operator==(
            postfix_const_iterator const &other) const noexcept;
        
        bool operator!=(
            postfix_const_iterator const &other) const noexcept;
        
        postfix_const_iterator &operator++();
        
        postfix_const_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class postfix_reverse_iterator
    {

        std::unique_ptr<iterator_data> _it_data;
        std::stack<node**> _path;


        iterator_data* clone_data() const
        {
            return _it_data.get() ? _it_data->clone_iterator_data() : nullptr;
        }
    
    public:
        
        explicit postfix_reverse_iterator(const std::stack<node**>& path, iterator_data* it_data = nullptr);

        postfix_reverse_iterator(const postfix_reverse_iterator& oth);

        postfix_reverse_iterator(postfix_reverse_iterator&& oth) noexcept = default;

        postfix_reverse_iterator& operator=(const postfix_reverse_iterator& oth);

        postfix_reverse_iterator& operator=(postfix_reverse_iterator&& oth) noexcept = default;

        virtual ~postfix_reverse_iterator() = default;
    
    public:
        
        bool operator==(
            postfix_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            postfix_reverse_iterator const &other) const noexcept;
        
        postfix_reverse_iterator &operator++();
        
        postfix_reverse_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class postfix_const_reverse_iterator
    {
        std::unique_ptr<iterator_data> _it_data;
        std::stack<node**> _path;


        iterator_data* clone_data() const
        {
            return _it_data.get() ? _it_data->clone_iterator_data() : nullptr;
        }

    public:
        
        explicit postfix_const_reverse_iterator(const std::stack<node**>& path, iterator_data* it_data = nullptr);

        postfix_const_reverse_iterator(const postfix_const_reverse_iterator& oth);

        postfix_const_reverse_iterator(postfix_const_reverse_iterator&& oth) noexcept = default;

        postfix_const_reverse_iterator& operator=(const postfix_const_reverse_iterator& oth);

        postfix_const_reverse_iterator& operator=(postfix_const_reverse_iterator&& oth) noexcept = default;

        virtual ~postfix_const_reverse_iterator() = default;
    
    public:
        
        bool operator==(
            postfix_const_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            postfix_const_reverse_iterator const &other) const noexcept;
        
        postfix_const_reverse_iterator &operator++();
        
        postfix_const_reverse_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };

    friend class prefix_iterator;
    friend class prefix_const_iterator;
    friend class prefix_reverse_iterator;
    friend class prefix_const_reverse_iterator;

    friend class postfix_iterator;
    friend class postfix_const_iterator;
    friend class postfix_reverse_iterator;
    friend class postfix_const_reverse_iterator;

    friend class infix_iterator;
    friend class infix_const_iterator;
    friend class infix_reverse_iterator;
    friend class infix_const_reverse_iterator;
    
    // endregion iterators definition

protected:
    
    // region target operations strategies definition
    
    enum class insertion_of_existent_key_attempt_strategy
    {
        update_value,
        throw_an_exception
    };
    
    enum class disposal_of_nonexistent_key_attempt_strategy
    {
        do_nothing,
        throw_an_exception
    };
    
    // endregion target operations strategies definition
    
    // region target operations associated exception types
    
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
    
    // endregion target operations associated exception types
    
    // region template methods definition
    
    class template_method_basics:
        public logger_guardant
    {
    
    private:
    
        binary_search_tree<tkey, tvalue> *_tree;
        
    public:
    
        explicit template_method_basics(
            binary_search_tree<tkey, tvalue> *tree);
        
    protected:
        
        // TODO: think about it!

    protected:
    
        [[nodiscard]] logger *get_logger() const noexcept final;
        
    };
    
    class insertion_template_method:
        public template_method_basics,
        public allocator_guardant
    {
    
    private:
        
        binary_search_tree<tkey, tvalue> *_tree;
    
    public:
        
        explicit insertion_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
        
    public:
        
        void insert(
            tkey const &key,
            tvalue const &value);
        
        void insert(
            tkey const &key,
            tvalue &&value);
    
        void set_insertion_strategy(
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;
    
    protected:
        
        // TODO: think about it!
    
    private:
        
        [[nodiscard]] allocator *get_allocator() const noexcept final;
        
    };
    
    class obtaining_template_method:
        public template_method_basics
    {
    
    private:
        
        binary_search_tree<tkey, tvalue> *_tree;
    
    public:
        
        explicit obtaining_template_method(
            binary_search_tree<tkey, tvalue> *tree);
    
    public:
        
        tvalue const &obtain(
            tkey const &key);
    
    protected:
        
        // TODO: think about it!
        
    };
    
    class disposal_template_method:
        public template_method_basics,
        public allocator_guardant
    {
    
    private:
        
        binary_search_tree<tkey, tvalue> *_tree;
    
    public:
        
        explicit disposal_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
    public:
        
        tvalue dispose(
            tkey const &key);
        
        void set_disposal_strategy(
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;
    
    protected:
        
        // TODO: think about it!
    
    private:
        
        [[nodiscard]] allocator *get_allocator() const noexcept final;
        
    };
    
    // endregion template methods definition

protected:
    
    mutable node *_root;

    insertion_of_existent_key_attempt_strategy _insert_exist_strategy;

    disposal_of_nonexistent_key_attempt_strategy _dispose_nonexist_strategy;


protected:

    // region unused constrector

    explicit binary_search_tree(
        typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
        typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
        typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
        std::function<int(tkey const &, tkey const &)>,
        allocator *allocator,
        logger *logger);//не нужен

    // endregion unused constrector

public:
    
    explicit binary_search_tree(
        std::function<int(tkey const &, tkey const &)> comparer,
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> const &other);
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> const &other);
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    ~binary_search_tree() override;

public:
    
    void insert(
        tkey const &key,
        tvalue const &value) final;
    
    void insert(
        tkey const &key,
        tvalue &&value) final;
    
    tvalue const &obtain(
        tkey const &key) final;
    
    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) final;
    
    tvalue dispose(
        tkey const &key) final;

    
public:

    // region set_strategy

    void set_insertion_strategy(
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;
    
    void set_removal_strategy(
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

    // endregion set_strategy //

public:
    
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

protected:

    std::stack<node**> find_path_to_node(const tkey& key);

    virtual void insert_inside(const tkey& key, const tvalue& value, std::stack<node**>& stk);

    virtual void insert_inside(const tkey& key, tvalue&& value, std::stack<node**>& stk);

    virtual tvalue const& obtain_inside(std::stack<node**>& stk);

    virtual tvalue dispose_inside(std::stack<node**>& stk);

    virtual void copy_subtree(node** dest, node* src);

    virtual void destroy_subtree(node* src) noexcept;

    
    // region subtree rotations definition
    
    void small_left_rotation( typename binary_search_tree<tkey, tvalue>::node*& subtree_root ) const;
    
    void small_right_rotation( typename binary_search_tree<tkey, tvalue>::node*& subtree_root ) const;
    
    void big_left_rotation( typename binary_search_tree<tkey, tvalue>::node*& subtree_root ) const;
    
    void big_right_rotation( typename binary_search_tree<tkey, tvalue>::node*& subtree_root ) const;
    
    void double_left_rotation( typename binary_search_tree<tkey, tvalue>::node*& subtree_root) const;
    
    void double_right_rotation( typename binary_search_tree<tkey, tvalue>::node *&subtree_root ) const;
    
    // endregion subtree rotations definition

};


// region binary_search_tree<tkey, tvalue>::node methods implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue const &value) : key(key), value(value), left_subtree(nullptr), right_subtree(nullptr){}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue &&value) : key(key), value(std::move(value)), left_subtree(nullptr), right_subtree(nullptr) {}

// endregion binary_search_tree<tkey, tvalue>::node methods implementation

// region iterators implementation

// region iterator data implementation

template<typename tkey,typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(unsigned int depth, tkey const &key, tvalue const &value):
    depth(depth), key(key), value(value) {}

// endregion iterator data implementation

// region prefix_iterator implementation

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator( const std::stack<node**>& path, iterator_data* it_data) : _path(path), _it_data(it_data)
{
    if(!_path.empty())
    {
        _it_data->update_iterator_data(_path);
    }
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(const prefix_iterator& oth) : _path(oth._path), _it_data(oth.clone_data()) {}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator& binary_search_tree<tkey, tvalue>::prefix_iterator::operator=(const binary_search_tree::prefix_iterator &oth)
{
    if(*this != oth)
    {
        _path = oth._path;
        _it_data = std::unique_ptr<iterator_data>(oth.clone_data());
    }

    return *this;
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
    return ((_path.empty() && (other._path).empty()) || (!_path.empty() && !(other._path).empty() && _path.top() == (other._path).top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator++()
{
    if(_path.empty())
    {
        return *this;
    }

    if((*(_path.top()))->left_subtree != nullptr)
    {
        _path.push(&((*(_path.top()))->left_subtree));
    }
    else if((*(_path.top()))->right_subtree != nullptr)
    {
        _path.push(&((*(_path.top()))->right_subtree));
    }
    else//terminal
    {
        node* son = *(_path.top());
        _path.pop();

        while(!_path.empty())
        {
            if((*(_path.top()))->left_subtree == son && (*(_path.top()))->right_subtree == nullptr)
            {
                son = *(_path.top());
                _path.pop();
                continue;
            }

            if((*(_path.top()))->left_subtree == son && (*(_path.top()))->right_subtree != nullptr)
            {
                _path.push(&((*(_path.top()))->right_subtree));
                break;
            }

            if((*(_path.top()))->right_subtree == son)
            {
                son = *(_path.top());
                _path.pop();
                continue;
            }
        }
    }

    _it_data->update_iterator_data(_path);

    return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator const binary_search_tree<tkey, tvalue>::prefix_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_iterator::operator*() const
{
    return _it_data.get();
}

// endregion prefix_iterator implementation

// region prefix_const_iterator implementation

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator( const std::stack<node**>& path, iterator_data* it_data) : _path(path), _it_data(it_data)
{
    if(!_path.empty())
    {
        _it_data->update_iterator_data(_path);
    }
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(const prefix_const_iterator& oth) : _path(oth._path), _it_data(oth.clone_data()) {}


template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator& binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator=(const prefix_const_iterator& oth)
{
    if(*this == oth)
    {
        _path = oth._path;
        _it_data = std::unique_ptr<iterator_data>(oth.clone_data());
    }

    return *this;
}



template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
    return (_path.empty() && other._path.empty() || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++()
{
    if(_path.empty())
    {
        return *this;
    }

    if((*(_path.top()))->left_subtree != nullptr)
    {
        _path.push(&((*(_path.top()))->left_subtree));
    }
    else if((*(_path.top()))->right_subtree != nullptr)
    {
        _path.push(&((*(_path.top()))->right_subtree));
    }
    else//terminal
    {
        node* son = *(_path.top());
        _path.pop();

        while(!_path.empty())
        {
            if((*(_path.top()))->left_subtree == son && (*(_path.top()))->right_subtree == nullptr)
            {
                son = *(_path.top());
                _path.pop();
                continue;
            }

            if((*(_path.top()))->left_subtree == son && (*(_path.top()))->right_subtree != nullptr)
            {
                _path.push(&((*(_path.top()))->right_subtree));
                break;
            }

            if((*(_path.top()))->right_subtree == son)
            {
                son = *(_path.top());
                _path.pop();
                continue;
            }
        }
    }

    _it_data->update_iterator_data(_path);

    return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator*() const
{
    return _it_data.get();
}

// endregion prefix_const_iterator implementation

// region prefix_reverse_iterator implementation

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(
    const std::stack<node**>& path, iterator_data* it_data ) : _path(path), _it_data(it_data)
    {
        if(!_path.empty())
        {
            _it_data->update_iterator_data(_path);
        }
    }

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(const prefix_reverse_iterator& oth) : _path(oth._path), _it_data(oth.clone_data()) {}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator& binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator=(const prefix_reverse_iterator& oth)
{
    if(*this == oth)
    {
        _path = oth._path;
        _it_data = std::unique_ptr<iterator_data>(oth.clone_data());
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
    return (_path.empty() && other._path.empty() || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++()
{
    if(_path.empty())
    {
        return *this;
    }

    if((*(_path.top()))->right_subtree != nullptr)
    {
        _path.push(&((*(_path.top()))->right_subtree));
    }
    else if((*(_path.top()))->left_subtree != nullptr)
    {
        _path.push(&((*(_path.top()))->left_subtree));
    }
    else//terminal
    {
        node* son = *(_path.top());
        _path.pop();

        while(!_path.empty())
        {
            if((*(_path.top()))->right_subtree == son && (*(_path.top()))->left_subtree == nullptr)
            {
                son = *(_path.top());
                _path.pop();
                continue;
            }

            if((*(_path.top()))->right_subtree == son && (*(_path.top()))->left_subtree != nullptr)
            {
                _path.push(&((*(_path.top()))->left_subtree));
                break;
            }

            if((*(_path.top()))->left_subtree == son)
            {
                son = *(_path.top());
                _path.pop();
                continue;
            }
        }
    }

    _it_data->update_iterator_data(_path);

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator*() const
{
    return _it_data.get();
}

// endregion prefix_reverse_iterator implementation

// region prefix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator( const std::stack<node**>& path, iterator_data* it_data) :  _path(path), _it_data(it_data)
{
    if(!_path.empty())
    {
        _it_data->update_iterator_data(_path);
    }
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(const prefix_const_reverse_iterator& oth) : _path(oth._path), _it_data(oth.clone_data()) {}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator& binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator=(const prefix_const_reverse_iterator& oth)
{
    if(*this == oth)
    {
        _path = oth._path;
        _it_data = std::unique_ptr<iterator_data>(oth.clone_data());
    }

    return *this;
}


template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
    return (_path.empty() && other._path.empty() || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++()
{
    if(_path.empty())
    {
        return *this;
    }

    if((*(_path.top()))->right_subtree != nullptr)
    {
        _path.push(&((*(_path.top()))->right_subtree));
    }
    else if((*(_path.top()))->left_subtree != nullptr)
    {
        _path.push(&((*(_path.top()))->left_subtree));
    }
    else//terminal
    {
        node* son = *(_path.top());
        _path.pop();

        while(!_path.empty())
        {
            if((*(_path.top()))->right_subtree == son && (*(_path.top()))->left_subtree == nullptr)
            {
                son = *(_path.top());
                _path.pop();
                continue;
            }

            if((*(_path.top()))->right_subtree == son && (*(_path.top()))->left_subtree != nullptr)
            {
                _path.push(&((*(_path.top()))->left_subtree));
                break;
            }

            if((*(_path.top()))->left_subtree == son)
            {
                son = *(_path.top());
                _path.pop();
                continue;
            }
        }
    }

    _it_data->update_iterator_data(_path);

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator*() const
{
    return _it_data.get();
}

// endregion prefix_const_reverse_iterator implementation

// region infix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(const std::stack<node**>& path, iterator_data* it_data) : _path(path), _it_data(it_data)
{
    if(!_path.empty())
    {
        _it_data->update_iterator_data(_path);
    }
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(const infix_iterator& oth) : _path(oth._path), _it_data(oth.clone_data()) {}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator& binary_search_tree<tkey, tvalue>::infix_iterator::operator=(const infix_iterator& oth)
{
    if(*this == oth)
    {
        _path = oth._path;
        _it_data = std::unique_ptr<iterator_data>(oth.clone_data());
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
    return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator++()
{
    if(_path.empty())
    {
        return *this;
    }

    if((*(_path.top()))->right_subtree != nullptr)
    {
        node** current_node = &((*(_path.top()))->right_subtree);
        while(*current_node != nullptr)
        {
            _path.push(current_node);
            current_node = &((*current_node)->left_subtree);
        }
    }
    else
    {
        node* son = *(_path.top());
        _path.pop();

        if(!_path.empty() && (*(_path.top()))->left_subtree == son) {}
        else
        {
            while(!_path.empty() && (*(_path.top()))->left_subtree != son)
            {
                son = *(_path.top());
                _path.pop();
            }
        }
    }

    _it_data->update_iterator_data(_path);

    return *this;

}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator const binary_search_tree<tkey, tvalue>::infix_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data* binary_search_tree<tkey, tvalue>::infix_iterator::operator*() const
{
    return _it_data.get();
}

// endregion infix_iterator implementation

// region infix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(const std::stack<node**>& path, iterator_data* it_data) : _path(path), _it_data(it_data)
{
    if(!_path.empty())
    {
        _it_data->update_iterator_data(_path);
    }
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(const infix_const_iterator& oth) : _path(oth._path), _it_data(oth.clone_data()) {}////////////////////////////////////////////////TODO

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator& binary_search_tree<tkey, tvalue>::infix_const_iterator::operator=(const infix_const_iterator& oth)
{
    if(*this == oth)
    {
        _path = oth._path;
        _it_data = std::unique_ptr<iterator_data>(oth.clone_data());
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
    return ((_path.empty() && other._path.empty()) || (!_path.empty() && !_path.empty() && _path.top() == other._path.top()));
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
    if(_path.empty())
    {
        return *this;
    }

    if((*(_path.top()))->right_subtree != nullptr)
    {
        node** current_node = &((*(_path.top()))->right_subtree);
        while(*current_node != nullptr)
        {
            _path.push(current_node);
            current_node = &((*current_node)->left_subtree);
        }
    }
    else
    {
        node* son = *(_path.top());
        _path.pop();

        if(!_path.empty() && (*(_path.top()))->left_subtree == son) {}
        else
        {
            while(!_path.empty() && (*(_path.top()))->left_subtree != son)
            {
                son = *(_path.top());
                _path.pop();
            }
        }
    }

    _it_data->update_iterator_data(_path);

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator const binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
    return _it_data.get();
}

// endregion infix_const_iterator implementation

// region infix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(const std::stack<node**>& path, iterator_data* it_data) : _path(path), _it_data(it_data)
{
    if(!_path.empty())
    {
        _it_data->update_iterator_data(_path);
    }
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(const infix_reverse_iterator& oth) : _path(oth._path), _it_data(oth.clone_data()) {}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator& binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator=(const infix_reverse_iterator& oth)
{
    if(*this == oth)
    {
        _path = oth._path;
        _it_data = std::unique_ptr<iterator_data>(oth.clone_data());
    }

    return *this;
}


template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
    return ((_path.empty() && other._path.empty()) || (!_path.empty() && !_path.empty() && _path.top() == other._path.top()));
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++()
{
    if(_path.empty())
    {
        return *this;
    }

    if((*(_path.top()))->left_subtree != nullptr)
    {
        node** current_node = &((*(_path.top()))->left_subtree);
        while(*current_node != nullptr)
        {
            _path.push(current_node);
            current_node = &((*current_node)->right_subtree);
        }
    }
    else
    {
        node* son = *(_path.top());
        _path.pop();

        if(!_path.empty() && (*(_path.top()))->right_subtree == son) {}
        else
        {
            while(!_path.empty() && (*(_path.top()))->right_subtree != son)
            {
                son = *(_path.top());
                _path.pop();
            }
        }
    }

    _it_data->update_iterator_data(_path);

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator*() const
{
    return _it_data.get();
}

// endregion infix_reverse_iterator implementation

// region infix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(const std::stack<node**>& path, iterator_data* it_data) : _path(path), _it_data(it_data)
{
    if(!_path.empty())
    {
        _it_data->update_iterator_data(_path);
    }
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(const infix_const_reverse_iterator& oth) : _path(oth._path), _it_data(oth.clone_data()) {}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator& binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator=(const infix_const_reverse_iterator& oth)
{
    if(*this == oth)
    {
        _path = oth._path;
        _it_data = std::unique_ptr<iterator_data>(oth.clone_data());
    }

    return *this;
}


template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
    return ((_path.empty() && other._path.empty()) || (!_path.empty() && !_path.empty() && _path.top() == other._path.top()));
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++()
{
    if(_path.empty())
    {
        return *this;
    }

    if((*(_path.top()))->left_subtree != nullptr)
    {
        node** current_node = &((*(_path.top()))->left_subtree);
        while(*current_node != nullptr)
        {
            _path.push(current_node);
            current_node = &((*current_node)->right_subtree);
        }
    }
    else
    {
        node* son = *(_path.top());
        _path.pop();

        if(!_path.empty() && (*(_path.top()))->right_subtree == son) {}
        else
        {
            while(!_path.empty() && (*(_path.top()))->right_subtree != son)
            {
                son = *(_path.top());
                _path.pop();
            }
        }
    }

    _it_data->update_iterator_data(_path);

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator*() const
{
    return _it_data.get();
}

// endregion infix_const_reverse_iterator implementation

// region postfix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(const std::stack<node**>& path, iterator_data* it_data) : _path(path), _it_data(it_data)
{
    if(!_path.empty())
    {
        _it_data->update_iterator_data(_path);
    }
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(const postfix_iterator& oth) : _path(oth._path), _it_data(oth.clone_data()) {}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator& binary_search_tree<tkey, tvalue>::postfix_iterator::operator=(const postfix_iterator& oth)
{
    if(*this == oth)
    {
        _path = oth._path;
        _it_data = std::unique_ptr<iterator_data>(oth.clone_data());
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
    return ((_path.empty() && other._path.empty()) || (!_path.empty() && !_path.empty() && _path.top() == other._path.top()));
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator++()
{
    node* son = *(_path.top());
    _path.pop();

    if(!_path.empty())
    {
        if((*(_path.top()))->left_subtree == son)
        {
            if((*(_path.top()))->right_subtree != nullptr)
            {
                _path.push(&((*_path.top())->right_subtree));

                while((*_path.top())->left_subtree != nullptr)
                {
                    _path.push(&((*_path.top())->left_subtree));
                }
            }
        }

        _it_data->update_iterator_data(_path);
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator const binary_search_tree<tkey, tvalue>::postfix_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_iterator::operator*() const
{
    return _it_data.get();
}

// endregion postfix_iterator implementation

// region postfix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(const std::stack<node**>& path, iterator_data* it_data) : _path(path), _it_data(it_data)
{
    if(!_path.empty())
    {
        _it_data->update_iterator_data(_path);
    }
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(const postfix_const_iterator& oth) : _path(oth._path), _it_data(oth.clone_data()) {}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator& binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator=(const postfix_const_iterator& oth)
{
    if(*this == oth)
    {
        _path = oth._path;
        _it_data = std::unique_ptr<iterator_data>(oth.clone_data());
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
    return ((_path.empty() && other._path.empty()) || (!_path.empty() && !_path.empty() && _path.top() == other._path.top()));
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++()
{
    node* son = *(_path.top());
    _path.pop();

    if(!_path.empty())
    {
        if((*(_path.top()))->left_subtree == son)
        {
            if((*(_path.top()))->right_subtree != nullptr)
            {
                _path.push(&((*_path.top())->right_subtree));

                while((*_path.top())->left_subtree != nullptr)
                {
                    _path.push(&((*_path.top())->left_subtree));
                }
            }
        }

        _it_data->update_iterator_data(_path);
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator*() const
{
    return _it_data.get();
}

// endregion postfix_const_iterator implementation

// region postfix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(const std::stack<node**>& path, iterator_data* it_data) : _path(path), _it_data(it_data)
{
    if(!_path.empty())
    {
        _it_data->update_iterator_data(_path);
    }
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(const postfix_reverse_iterator& oth) : _path(oth._path), _it_data(oth.clone_data()) {}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator& binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator=(const postfix_reverse_iterator& oth)
{
    if(*this == oth)
    {
        _path = oth._path;
        _it_data = std::unique_ptr<iterator_data>(oth.clone_data());
    }

    return *this;
}


template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
    return ((_path.empty() && other._path.empty()) || (!_path.empty() && !_path.empty() && _path.top() == other._path.top()));

}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++()
{
    node* son = *(_path.top());
    _path.pop();

    if(!_path.empty())
    {
        if((*(_path.top()))->right_subtree == son)
        {
            if((*(_path.top()))->left_subtree != nullptr)
            {
                _path.push(&((*_path.top())->left_subtree));

                while((*_path.top())->right_subtree != nullptr)
                {
                    _path.push(&((*_path.top())->right_subtree));
                }
            }
        }

        _it_data->update_iterator_data(_path);
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator*() const
{
    return _it_data.get();
}

// endregion postfix_reverse_iterator implementation

// region postfix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(const std::stack<node**>& path, iterator_data* it_data) : _path(path), _it_data(it_data)
{
    if(!_path.empty())
    {
        _it_data->update_iterator_data(_path);
    }
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(const postfix_const_reverse_iterator& oth) : _path(oth._path), _it_data(oth.clone_data()) {}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator& binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator=(const postfix_const_reverse_iterator& oth)
{
    if(*this == oth)
    {
        _path = oth._path;
        _it_data = std::unique_ptr<iterator_data>(oth.clone_data());
    }

    return *this;
}


template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
    return ((_path.empty() && other._path.empty()) || (!_path.empty() && !_path.empty() && _path.top() == other._path.top()));

}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++()
{
    node* son = *(_path.top());
    _path.pop();

    if(!_path.empty())
    {
        if((*(_path.top()))->right_subtree == son)
        {
            if((*(_path.top()))->left_subtree != nullptr)
            {
                _path.push(&((*_path.top())->left_subtree));

                while((*_path.top())->right_subtree != nullptr)
                {
                    _path.push(&((*_path.top())->right_subtree));
                }
            }
        }

        _it_data->update_iterator_data(_path);
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++(
    int not_used)
{
    auto tmp = *this;
    ++(*this);
    return  tmp;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator*() const
{
    return _it_data.get();
}

// endregion postfix_const_reverse_iterator implementation

// endregion iterators implementation

// region target operations associated exception types implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::insertion_of_existent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to insert already existing key inside the tree."), _key(key) {}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to obtain a value by non-existing key from the tree."), _key(key) {}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to dispose a value by non-existing key from the tree."), _key(key) {}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

// endregion target operations associated exception types implementation

// region template methods implementation

// region binary_search_tree<tkey, tvalue>::template_method_basics implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(
    binary_search_tree<tkey, tvalue> *tree)
{
    throw not_implemented("template<typename tkey, typename tvalue>binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(binary_search_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> [[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept", "your code should be here...");
}

// endregion binary_search_tree<tkey, tvalue>::template_method_basics implementation

// region search_tree<tkey, tvalue>::insertion_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    binary_search_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{
    throw not_implemented("template<typename tkey, typename tvalue>binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(binary_search_tree<tkey, tvalue> *, typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue const &value)
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(tkey const &,tvalue const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue &&value)
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(tkey const &, tvalue &&)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::insertion_template_method::set_insertion_strategy(typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::get_allocator() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::get_allocator() const noexcept", "your code should be here...");
}

// endregion search_tree<tkey, tvalue>::insertion_template_method implementation

// region search_tree<tkey, tvalue>::obtaining_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    binary_search_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(binary_search_tree<tkey, tvalue> *tree)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain(
    tkey const &key)
{
    throw not_implemented("template<typename tkey, typename tvalue> tvalue const &binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain(tkey const &)", "your code should be here...");
}

// endregion search_tree<tkey, tvalue>::obtaining_template_method implementation

// region search_tree<tkey, tvalue>::disposal_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    binary_search_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree<tkey, tvalue>::template_method_basics(tree)
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(binary_search_tree<tkey, tvalue> *, typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(
    tkey const &key)
{
    throw not_implemented("template<typename tkey, typename tvalue> tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(tkey const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::disposal_template_method::set_disposal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::disposal_template_method::set_disposal_strategy(typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_strategy)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> [[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept", "your code should be here...");
}

// endregion search_tree<tkey, tvalue>::disposal_template_method implementation

// endregion template methods

// region construction, assignment, destruction implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
    typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
    typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger)
//    :
//    search_tree<tkey, tvalue>(comparer, logger, allocator),
//    _insertion_template(insertion_template),
//    _obtaining_template(obtaining_template),
//    _disposal_template(disposal_template)
{
    throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::binary_search_tree(typename binary_search_tree<tkey, tvalue>::insertion_template_method *, typename binary_search_tree<tkey, tvalue>::obtaining_template_method *, typename binary_search_tree<tkey, tvalue>::disposal_template_method *, std::function<int(tkey const &, tkey const &)>, allocator *, logger *)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        search_tree<tkey, tvalue>(keys_comparer, logger, allocator), _root(nullptr), _dispose_nonexist_strategy(disposal_strategy), _insert_exist_strategy(insertion_strategy) {}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> const &other) : search_tree<tkey, tvalue>(other), _insert_exist_strategy(other._insert_exist_strategy), _dispose_nonexist_strategy(other._dispose_nonexist_strategy)
{
    try {
        copy_subtree(&_root, other._root);
    }
    catch(...)
    {
        destroy_subtree(_root);
        throw;
    }

}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> &&other) noexcept : search_tree<tkey, tvalue>(other), _insert_exist_strategy(other._insert_exist_strategy), _dispose_nonexist_strategy(other._dispose_nonexist_strategy)
{
    _root = std::exchange(other._root, nullptr);
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> const &other)
{
    if(this == &other)
    {
        return *this;
    }

    search_tree<tkey, tvalue>::operator=(other);
    _insert_exist_strategy = other._insert_exist_strategy;
    _dispose_nonexist_strategy = other._dispose_nonexist_strategy;

    try{
        copy_subtree(&_root, other._root);
    }
    catch(...)
    {
        destroy_subtree(_root);
        throw;
    }

    return *this;

}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> &&other) noexcept
{
    if(this == &other)
    {
        return *this;
    }

    search_tree<tkey, tvalue>::operator=(std::move(other));
    _insert_exist_strategy = other._insert_exist_strategy;
    _dispose_nonexist_strategy = other._dispose_nonexist_strategy;

    std::swap(_root, other._root);

    return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::~binary_search_tree()
{
    destroy_subtree(_root);
}

// endregion construction, assignment, destruction implementation

// region associative_container<tkey, tvalue> contract implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue const &value)
{
    std::stack<node**> path = find_path_to_node(key);

    if(*path.top() == nullptr)
    {
        insert_inside(key, value, path);
    }
    else
    {
        if(_insert_exist_strategy == insertion_of_existent_key_attempt_strategy::throw_an_exception)
        {
            throw insertion_of_existent_key_attempt_exception(key);
        }
        else
        {
            (*path.top())->value = value; //у value своё присваивание копированием
        }
    }

}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue &&value)
{
    std::stack<node**> path = find_path_to_node(key);

    if(*path.top() == nullptr)
    {
        insert_inside(key, std::move(value), path);
    }
    else
    {
        if(_insert_exist_strategy == insertion_of_existent_key_attempt_strategy::throw_an_exception)
        {
            throw insertion_of_existent_key_attempt_exception(key);
        }
        else
        {
            (*path.top())->value = std::move(value);
        }
        return;
    }
}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtain(
    tkey const &key)
{
    std::stack<node**> path = find_path_to_node(key);

    if(*path.top() == nullptr)
    {
        throw obtaining_of_nonexistent_key_attempt_exception(key);
    }

    return obtain_inside(path);
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    auto path = find_path_to_node(lower_bound);

    if(*path.top() == nullptr)
    {
        path.pop();
    }

    infix_iterator it(path, new iterator_data);
    infix_iterator end = end_infix();

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> result;

    if(it != end)
    {
        int cmp = search_tree<tkey, tvalue>::_keys_comparer((*it)->key, lower_bound);

        if(cmp < 0)
        {
            ++it;
        }
        else if(cmp == 0)
        {
            if(lower_bound_inclusive)
            {
                result.emplace_back( (*it)->key, (*it)->value );
            }
            ++it;
        }

        if(it != end)
        {
            cmp = search_tree<tkey, tvalue>::_keys_comparer((*it)->key, upper_bound);
        }

        while(it != end && cmp < 0)
        {
            result.emplace_back((*it)->key, (*it)->value);
            ++it;
            cmp = search_tree<tkey, tvalue>::_keys_comparer((*it)->key, upper_bound);
        }


        if(it != end && cmp == 0 && upper_bound_inclusive)
        {
            result.emplace_back((*it)->key, (*it)->value);
        }

    }

    return result;

}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose(
    tkey const &key)
{
    std::stack<node**> path = find_path_to_node(key);

    if(*path.top() == nullptr)
    {
        if(_dispose_nonexist_strategy == disposal_of_nonexistent_key_attempt_strategy::throw_an_exception)
        {
            throw disposal_of_nonexistent_key_attempt_exception(key);
        }
        else
        {
            if constexpr (std::is_default_constructible<tvalue>::value)
            {
                return {};//tvalue()
            }
            else
            {
                throw disposal_of_nonexistent_key_attempt_exception(key);
            }
        }
    }

    return dispose_inside(path);

}

// endregion associative_containers contract implementations

// region help-function for contract

template<typename tkey, typename tvalue>
std::stack<typename binary_search_tree<tkey, tvalue>::node**> binary_search_tree<tkey, tvalue>::find_path_to_node(const tkey& key)
{
    std::stack<node**> result;

    result.push(&(_root));

    node** current_node  = &(_root);

    while(*current_node != nullptr)
    {
        int cmp = search_tree<tkey, tvalue>::_keys_comparer(key, (*current_node)->key);

        if(cmp > 0)
        {
            current_node = &((*current_node)->right_subtree);
        }
        else if(cmp < 0)
        {
            current_node = &((*current_node)->left_subtree);
        }
        else
        {
            break;
        }

        result.push(current_node);
    }

    return result;
}



template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::insert_inside(const tkey& key, tvalue&& value, std::stack<node**>& stk)
{
    (*stk.top()) =  static_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node),1));
    try {
        allocator::construct(*stk.top(), key, std::move(value));
    }
    catch(...)
    {
        allocator_guardant::deallocate_with_guard(*stk.top());
        throw;
    }
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::insert_inside(const tkey& key, const tvalue& value, std::stack<node**>& stk)
{
    (*stk.top()) =  static_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node),1));
    try {
        allocator::construct(*stk.top(), key, value);
    }
    catch(...)
    {
        allocator_guardant::deallocate_with_guard(*stk.top());
        throw;
    }
}

template<typename tkey, typename tvalue>
tvalue const& binary_search_tree<tkey, tvalue>::obtain_inside(std::stack<node**>& stk)
{
    return (*stk.top())->value;
}

template<typename tkey, typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose_inside(std::stack<node**>& stk)
{
    node* current_node = *stk.top();

    if(current_node->left_subtree == nullptr && current_node->right_subtree == nullptr)
    {
        *stk.top() = nullptr;
    }
    else if(current_node->right_subtree == nullptr || current_node->left_subtree == nullptr)
    {
        node* update_node = current_node->right_subtree != nullptr ? (current_node->right_subtree) : (current_node->left_subtree);

        *stk.top() = update_node;
    }
    else
    {
        node** update = &((*stk.top())->left_subtree);

        while((*update)->right_subtree != nullptr)
        {
            update = &((*update)->right_subtree);
        }

        node* previous_node = *stk.top();

        *(stk.top()) = *update;

        *update = (*update)->left_subtree;

        (*stk.top())->left_subtree = previous_node->left_subtree /*;*/== *(stk.top()) ? *update : previous_node->left_subtree;
        (*stk.top())->right_subtree = previous_node->right_subtree;

    }

    tvalue res = current_node->value;
    allocator::destruct(current_node);
    allocator_guardant::deallocate_with_guard(current_node);
    return res;
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::copy_subtree(node** dest, node* src)
{
    if(src == nullptr)
    {
        *dest = nullptr;
    }

    *dest = reinterpret_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node)));

    try{
        allocator::construct(*dest, *src);
    }
    catch(...)
    {
        allocator_guardant::deallocate_with_guard(*dest);
        *dest = nullptr;
        throw;
    }

    (*dest)->right_subtree = nullptr;
    (*dest)->left_subtree = nullptr;

    copy_subtree(&((*dest)->left_subtree), src->left_subtree);
    copy_subtree(&((*dest)->right_subtree), src->right_subtree);
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::destroy_subtree(node* src) noexcept
{
    if(src != nullptr)
    {
        destroy_subtree(src->left_subtree);
        destroy_subtree(src->right_subtree);

        allocator::destruct(src);
        allocator_guardant::deallocate_with_guard(src);
    }
}








// endregion help-function for contract

// region set_strategy

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    _insert_exist_strategy = insertion_strategy;
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_removal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    _dispose_nonexist_strategy = disposal_strategy;
}

// endregion set_strategy

// region iterators requesting implementation

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::begin_prefix()  noexcept
{
    std::stack<node**> stk;
    if(_root != nullptr)
    {
        stk.push(&_root);
    }

    return prefix_iterator(stk, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::end_prefix()  noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
    std::stack<node**> stk;
    if(_root != nullptr)
    {
        stk.push((&_root));///////////////////////////////////////////////
    }

    return prefix_const_iterator(stk, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_prefix()  noexcept
{
    std::stack<node**> stk;
    if(_root != nullptr)
    {
        stk.push(&(_root));
    }

    return prefix_reverse_iterator(stk, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_prefix()  noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_prefix() const noexcept
{
    std::stack<node**> stack;
    if(_root != nullptr)
    {
        stack.push(&(_root));
    }

    return prefix_const_reverse_iterator(stack, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::begin_infix() noexcept
{
    std::stack<node**> stk;
    node** current_node = &_root;
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = &((*current_node)->left_subtree);
    }

    return infix_iterator(stk, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::end_infix()  noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    std::stack<node**> stk;
    node** current_node = (&_root);
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = (&((*current_node)->left_subtree));
    }

    return infix_const_iterator(stk, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_infix()  noexcept
{
    std::stack<node**> stk;
    node** current_node = &_root;
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = &((*current_node)->right_subtree);
    }

    return infix_reverse_iterator(stk, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_infix()  noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_infix() const noexcept
{
    std::stack<node**> stk;
    node** current_node = &_root;
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = &((*current_node)->right_subtree);
    }

    return infix_const_reverse_iterator(stk, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::begin_postfix()  noexcept
{
    std::stack<node**> stk;
    node** current_node = &_root;
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = (*current_node)->left_subtree != nullptr ? &((*current_node)->left_subtree) : &((*current_node)->right_subtree);
    }
    return postfix_iterator(stk, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::end_postfix()  noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_postfix() const noexcept
{
    std::stack<node**> stk;
    node** current_node = (&_root);
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = (*current_node)->left_subtree != nullptr ? (&((*current_node)->left_subtree)) : (&((*current_node)->right_subtree));
    }
    return postfix_const_iterator(stk, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_postfix()  noexcept
{
    std::stack<node**> stk;
    node** current_node = &_root;
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = (*current_node)->right_subtree != nullptr ? &((*current_node)->right_subtree) : &((*current_node)->left_subtree);
    }
    return postfix_reverse_iterator(stk, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_postfix()  noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_postfix() const noexcept
{
    std::stack<node**> stk;
    node** current_node = &_root;
    while(*current_node != nullptr)
    {
        stk.push(current_node);
        current_node = (*current_node)->right_subtree != nullptr ? &((*current_node)->right_subtree) : &((*current_node)->left_subtree);
    }
    return postfix_const_reverse_iterator(stk, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(std::stack<node**>(), nullptr);
}


// endregion iterators request implementation

// region subtree rotations implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_left_rotation( binary_search_tree<tkey, tvalue>::node *&subtree_root ) const//root - относительно него вращение
{
    if(subtree_root != nullptr && subtree_root->right_subtree != nullptr)
    {
        node* ex_top = subtree_root;

        subtree_root = subtree_root->right_subtree;
        ex_top->right_subtree = subtree_root->left_subtree;
        subtree_root->left_subtree = ex_top;

    }
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_right_rotation( binary_search_tree<tkey, tvalue>::node *&subtree_root ) const
{
    if(subtree_root != nullptr  && subtree_root->left_subtree != nullptr)
    {
        node* ex_top = subtree_root;

        subtree_root = subtree_root->left_subtree;
        ex_top->left_subtree = subtree_root->right_subtree;
        subtree_root->right_subtree = ex_top;
    }
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_left_rotation( binary_search_tree<tkey, tvalue>::node *&subtree_root ) const
{
    if(subtree_root != nullptr && subtree_root->right_subtree != nullptr && subtree_root->right_subtree->left_subtree != nullptr)
    {
        small_right_rotation(subtree_root->right_subtree);
        small_left_rotation(subtree_root);
    }
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_right_rotation( binary_search_tree<tkey, tvalue>::node *&subtree_root ) const
{
    if(subtree_root != nullptr && subtree_root->left_subtree != nullptr && subtree_root->left_subtree->right_subtree != nullptr)
    {
        small_left_rotation(subtree_root->left_subtree);
        small_right_rotation(subtree_root);
    }
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_left_rotation( binary_search_tree<tkey, tvalue>::node *&subtree_root ) const
{
    if(subtree_root != nullptr && subtree_root->right_subtree != nullptr && subtree_root->right_subtree->right_subtree != nullptr)
    {
        small_left_rotation(subtree_root);
        small_left_rotation(subtree_root);
    }

}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_right_rotation( binary_search_tree<tkey, tvalue>::node *&subtree_root ) const
{
    if(subtree_root != nullptr && subtree_root->left_subtree != nullptr && subtree_root->left_subtree->left_subtree != nullptr)
    {
        small_right_rotation(subtree_root);
        small_right_rotation(subtree_root);
    }
}

// endregion subtree rotations implementation

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H