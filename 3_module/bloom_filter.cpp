#include <algorithm>
#include <cmath>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>

class BloomFilter final {
private:
  std::vector<bool> bitset;
  std::vector<int> primeNumbers;
  static const int M_31 = 2'147'483'647;
  size_t numberHash;

public:
  explicit BloomFilter(const size_t size_, const float P)
      : bitset(std::round(-(size_ * std::log2(P) / std::log(2))), false),
        numberHash(std::round(-std::log2(P))) {
    if (numberHash == 0) {
      throw std::runtime_error("error");
    }

    uint64_t size = numberHash <= 5
                        ? 11
                        : numberHash * std::log(numberHash) +
                              numberHash * std::log(std::log(numberHash));
    std::vector<bool> bufferBitset(size, true);
    for (int p = 2; p < bufferBitset.size(); ++p) {
      if (bufferBitset.at(p)) {
        if (primeNumbers.size() == numberHash) {
          break;
        }
        primeNumbers.push_back(p);
        for (int j = p * p; j < bufferBitset.size() + 1; j += p)
          bufferBitset[j] = false;
      }
    }
    primeNumbers.reserve(numberHash);
  }

  bool search(const uint64_t key) const {
    for (int i = 0; i < numberHash; ++i) {
      if (!bitset[hash(key, i)])
        return false;
    }
    return true;
  }

  void add(const uint64_t key) {
    for (int i = 0; i < numberHash; ++i) {
      bitset[hash(key, i)] = true;
    }
  }

  size_t hash() const { return numberHash; }

  size_t size() const { return bitset.size(); }

private:
  int hash(const uint64_t key, const int i) const {
    return int(((key % M_31) * (i + 1) % M_31 + primeNumbers[i] % M_31) % M_31 %
               bitset.size());
  }

  friend void print(const BloomFilter &bloomFilter, std::ostream &output);
};

void print(const BloomFilter &bloomFilter, std::ostream &output) {
  for (const auto i : bloomFilter.bitset) {
    output << i;
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::string buffer;
  size_t n = 0;
  float P = 0.;
  BloomFilter *filter = nullptr;
  while (std::cin >> buffer) {
    if (buffer == "print")
      std::cout << "error" << std::endl;
    else {
      std::cin >> n;
      if (buffer == "set") {
        std::cin >> P;
        if (n > 0 && P > 0.0 && P < 1.0 && std::round(-std::log2(P)) != 0) {
          try {
            filter = new BloomFilter(n, P);
            break;
          } catch (std::runtime_error& exception) {
            std::cout << "error" << std::endl;
          }
        } else if (buffer.empty()) {
        } else {
          std::cout << "error" << std::endl;
        }
      } else {
        std::cout << "error" << std::endl;
      }
    }
  }

  if (n > 0 && P > 0.0 && P < 1.0 && filter != nullptr) {
    std::cout << filter->size() << " " << filter->hash() << std::endl;
    uint64_t key = 0;
    while (std::cin >> buffer) {
      if (buffer == "add") {
        std::cin >> key;
        filter->add(key);
      } else if (buffer == "search") {
        std::cin >> key;
        std::cout << filter->search(key) << std::endl;
      } else if (buffer == "print") {
        print(*filter, std::cout);
        std::cout << std::endl;
      } else if (buffer.empty()) {
        continue;
      } else if (buffer == "set") {
        std::cin >> n >> P;
        std::cout << "error" << std::endl;
      } else
        std::cout << "error" << std::endl;
    }
  }

  if (filter != nullptr) {
    delete filter;
  }
  return 0;
}