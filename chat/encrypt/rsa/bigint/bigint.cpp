#include "bigint.hpp"
const unsigned int BASE = 1000000000;

reverse::Bigint::Bigint() noexcept : base(BASE), isEmpty(false) {}

reverse::Bigint::Bigint(const reverse::Bigint &b) noexcept
    : base(BASE), digits(b.digits), isEmpty(false) {}

reverse::Bigint::Bigint(const std::string &s) noexcept
    : base(BASE), isEmpty(false) {
    int ind = s.size() - 9;
    while (ind >= 0) {
        digits.emplace_back((unsigned int)std::stoi(s.substr(ind, 9), nullptr));
        ind -= 9;
    }
    if (ind > -9) {
        digits.emplace_back((unsigned int)std::stoi(s.substr(0, 9 + ind), nullptr));
    }
}

reverse::Bigint::Bigint(const unsigned int num_digits) noexcept
    : base(BASE), digits(num_digits), isEmpty(false) {}

reverse::Bigint::Bigint(const int count, const bool isempty) noexcept
    : Bigint(std::to_string(count)) {
    // handle empty line cases
    isEmpty = isempty;
}

reverse::Bigint::Bigint(const std::vector<int_fast32_t> vec) noexcept
    : base(BASE), digits(vec), isEmpty(false) {}

const reverse::Bigint BZero("0");
const reverse::Bigint BOne("1");
const reverse::Bigint BTwo("2");
const reverse::Bigint BBase(std::to_string(BASE));

std::vector<int_fast32_t> reverse::Bigint::as_vec() const noexcept {
    return digits;
}

std::string reverse::Bigint::to_string() const noexcept {
    if (digits.empty()) return "0";
    std::string s;
    auto vit = digits.rbegin();
    s += std::to_string(*vit);
    while (++vit != digits.rend()) {
        std::string sub("000000000" + std::to_string(*vit));
        s += sub.substr(sub.size() - 9);
    }
    return s;
}

int_fast32_t &reverse::Bigint::operator[](unsigned int idx) noexcept {
    return digits[idx];
}

int_fast32_t reverse::Bigint::operator[](unsigned int idx) const noexcept {
    return digits[idx];
}

bool reverse::Bigint::operator<(const reverse::Bigint &b) const noexcept {
    if (digits.size() != b.digits.size()) return digits.size() < b.digits.size();
    return std::lexicographical_compare(digits.rbegin(), digits.rend(),
                                        b.digits.rbegin(), b.digits.rend());
}

bool reverse::Bigint::operator>(const reverse::Bigint &b) const noexcept {
    return b < *this;
}

bool reverse::Bigint::operator==(const reverse::Bigint &b) const noexcept {
    return digits == b.digits;
}

bool reverse::Bigint::operator<=(const reverse::Bigint &b) const noexcept {
    return !(*this > b);
}

bool reverse::Bigint::operator>=(const reverse::Bigint &b) const noexcept {
    return !(*this < b);
}

bool reverse::Bigint::operator!=(const reverse::Bigint &b) const noexcept {
    return !(*this == b);
}

reverse::Bigint reverse::Bigint::operator+(
    const reverse::Bigint &b) const noexcept {
    reverse::Bigint sum(
        (unsigned int)(std::max(digits.size(), b.digits.size()) + 1));
    auto carry = false;
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int k = 0;

    auto update_carry_and_sum = [&k, &sum, &carry]() {
        carry = sum[k] / BASE == 1;
        sum[k] %= BASE;
        ++k;
    };

    while (i < digits.size() && j < b.digits.size()) {
        sum[k] = digits[i++] + b[j++] + carry;
        update_carry_and_sum();
    }

    while (i < digits.size()) {
        sum[k] = digits[i++] + carry;
        update_carry_and_sum();
    }

    while (j < b.digits.size()) {
        sum[k] = b[j++] + carry;
        update_carry_and_sum();
    }

    if (carry) {
        sum[k] = 1;
    } else {
        sum.digits.resize(k);
    }
    return sum;
}

reverse::Bigint reverse::Bigint::operator-(
    const reverse::Bigint &b) const noexcept {
    if (*this < b) return BZero;
    reverse::Bigint diff((unsigned int)digits.size());
    auto borrow = false;
    unsigned int j = 0;
    while (j < b.digits.size()) {
        if (digits[j] < b[j] + borrow) {
            diff[j] = BASE + digits[j] - b[j] - borrow;
            borrow = true;
        } else {
            diff[j] = digits[j] - b[j] - borrow;
            borrow = false;
        }
        j++;
    }
    while (j < digits.size()) {
        if (digits[j] < int(borrow)) {
            diff[j] = BASE + digits[j] - borrow;
            borrow = true;
        } else {
            diff[j] = digits[j] - borrow;
            borrow = false;
        }
        j++;
    }
    diff.purge();
    return diff;
}

reverse::Bigint reverse::Bigint::operator*(
    const reverse::Bigint &b) const noexcept {
    reverse::Bigint prod(digits.size() + b.digits.size());
    std::reference_wrapper<const reverse::Bigint> smaller = b;
    std::reference_wrapper<const reverse::Bigint> larger = *this;
    if (*this < b) {
        smaller = *this;
        larger = b;
    }
    unsigned int carry = 0;
    for (unsigned int i = 0; i < smaller.get().digits.size(); ++i) {
        carry = 0;
        for (unsigned int j = 0; j < larger.get().digits.size(); ++j) {
            const unsigned int ij = i + j;
            prod[ij] += (smaller.get()[i] * larger.get()[j]) + carry;
            carry = prod[ij] / BASE;
            prod[ij] %= BASE;
        }
        for (unsigned int k = i + larger.get().digits.size(); carry != 0; ++k) {
            prod[k] += carry;
            carry = prod[k] / BASE;  // b.base;
            prod[k] %= BASE;         // b.base;
        }
    }
    prod.purge();
    return prod;
}

reverse::Bigint reverse::Bigint::operator/(
    const unsigned int b) const noexcept {
    int_fast32_t carry = 0;
    reverse::Bigint res(*this);
    for (int i = (int)digits.size() - 1; i >= 0; --i) {
        carry = digits[i] + carry * BASE;
        res.digits[i] = carry / b;
        carry = carry % b;
    }
    while (res.digits.size() > 1 && res.digits.back() == 0) {
        res.digits.pop_back();
    }
    return res;
}

reverse::Bigint reverse::Bigint::operator/(
    const reverse::Bigint &b) const noexcept {
    if (*this < b) return BZero;
    if (b == *this) return BOne;
    if (b == BZero) {
        std::cout << "Error: Divide by 0\n";
        exit(1);
    }
    if (b < BBase) {
        return (*this / b[0]);
    }
    return reverse::Bigint(
        (normal::Bigint(digits) / normal::Bigint(b.digits)).as_vec());
}

reverse::Bigint reverse::Bigint::operator%(
    const reverse::Bigint &b) const noexcept {
    if (*this < b) return *this;
    if (b == *this) return BZero;
    if (b == BZero) {
        std::cout << "Error: Divide by 0\n";
        exit(1);
    }
    return reverse::Bigint(
        (normal::Bigint(digits) % normal::Bigint(b.digits)).as_vec());
}

reverse::Bigint reverse::Bigint::expmod(
    reverse::Bigint exp, const reverse::Bigint mod) const noexcept {
    if (exp == BZero) return BOne;
    reverse::Bigint res(BOne);
    reverse::Bigint tmp(*this);
    while (BZero < exp) {
        if (exp[0] % 2 == 1) {
            res = res * tmp % mod;
        }
        tmp = tmp * tmp % mod;
        exp = exp / 2;
    }
    return res;
}

reverse::Bigint reverse::Bigint::gcd(const reverse::Bigint &b) const noexcept {
    return (b != BZero) ? b.gcd(*this % b) : *this;
}

reverse::Bigint reverse::Bigint::encrypt(
    const std::string &rsa_n, const std::string &rsa_e) const noexcept {
    if (isEmpty) return *this;
    return expmod(reverse::Bigint(rsa_e), reverse::Bigint(rsa_n)).to_string();
}

reverse::Bigint reverse::Bigint::decrypt(
    const std::string &rsa_n, const std::string &rsa_d) const noexcept {
    if (num_digits() <= 3 || (digits[0] == 0 && digits[1] == 0 && digits[2] == 0))
        return reverse::Bigint();
    return expmod(reverse::Bigint(rsa_d), reverse::Bigint(rsa_n));
}

void reverse::Bigint::purge() noexcept {
    int32_t i = digits.size() - 1;
    while (i >= 0 && digits[i] == 0) {
        --i;
    }
    digits.resize(i + 1);
}

unsigned int reverse::Bigint::num_digits() const noexcept {
    return digits.size();
}

void reverse::Bigint::reverse_bigint() noexcept {
    std::reverse(digits.begin(), digits.end());
}

reverse::Bigint reverse::Bigint::reverse_copy() const noexcept {
    reverse::Bigint n(*this);
    n.reverse_bigint();
    return n;
}

void reverse::Bigint::changeToBase10() noexcept {
    if (base != 10) {
        // template it so base 10
        std::vector<int_fast32_t> ndigits(num_digits() * 9);
        for (const auto c : to_string()) {
            ndigits.emplace_back(c - '0');
        }
        digits.swap(ndigits);
        ndigits.clear();
        ndigits.shrink_to_fit();
        base = 10;
    }
}