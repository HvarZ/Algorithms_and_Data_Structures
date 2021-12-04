#include <vector>
#include <cmath>
#include <ostream>
#include <iostream>

#define MAX_CHUNK_SIZE 8

auto isPrime(const uint64_t number) -> bool {
  for (uint64_t i = 2; i <= std::sqrt(number); ++i) {
    if (number % i == 0) {
      return false;
    }
  }

  return true;
}

class Bitset {
public:
  Bitset() = delete;
  explicit Bitset(const uint64_t size)
      : data_(std::vector<uint8_t>(std::ceil(static_cast<float>(size) / MAX_CHUNK_SIZE), {0})),
        size_(size) {}

  void add(const uint64_t index) noexcept {
    data_[index / MAX_CHUNK_SIZE] |= (1 << (index % MAX_CHUNK_SIZE));
  }

  [[nodiscard]] auto exist(const uint64_t index) const noexcept -> bool {
    return (data_[index / MAX_CHUNK_SIZE] & (1 << index % MAX_CHUNK_SIZE)) == 1;
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
        hashSize_(std::round(-log2(probability))) {
    primeNumbers_.reserve(size);
    fillPrimeNumbers();
  }

  void add(const uint64_t key) noexcept {
    for (uint64_t i = 0; i < hashSize_; ++i) {
      bitset_.add(hash(i, key));
    }
  }

  [[nodiscard]] auto search(const uint64_t key) const noexcept-> bool {
    for (uint64_t i = 0; i < hashSize_; ++i) {
      if (!bitset_.exist(hash(i, key))) {
        return false;
      }
    }
    return true;
  }

  friend void print(std::ostream&, const BloomFilter& filter) noexcept;


private:
  Bitset bitset_;
  uint64_t hashSize_;
  std::vector<uint64_t> primeNumbers_;
  const uint64_t M = std::pow(2, 31) - 1;

private:
  void fillPrimeNumbers() noexcept {
    uint64_t counter = 0;
    uint64_t number = 2;
    while (counter != hashSize_) {
      if (isPrime(number) == true) {
        primeNumbers_.push_back(number);
        ++counter;
      }
      ++number;
    }
  }

  [[nodiscard]] auto hash(const uint64_t i, const uint64_t key) const noexcept -> uint64_t  {
    return ((key % M) * (i + 1) % M + primeNumbers_[i] % M) % M % bitset_.size();
  }

};

void print(std::ostream& out, const BloomFilter& filter) noexcept {
  uint8_t bufferChunk;
  uint8_t maxValueChunk;
  auto numberChunks = std::ceil(static_cast<float>(filter.bitset_.size()) / 8);
  for (uint64_t i = 0; i < numberChunks; ++i) {
    maxValueChunk = UINT8_MAX;
    if (i == numberChunks - 1) {
      maxValueChunk = std::pow(filter.bitset_.size() % UINT8_MAX, 2) - 1;
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
  out << std::endl;
}

auto isIntegerNumber(const std::string& command) noexcept -> bool {
  return std::all_of(command.begin(), command.end(),
                     [](const char symbol) { return isdigit(symbol) > 0; });
}

auto isFloatNumber(const std::string& command) noexcept -> bool {
  bool isFindPoint = false;
  for (const auto& symbol : command) {
    if (!std::isdigit(symbol)) {
      if (symbol != '.' || isFindPoint == true) {
        return false;
      } else if (symbol == '.') {
        isFindPoint = true;
      }
    } else {
      return false;
    }
  }
  return true;
}

auto isSetValid(const std::string& line) noexcept -> bool {
  if (std::count(line.begin(), line.end(), ' ') != 2) {
    return false;
  } else {
    auto iter1 = std::find(line.begin(), line.end(), ' ');
    auto iter2 = std::find(std::next(iter1), line.end(), ' ');
    std::string command(line.begin(), iter1);
    std::string n(std::next(iter1), iter2);
    std::string P(std::next(iter2), line.end());
    if (!(command == "set" && isIntegerNumber(n) && isFloatNumber(P))) {
      return false;
    }
  }
  return true;
}



int main() {
  std::string command;
  while (std::getline(std::cin, command, '\n')) {
    if (command.empty()) {
      continue;
    } else if (isSetValid(command)) {
      break;
    } else {
      std::cout << "error\n";
    }
  }

  return EXIT_SUCCESS;
}