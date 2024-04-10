#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <mutex>
#include <sstream>

namespace __cnst
{
    constexpr size_t next_power_2(size_t number) {
        size_t power = 1;
        size_t k = 0;
        while (power < number) {
            power <<= 1;
            ++k;
        }
        return k;
    }
}


class allocator_buddies_system final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:
    
    void *_trusted_memory;

    struct one_byte_field
    {
        bool is_occup : 1;
        unsigned char size : 7;
    };

    static constexpr const size_t _meta_allocator = sizeof(logger*) + sizeof(allocator*) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(unsigned char) + sizeof(std::mutex);

    static constexpr const size_t _meta_ocuupied_block = sizeof(one_byte_field) + sizeof(void*);

    static constexpr const size_t _meta_free_block = sizeof(one_byte_field);

    static constexpr const size_t _minimal_k = __cnst::next_power_2(_meta_ocuupied_block);


public:
    
    ~allocator_buddies_system() override;
    
    allocator_buddies_system(
        allocator_buddies_system const &other) = delete;
    
    allocator_buddies_system &operator=(
        allocator_buddies_system const &other) = delete;
    
    allocator_buddies_system(
        allocator_buddies_system &&other) noexcept;
    
    allocator_buddies_system &operator=(
        allocator_buddies_system &&other) noexcept;

public:
    
    explicit allocator_buddies_system(
        size_t space_size_power_of_two,
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
    
    inline logger *get_logger() const override;

    inline std::string get_typename() const noexcept override;

    inline allocator *get_allocator() const override;

private:

    static size_t size_by_power_2(size_t power_of_2) noexcept;

    static size_t next_power_2(size_t number) noexcept;

    inline std::mutex& get_mutex() noexcept;

    void* get_first_suitable(size_t need_size)  const noexcept;

    void* get_worst_suitable(size_t need_size) const noexcept;

    void* get_best_suitable(size_t need_size)  const noexcept;

    inline size_t get_size_full() const noexcept;

    inline allocator_with_fit_mode::fit_mode& get_fit_mode() const noexcept;

    static inline size_t get_size_block(void* current_block) noexcept;

    void* get_twin(void* current_block) noexcept;

    static bool is_busy(void* current_block) noexcept;

    class iterator_twin
    {
        void* _ptr_block;

    public:

        iterator_twin();

        iterator_twin(void* ptr);

        bool operator==(const iterator_twin& oth) const noexcept;

        bool operator!=(const iterator_twin& oth) const noexcept;

        iterator_twin& operator++() noexcept;

        iterator_twin operator++(int) noexcept;

        size_t size() const noexcept;

        void* get_ptr_iterator_twin() const noexcept;

        bool is_block_ocuppied() const noexcept;

    };

    friend class iterator_free_block;

    iterator_twin begin_for_iter_twin() const noexcept;

    iterator_twin end_for_iter_twin() const noexcept;

    static std::string get_info_in_string(const std::vector<allocator_test_utils::block_info>& vec) noexcept;

    static std::string get_dump(char* at, size_t size);
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H
