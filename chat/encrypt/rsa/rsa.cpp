#include "rsa.hpp"

Rsa::Rsa(const std::string& n, const std::string& e, const std::string& d) : rsa_n(n), rsa_e(e), rsa_d(d) {}

Rsa::Rsa(const Rsa& from) {
    rsa_n = from.rsa_n;
    rsa_e = from.rsa_e;
    rsa_d = from.rsa_d;
}


