#pragma once
#pragma GCC optimize("finite-math-only,no-trapping-math,rename-registers")

#include <algorithm>
#include <future>
#include <iostream>
#include <string>
#include <vector>

namespace reverse {
class Bigint {
public:
    Bigint() noexcept;
    Bigint(const Bigint&) noexcept;
    Bigint(const unsigned int num_digits) noexcept;
    Bigint(const std::string&) noexcept;
    Bigint(const int, const bool) noexcept;
    Bigint(const std::vector<int_fast32_t>) noexcept;

    std::string to_string() const noexcept;
    Bigint operator+(const Bigint&) const noexcept;
    Bigint operator-(const Bigint&) const noexcept;
    Bigint operator*(const Bigint&) const noexcept;
    Bigint operator/(const unsigned int) const noexcept;

    Bigint operator/(const Bigint&) const noexcept;
    Bigint operator%(const Bigint&) const noexcept;

    Bigint expmod(Bigint, const Bigint) const noexcept;
    Bigint gcd(const Bigint&) const noexcept;
    Bigint encrypt(const std::string&, const std::string&) const noexcept;
    Bigint decrypt(const std::string&, const std::string&) const noexcept;

    bool operator<(const Bigint&) const noexcept;
    bool operator==(const Bigint&) const noexcept;
    bool operator>(const Bigint&) const noexcept;
    bool operator!=(const Bigint&) const noexcept;
    bool operator<=(const Bigint&) const noexcept;
    bool operator>=(const Bigint&) const noexcept;

    int_fast32_t& operator[](unsigned int idx) noexcept;
    int_fast32_t operator[](unsigned int idx) const noexcept;
    unsigned int num_digits() const noexcept;
    std::vector<int_fast32_t> as_vec() const noexcept;

    void reverse_bigint() noexcept;
    Bigint reverse_copy() const noexcept;
    void changeToBase10() noexcept;

private:
    unsigned int base;
    std::vector<int_fast32_t> digits;
    void purge() noexcept;
    bool isEmpty;
};
}  // namespace reverse

namespace normal {
class Bigint {
public:
    Bigint() noexcept;
    Bigint(const std::vector<int_fast32_t>) noexcept;
    Bigint(const Bigint&) noexcept;
    Bigint(const unsigned int num_digits) noexcept;
    Bigint(const std::string&) noexcept;

    std::string to_string() const noexcept;
    Bigint operator-(const Bigint&) const noexcept;
    Bigint operator*(const unsigned int) const noexcept;
    std::pair<Bigint, Bigint> mult(const unsigned int) const noexcept;

    Bigint operator/(const Bigint&) const noexcept;
    Bigint operator%(const Bigint&) const noexcept;
    bool operator<(const Bigint&) const noexcept;
    bool operator>(const Bigint&) const noexcept;
    bool operator==(const Bigint&) const noexcept;
    bool operator!=(const Bigint&) const noexcept;
    int_fast32_t& operator[](unsigned int idx) noexcept;
    int_fast32_t operator[](unsigned int idx) const noexcept;
    std::vector<int_fast32_t> as_vec() const noexcept;
    unsigned int bin_search(Bigint&) const noexcept;
    Bigint binSearch(const Bigint&) const noexcept;

private:
    std::vector<int_fast32_t> digits;
};

}  // namespace normal
