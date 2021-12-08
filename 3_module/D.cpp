#include <vector>
#include <cmath>
#include <ostream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#define MAX_CHUNK_SIZE 8
#define FIRST_SIEVE_LIMIT 11

class Bitset {
public:
  enum class ConstructorParameters : uint8_t {
    EMPTY = 0,
    FULL = 255
  };

public:
  Bitset() = delete;
  explicit Bitset(const uint64_t size, const ConstructorParameters& parameter = ConstructorParameters::EMPTY)
      : data_(std::vector<uint8_t>(std::ceil(static_cast<float>(size) / MAX_CHUNK_SIZE), {static_cast<uint8_t>(parameter)})),
        size_(size) {
  }

  void add(const uint64_t index) noexcept {
    data_[index / MAX_CHUNK_SIZE] |= (1 << (index % MAX_CHUNK_SIZE));
  }

  void reset(const uint64_t index) noexcept {
    data_[index / MAX_CHUNK_SIZE] &= ~(1 << (index % MAX_CHUNK_SIZE));
  }

  [[nodiscard]] auto exist(const uint64_t index) const noexcept -> bool {
    return (data_[index / MAX_CHUNK_SIZE] & (1 << index % MAX_CHUNK_SIZE)) != 0;
  }

  [[nodiscard]] auto size() const noexcept -> uint64_t {
    return size_;
  }

  [[nodiscard]] auto getChunk(const uint64_t chunkIndex) const noexcept -> uint8_t {
    return data_[chunkIndex];
  }


private:
  std::vector<uint8_t> data_;
  uint64_t size_;
};


class BloomFilter {
public:
  BloomFilter() = delete;

  explicit BloomFilter(const uint64_t size, const double probability)
      : bitset_(std::round(-(size * std::log2(probability)) / std::log(2))),
        hashSize_(std::round(-std::log2(probability))) {
    primeNumbers_.reserve(size);
    fillPrimeNumbers();
  }

  void add(const uint64_t key) noexcept {
    for (uint64_t i = 0; i < hashSize_; ++i) {
      bitset_.add(hash(i, key));
    }
  }

  [[nodiscard]] auto search(const uint64_t key) const noexcept -> bool {
    for (uint64_t i = 0; i < hashSize_; ++i) {
      if (!bitset_.exist(hash(i, key))) {
        return false;
      }
    }
    return true;
  }

  [[nodiscard]] auto getSize() const noexcept -> uint64_t {
    return bitset_.size();
  }

  [[nodiscard]] auto getHashSize() const noexcept -> uint64_t {
    return hashSize_;
  }

  friend void print(std::ostream&, const BloomFilter& filter) noexcept;


private:
  Bitset bitset_;
  uint64_t hashSize_;
  std::vector<uint64_t> primeNumbers_;
  const uint64_t M = std::pow(2, 31) - 1;

private:
  void fillPrimeNumbers() noexcept {
    uint64_t size = hashSize_ <= 5 ?
                    FIRST_SIEVE_LIMIT :
                    hashSize_ * std::log(hashSize_) + hashSize_ * std::log(std::log(hashSize_));
    Bitset bitset(size, Bitset::ConstructorParameters::FULL);
    for (int p = 2; p < bitset.size() + 1; p++) {
      if (bitset.exist(p)) {
        if (primeNumbers_.size() == hashSize_) {
          break;
        }
        primeNumbers_.push_back(p);
        for (int j = p * p; j < bitset.size() + 1; j += p)
          bitset.reset(j);
      }
    }
  }

  [[nodiscard]] auto hash(const uint64_t i, const uint64_t key) const noexcept -> uint64_t  {
    return ((key % M) * (i + 1) % M + primeNumbers_[i] % M) % M % bitset_.size();
  }

};

void print(std::ostream& out, const BloomFilter& filter) noexcept {
  uint8_t bufferChunk;
  uint8_t maxValueChunk;
  const auto remainder = filter.bitset_.size() % MAX_CHUNK_SIZE;
  const auto numberChunks = std::ceil(static_cast<float>(filter.bitset_.size()) / MAX_CHUNK_SIZE);
  for (uint64_t i = 0; i < numberChunks; ++i) {
    maxValueChunk = UINT8_MAX;
    if (i == numberChunks - 1 && remainder != 0) {
      if (remainder == 1) {
        maxValueChunk = 1;
      } else {
        maxValueChunk = std::pow(2, remainder) - 1;
      }
    }
    bufferChunk = filter.bitset_.getChunk(i);
    while (maxValueChunk > 0) {
      if ((bufferChunk & 1) == true) {
        out << 1;
      } else {
        out << 0;
      }
      bufferChunk >>= 1;
      maxValueChunk >>= 1;
    }
  }
}

namespace Parsing {
  auto isIntegerNumber(const std::string &command) noexcept -> bool {
    return std::all_of(command.begin(), command.end(),
                       [](const char symbol) { return isdigit(symbol) > 0; });
  }

  auto isFloatNumber(const std::string &command) noexcept -> bool {
    bool isFindPoint = false;
    for (const auto &symbol : command) {
      if (!std::isdigit(symbol)) {
        if (symbol != '.' || isFindPoint == true) {
          return false;
        } else if (symbol == '.') {
          isFindPoint = true;
        }
      }
    }
    return true;
  }

  auto getCommand(const std::string &line) -> std::string {
    return std::string(line.begin(), std::find(line.begin(), line.end(), ' '));
  }

  auto getValue(const std::string &line) -> uint64_t {
    return std::stoull(std::string(
        std::next(std::find(line.begin(), line.end(), ' ')), line.end()));
  }

  auto getSetValid(const std::string &line) -> std::pair<uint64_t, double> {
    if (std::count(line.begin(), line.end(), ' ') != 2) {
      throw std::logic_error("error");
    } else {
      auto iter1 = std::find(line.begin(), line.end(), ' ');
      auto iter2 = std::find(std::next(iter1), line.end(), ' ');
      std::string command(line.begin(), iter1);
      std::string n(std::next(iter1), iter2);
      std::string P(std::next(iter2), line.end());
      if (!(command == "set" && isIntegerNumber(n) && isFloatNumber(P))) {
        throw std::logic_error("error");
      } else {
        if (std::stoull(n) == 0 || std::stod(P) > 1 ||
            std::round(-std::log2(std::stod(P))) == 0) {
          throw std::logic_error("error");
        }
        return std::make_pair(std::stoull(n), std::stod(P));
      }
    }
  }

  auto isValidCommand(const std::string &line) noexcept -> bool {
    size_t spaceCounter = std::count(line.begin(), line.end(), ' ');
    if (spaceCounter > 1) {
      return false;
    } else {
      auto spaceIter = std::find(line.begin(), line.end(), ' ');
      std::string command(line.begin(), spaceIter);
      if (command == "print") {
        return true;
      } else if (command == "search" || command == "add") {
        if (isIntegerNumber(std::string(std::next(spaceIter), line.end()))) {
          return true;
        } else {
          return false;
        }
      } else {
        return false;
      }
    }
  }
}


int main() {
  std::string command;
  std::ios::sync_with_stdio(false);
  BloomFilter* iterFilter = nullptr;
  while (std::getline(std::cin, command, '\n')) {
    if (command.empty()) {
      continue;
    } else {
      try {
        auto [n, P] = Parsing::getSetValid(command);
        iterFilter = new BloomFilter(n, P);
        std::cout << iterFilter->getSize() << " " << iterFilter->getHashSize() << std::endl;
        break;
      } catch (...) {
        std::cout << "error\n";
      }
    }
  }

  while (std::getline(std::cin, command, '\n')) {
    if (command.empty()) {
      continue;
    } else {
      if (iterFilter == nullptr) {
        break;
      }
      else if (Parsing::isValidCommand(command)) {
        auto commandId = Parsing::getCommand(command);
        if (commandId == "print") {
          print(std::cout, *iterFilter);
          std::cout << std::endl;
        } else if (commandId == "add") {
          iterFilter->add(Parsing::getValue(command));
        } else if (commandId == "search") {
          iterFilter->search(Parsing::getValue(command)) ? std::cout << 1 << std::endl : std::cout << 0 << std::endl;
        }
      } else {
        std::cout << "error\n";
      }
    }
  }

  if (iterFilter != nullptr) {
    delete iterFilter;
  }
  return EXIT_SUCCESS;
}