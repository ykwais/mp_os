#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <mutex>
#include <sstream>

class allocator_sorted_list final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:
    
    void *_trusted_memory;

    static constexpr const size_t _meta_allocator = sizeof(logger*) + sizeof(allocator*) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(size_t) + sizeof(std::mutex) + sizeof(void*);

    static constexpr const size_t _meta_block = sizeof(size_t) + sizeof(void*);

public:
    
    ~allocator_sorted_list() override;
    
    allocator_sorted_list(
        allocator_sorted_list const &other) = delete;
    
    allocator_sorted_list &operator=(
        allocator_sorted_list const &other) = delete;
    
    allocator_sorted_list(
        allocator_sorted_list &&other) noexcept;
    
    allocator_sorted_list &operator=(
        allocator_sorted_list &&other) noexcept;

public:
    
    explicit allocator_sorted_list(
        size_t space_size,
        allocator *parent_allocator = nullptr,
        logger *logger = nullptr,
        allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

public:
    
    [[nodiscard]] void *allocate(
        size_t value_size,
        size_t values_count) override;
    
    void deallocate(
        void *at) override;

public:
    
    inline void set_fit_mode(
        allocator_with_fit_mode::fit_mode mode) override;

public:
    
    std::vector<allocator_test_utils::block_info> get_blocks_info(size_t& full_size_avail) const noexcept override;

private:

    [[nodiscard]] inline allocator *get_allocator() const override;

    [[nodiscard]] inline logger *get_logger() const override;

    [[nodiscard]] inline std::string get_typename() const noexcept override;

private:

    inline std::mutex& get_mutex() noexcept;

    [[nodiscard]] inline allocator_with_fit_mode::fit_mode& get_fit_mode() const noexcept;

    [[nodiscard]] void* get_first_suitable(size_t need_size)  const noexcept; //на предыдущий блок

    [[nodiscard]] void* get_worst_suitable(size_t need_size) const noexcept; //на предыдущий блок

    [[nodiscard]] void* get_best_suitable(size_t need_size)  const noexcept; //на предыдущий блок

    [[nodiscard]] inline size_t get_size_full() const noexcept;//возвращает размер памяти для всего аллокатора

    static inline size_t get_size_block(void* current_block) noexcept;//размер памяти текущего занятого

    inline void** get_parrent_for_current_load_block(void* current_block) const noexcept;//проверка на принадлежность блока данному аллокатору

    static inline void** get_first_block(void* ptr) noexcept;

    static inline void* get_ptr_from_block(void* current) noexcept;

    inline void** get_ptr_previous(void* ptr) const noexcept;

    inline void* get_previous_for_loaded(void* loaded_ptr) const noexcept;

    inline size_t get_next_free_size(void* loaded_block) const noexcept;

    static std::string get_info_in_string(const std::vector<allocator_test_utils::block_info>& vec) noexcept;

    static std::string get_dump(char* at, size_t size);

    class iterator_free_block
    {
        void* _ptr_free;

    public:

        iterator_free_block();

        iterator_free_block(void* ptr);

        bool operator==(const iterator_free_block& oth) const noexcept;

        bool operator!=(const iterator_free_block& oth) const noexcept;

        iterator_free_block& operator++() noexcept;

        iterator_free_block operator++(int) noexcept;

        size_t size();

        void* get_ptr_free_block() const noexcept;

    };

    friend class iterator_free_block;

    [[nodiscard]] iterator_free_block begin_for_free_iter() const noexcept;

    [[nodiscard]] iterator_free_block end_for_free_iter() const noexcept;
    
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H