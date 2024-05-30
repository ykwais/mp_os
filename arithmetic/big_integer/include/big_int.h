
#ifndef MP_OS_BIG_INT_H
#define MP_OS_BIG_INT_H

#include <vector>
#include <utility>
#include <iostream>
#include <concepts>

namespace __crutch
{
    constexpr unsigned int get_mask_of_half()
    {
        unsigned int res = 0;

        for(size_t i = 0; i < sizeof(unsigned int) * 4; ++i)
        {
            res |= (1u << i);
        }

        return res;
    }

    constexpr size_t next_power_2(size_t size) noexcept
    {
        int ones_counter = 0, index = -1;

        constexpr const size_t o = 1;

        for (int i = sizeof(size_t) * 8 - 1; i >= 0; --i)
        {
            if (size & (o << i))
            {
                if (ones_counter == 0)
                    index = i;
                ++ones_counter;
            }
        }

        return ones_counter <= 1 ? (1u << index) : (1u << (index + 1));
    }
}

class big_int
{
    bool _sign;
    std::vector<unsigned int> _digits;

    static const constexpr unsigned int mask = __crutch::get_mask_of_half();

    constexpr void optimisation() noexcept;

    constexpr static void plus_assign_without_sign(std::vector<unsigned int>& lhs, const std::vector<unsigned int>& rhs, size_t shift = 0);

    static void minus_assign_without_sign(std::vector<unsigned int>& lhs, const std::vector<unsigned int>& rhs, size_t shift = 0);

    void minus_assign_without_sign_reverse(const std::vector<unsigned int>& lhs, std::vector<unsigned int>& rhs, size_t shift = 0);

    bool need_karatsuba() const noexcept;

    bool need_newton() const noexcept;

    static std::vector<unsigned int> summ_four(unsigned int a0b0, unsigned int a1b0, unsigned int a0b1, unsigned int a1b1);

    void mult_assign_without_sign(std::vector<unsigned int>& lhs, const std::vector<unsigned int>& rhs) const;

    std::vector<unsigned int> mult_common(const std::vector<unsigned int>& lhs, const std::vector<unsigned int>& rhs) const;

    std::vector<unsigned int> mult_karatsuba(const std::vector<unsigned int>& lhs, const std::vector<unsigned int>& rhs) const;

    void divide_assign_without_sign(std::vector<unsigned int>& lhs, const std::vector<unsigned int>& rhs) const;

    std::vector<unsigned int> div_common(const std::vector<unsigned int>& lhs, const std::vector<unsigned int>& rhs) const;

    std::vector<unsigned int> div_newton(const std::vector<unsigned int>& lhs, const std::vector<unsigned int>& rhs) const;

    static void move_caretka(std::vector<unsigned int>& vec, unsigned int new_var);

    unsigned int find_quolitent(const std::vector<unsigned int>& numerator, const std::vector<unsigned int>& denominator) const;

    static std::strong_ordering compare_no_sign(const std::vector<unsigned int>& lhs, const std::vector<unsigned int>& rhs, size_t shift = 0) noexcept;

    std::strong_ordering compare(const big_int& other, size_t shift = 0) const noexcept;

    static std::vector<big_int> newton_transform(const std::vector<big_int>& f);

    static std::vector<big_int> multiply_vectors(const std::vector<big_int>& lhs, const std::vector<big_int>& rhs);

    static std::vector<big_int> multiply_vector(const std::vector<big_int>& lhs, size_t val);

    static std::vector<big_int> plus_vectors(const std::vector<big_int>& lhs, const std::vector<big_int>& rhs);

    static std::vector<big_int> minus_vectors(const std::vector<big_int>& lhs, const std::vector<big_int>& rhs);

    static void modulo_vector(std::vector<big_int>& val, size_t power);

public:

    explicit big_int(const std::vector<unsigned int> &digits, bool sign = true);
    explicit big_int(std::vector<unsigned int> &&digits, bool sign = true);
    explicit big_int(const std::string& num, unsigned int radix = 10);

    big_int(const big_int&) = default;
    big_int(big_int&&) = default;

    big_int& operator=(const big_int&) = default;
    big_int& operator=(big_int&&) = default;

    template<std::integral Num>
    big_int(Num d);

    big_int();

    explicit operator bool();

    big_int& operator++();
    big_int operator++(int);

    big_int& operator--();
    big_int operator--(int);

    big_int& operator+=(const big_int& other);

    big_int& plus_assign(const big_int& other, size_t shift = 0);


    big_int& operator-=(const big_int& other);

    big_int& minus_assign(const big_int& other, size_t shift = 0);

    big_int& operator*=(const big_int& other);

    big_int& operator/=(const big_int& other);

    big_int& operator%=(const big_int& other);

    big_int operator+(const big_int& other) const;
    big_int operator-(const big_int& other) const;
    big_int operator*(const big_int& other) const;
    big_int operator/(const big_int& other) const;
    big_int operator%(const big_int& other) const;

    std::strong_ordering operator<=>(const big_int& other) const noexcept;

    bool operator==(const big_int& other) const noexcept;

    big_int& operator<<=(size_t shift);

    big_int& operator>>=(size_t shift);


    big_int operator<<(size_t shift) const;
    big_int operator>>(size_t shift) const;

    big_int operator~() const;

    big_int& operator&=(const big_int& other);

    big_int& operator|=(const big_int& other);

    big_int& operator^=(const big_int& other);


    big_int operator&(const big_int& other) const;
    big_int operator|(const big_int& other) const;
    big_int operator^(const big_int& other) const;

    friend std::ostream &operator<<(std::ostream &stream, big_int const &value) noexcept;

    friend std::istream &operator>>(std::istream &stream, big_int &value);

    std::string to_string() const;
};

big_int operator""_bi(unsigned long long n);

#endif //MP_OS_BIG_INT_H
