#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <mutex>

class allocator_boundary_tags final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:
    
    void *_trusted_memory;

    static constexpr const size_t _size_allocator_meta = sizeof(logger*) + sizeof(allocator*) + sizeof(allocator_with_fit_mode::fit_mode)  +  sizeof(size_t) +  sizeof(std::mutex)  + sizeof(void*);

    static constexpr const size_t _size_load_block_meta = sizeof(size_t) + 3 * sizeof(void*);

public:
    
    ~allocator_boundary_tags() override;
    
    allocator_boundary_tags(
        allocator_boundary_tags const &other) = delete;
    
    allocator_boundary_tags &operator=(
        allocator_boundary_tags const &other) = delete;
    
    allocator_boundary_tags(
        allocator_boundary_tags &&other) noexcept;
    
    allocator_boundary_tags &operator=(
        allocator_boundary_tags &&other) noexcept;

public:
    
    explicit allocator_boundary_tags(
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
    
    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:

    inline allocator *get_allocator() const override;
    
    inline logger *get_logger() const override;
    
    inline std::string get_typename() const noexcept override;

private:

    inline std::mutex& get_mutex() noexcept;

    inline allocator_with_fit_mode::fit_mode& get_fit_mode() const noexcept;

    void* get_first_suitable(size_t need_size)  const noexcept; //на предыдущий блок

    void* get_worst_suitable(size_t need_size) const noexcept; //на предыдущий блок

    void* get_best_suitable(size_t need_size)  const noexcept; //на предыдущий блок

    inline size_t get_size_full() const noexcept;//возвращает размер памяти для всего аллокатора

    inline size_t get_size_current_load_block(void* current_block) const noexcept;//размер памяти текущего занятого

    inline void** get_next_load_block(void* current_block) const noexcept;//найти следующий загруженный

    inline void** get_prev_load_block(void* current_block) const noexcept;//возвращает предыдущий загруженный

    inline void** get_parrent_for_current_load_block(void* current_block) const noexcept;//проверка на принадлежность блока данному аллокатору

    inline void** get_first_block() const noexcept;//указатель на первый блок занятый

    inline size_t get_next_free_size(void* loaded_block) const noexcept;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H