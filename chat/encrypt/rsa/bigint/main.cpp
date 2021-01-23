#include "bigint.hpp"

const std::string rsa_n =
    "96165402670130584772537629772934250633792434584735938169004540197211175700"
    "03248808113992652836857529658675570356835067184715201230519907361653795328"
    "462699";
const std::string rsa_e = "65537";
const std::string rsa_d =
    "48020339163872217484261813509148210724346418270901449753861827402748568533"
    "18276518446521844642275539818092186650425384826827514552122318308590929813"
    "048801";

const bool is_digit(const char value) noexcept { return std::isdigit(value); }
const bool is_numeric(const std::string& value) noexcept {
    return std::all_of(value.begin(), value.end(), is_digit);
}

std::string to_numeric(std::string next_line) noexcept {
    std::string nlad("");
    for (const char c : next_line) {
        const int d = ((int)c & 0xFF);
        nlad += (d / 100) % 10 + '0';
        nlad += (d / 10) % 10 + '0';
        nlad += d % 10 + '0';
    }
    return nlad;
}

std::string to_chars(reverse::Bigint to_numeric) noexcept {
    to_numeric.changeToBase10();
    std::string nlas("");
    auto d_to_go = to_numeric.num_digits() % 3;
    if (d_to_go == 0) d_to_go = 3;
    auto index = 0;
    auto next_d = 0;
    const int size = (to_numeric.num_digits() + 2) / 3;
    for (int n = 0; n < size; n++) {
        do {
            next_d = next_d * 10 + to_numeric[index++];
        } while (--d_to_go);
        nlas += (char)next_d;
        next_d = 0;
        d_to_go = 3;
    }
    return nlas;
}

std::string async_encrypt(const int tp, const std::string& subline) {
    switch (tp) {
        case -1:
            return reverse::Bigint(to_numeric(subline)).encrypt(rsa_n, rsa_e).to_string() + "\n";
        default:
            return reverse::Bigint(to_numeric(subline)).encrypt(rsa_n, rsa_e).to_string() + "\n" + reverse::Bigint(tp).encrypt(rsa_n, rsa_e).to_string() + "\n";
    }
}

const reverse::Bigint Bbig("100000");
const unsigned int rsa_n_size = rsa_n.size();
const int rsa_n_size3 = rsa_n.size() / 3;

std::string async_decrypt(const int tp, const std::string& st) {
    const reverse::Bigint pst = reverse::Bigint(st).decrypt(rsa_n, rsa_d);

    switch (tp) {
        case 1:
            return to_chars(pst.to_string());
        default:
            if (pst > Bbig) {
                return to_chars(pst.to_string()) + "\n";
            }
            return "\n";
    }
}

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);

    if (argc != 4 && (argc != 5 || *argv[1] != '^') &&
        (argc != 2 || (*argv[1] != 'e' && *argv[1] != 'd'))) {
        std::cout
            << "Run as bigint -op number1 number2, or bigint -help, "
               "where:\n	op is one of + - * / %% gcd encrypt decrypt\n	"
               "number1 and number2 are positive integers of arbitrary length\n"
            << '\n';
        return 0;
    }
    std::string num1, num2, num3;
    const std::string op = std::string(argv[1]);
    if (op[0] != 'd' && op[0] != 'e') {
        num1 = std::string(argv[2]);
        if (!is_numeric(num1)) {
            std::cout << "Error: " << num1 << " is not an unsigned integer.\n"
                      << '\n';
            return 1;
        }
        num2 = std::string(argv[3]);
        if (!is_numeric(num2)) {
            std::cout << "Error: " << num2 << " is not an unsigned integer.\n"
                      << '\n';
            return 1;
        }
        if (argc == 5) {
            num3 = std::string(argv[4]);
            if (!is_numeric(num3)) {
                std::cout << "Error: " << num3 << " is not an unsigned integer.\n"
                          << '\n';
                return 1;
            }
        } else if (op[0] == '^') {
            std::cout << "Error: Modular exponentiation requires an exponent and a "
                         "modulus!\n"
                      << '\n';
            return 1;
        }
    }

    int counter = 3;
    std::vector<std::future<std::string>> fut_encrypt;
    std::vector<std::future<std::string>> fut_decrypt;

    switch (op[0]) {
        case '+':
            std::cout << (reverse::Bigint(num1) + reverse::Bigint(num2)).to_string()
                      << '\n';
            return 0;
        case '-':
            std::cout << (reverse::Bigint(num1) - reverse::Bigint(num2)).to_string()
                      << '\n';
            return 0;
        case '*':
            std::cout << (reverse::Bigint(num1) * reverse::Bigint(num2)).to_string()
                      << '\n';
            return 0;
        case '/':
            std::cout << (reverse::Bigint(num1) / reverse::Bigint(num2)).to_string()
                      << '\n';
            return 0;
        case '%':
            std::cout << (reverse::Bigint(num1) % reverse::Bigint(num2)).to_string()
                      << '\n';
            return 0;
        case '<':
            std::cout << (reverse::Bigint(num1) < reverse::Bigint(num2)) << '\n';
            return 0;
        case '>':
            std::cout << (reverse::Bigint(num1) > reverse::Bigint(num2)) << '\n';
            return 0;
        case '=':
            std::cout << (reverse::Bigint(num1) == reverse::Bigint(num2)) << '\n';
            return 0;
        case '^':
            std::cout << reverse::Bigint(num1).expmod(num2, num3).to_string() << '\n';
            return 0;
        case 'g':
            std::cout
                << (reverse::Bigint(num1).gcd(reverse::Bigint(num2))).to_string()
                << '\n';
            return 0;
        case 'e':
            while (!std::cin.eof()) {
                std::string next_line;
                std::getline(std::cin, next_line);

                if (next_line.size() * 3 > rsa_n_size) {
                    // 2 lines
                    const std::string sub1 = next_line.substr(0, rsa_n_size3);
                    const std::string sub2 = next_line.substr(rsa_n_size3, rsa_n_size3);

                    fut_encrypt.emplace_back(std::async(std::launch::async, async_encrypt, -1, sub1));

                    fut_encrypt.emplace_back(std::async(std::launch::async, async_encrypt, -1, sub2));

                } else {
                    // 1 line
                    fut_encrypt.emplace_back(std::async(std::launch::async, async_encrypt, ++counter, next_line));
                }
            }
            for (auto& fut : fut_encrypt) {
                std::cout << fut.get();
            }

            // remove vector
            std::vector<std::future<std::string>>().swap(fut_encrypt);

            return 0;
        case 'd':
            while (!std::cin.eof()) {
                std::string part1, part2;
                std::getline(std::cin, part1);
                std::getline(std::cin, part2);

                fut_decrypt.emplace_back(std::async(std::launch::async, async_decrypt, 1, part1));

                fut_decrypt.emplace_back(std::async(std::launch::async, async_decrypt, 2, part2));
            }

            for (auto& fut : fut_decrypt) {
                std::cout << fut.get();
            }

            return 0;
        default:
            std::cout << "Error: " << op << " is not a supported operator\n"
                      << '\n';
            return 1;
    }
}
