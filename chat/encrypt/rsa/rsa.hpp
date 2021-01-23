#pragma once 

#include <iostream>

class Rsa {
    public:
        std::string rsa_n;
        std::string rsa_e;

        Rsa(const std::string& n, const std::string& e, const std::string& d);
        Rsa(const Rsa&);
        ~Rsa();
        
        std::string encrypt_string(const std::string&);
        std::string decrypt_string(const std::string&);
        
    private:
        std::string rsa_d;
};