#include <vector>
#include <cmath>
#include <ostream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

using u64 = unsigned long long;

class bloom_filter {
public:
    explicit bloom_filter(const u64 size, const double probability)
            : bitset(std::round(-(size * std::log2(probability)) / std::log(2))),
              hashes(std::round(-std::log2(probability))) {
        primes.reserve(size);
        fill_primes();
    }

    void add(const u64 key) {
        for (u64 i = 0; i < hashes; ++i) {
            bitset[hash(i, key)] = true;
        }
    }

    bool search(const u64 key) const {
        for (u64 i = 0; i < hashes; ++i) {
            if (!bitset.at(hash(i, key))) {
                return false;
            }
        }
        return true;
    }

    u64 size() const {
        return bitset.size();
    }

    u64 number_hashes() const {
        return hashes;
    }

    friend void print(std::ostream&, const bloom_filter& filter);


private:
    std::vector<bool> bitset;
    u64 hashes;
    std::vector<u64> primes;
    const u64 M = std::pow(2, 31) - 1;
    const u64 SIEVE_LIMIT = 11;

private:
    void fill_primes() {
        u64 size;
        if (hashes <= 5) {
            size = SIEVE_LIMIT;
        } else {
            size = hashes * std::log(hashes) + hashes * std::log(std::log(hashes));
        }
        std::vector<bool> bitset_(size, true);
        for (int p = 2; p < bitset_.size(); ++p) {
            if (bitset_.at(p)) {
                if (primes.size() == hashes) {
                    break;
                }
                primes.push_back(p);
                for (int j = p * p; j < bitset_.size() + 1; j += p)
                    bitset_[j] = false;
            }
        }
    }

    u64 hash(const u64 i, const u64 key) const {
        return ((key % M) * (i + 1) % M + primes[i] % M) % M % bitset.size();
    }

};

void print(std::ostream& out, const bloom_filter& filter) {
    for (const auto& bit : filter.bitset) {
        if (bit) {
            out << 1;
        } else {
            out << 0;
        }
    }
}


bool is_integer(const std::string &command) {
    return std::all_of(command.begin(), command.end(),
                       [](const char symbol) { return std::isdigit(symbol) > 0;
    });
}


int main() {
    std::ios::sync_with_stdio(false);
    std::string command;
    uint64_t n = 0;
    float P = 0.f;
    while (std::cin >> command) {
        if (command.empty()) {
            continue;
        } else if (command == "set") {
            std::cin >> n >> P;
            if (n != 0 && P > 0.f && P < 1.f &&
                static_cast<uint64_t>(std::round(-1.f * std::log2(P))) != 0) {
                break;
            } else {
                std::cout << "error\n";
            }
        } else {
            std::string number;
            if (command != "print") {
                std::cin >> number;
            }
            std::cout << "error\n";
        }
    }

    if (n != 0 && P != 0.f && P < 1.f) {
        bloom_filter bloom(n, P);
        std::cout << bloom.size() << " " << bloom.number_hashes() << "\n";

        std::string number;
        while (std::cin >> command) {
            if (command.empty()) {
                continue;
            } else if (command == "print") {
                print(std::cout, bloom);
                std::cout << std::endl;
            } else {
                std::cin >> number;
                if (command == "add" && is_integer(number)) {
                    bloom.add(std::stoull(number));
                } else if (command == "search" && is_integer(number)) {
                    std::cout << bloom.search(std::stoull(number)) << "\n";
                } else if (command == "set") {
                    std::cin >> number;
                    std::cout << "error\n";
                } else {
                    std::cout << "error\n";
                }
            }
        }
    }
    return 0;
}