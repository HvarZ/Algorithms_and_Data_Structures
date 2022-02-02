#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <ostream>
#include <stdexcept>

#define MARCIN_31 2147483647
#define SIEVE_START 12

using namespace std;

using bitset_t = vector<bool>;

class bloom_filter final {
private:
  bitset_t bitset;
  uint64_t hashes;
  vector<uint64_t> primes;

public:
  bloom_filter(const uint64_t n, const double P)
    : bitset(round(-(n * log2(P)) / log(2)),false),
      hashes(round(-log2(P))) {
    set_primes();
  }

  pair<uint64_t, uint64_t> construct_data() const {
    return make_pair(bitset.size(), hashes);
  }

  void add(const uint64_t key) {
    for (uint64_t i = 0; i < hashes; ++i) {
      bitset[hash(i, key)] = true;
    }
  }

  bool search(const uint64_t key) const {
    for (uint64_t i = 0; i < hashes; ++i) {
      if (!search_bitset(hash(i, key))) {
        return false;
      }
    }
    return true;
  }

  void print(ostream& output) const {
    for_each(bitset.begin(), bitset.end(), [&output](const bool bit) {
      bit ? output << 1 : output << 0;
    });
  }

private:
  uint64_t sieve_limit() const {
    return hashes * log(hashes) + hashes * log(log(hashes));
  }

  void set_primes() {
    uint64_t size;
    if (hashes <= 5) {
      size = SIEVE_START;
    } else {
      size = sieve_limit();
    }
    bitset_t bitset_(size, true);
    for (uint64_t i = 2; i * i <= size - 1; ++i) {
      if (bitset_.at(i)) {
        for (uint64_t j = i * i; j <= size - 1; j += i) {
          bitset_[j] = false;
        }
      }
    }
    for (auto iter = bitset_.begin() + 2; iter != bitset_.end(); ++iter) {
      if (primes.size() == hashes) {
        break;
      }
      auto index = distance(bitset_.begin(), iter);
      if (bitset_.at(index)) {
        primes.push_back(index);
      }
    }
  }

  bool search_bitset(const uint64_t key) const {
    return bitset.at(key);
  }

  uint64_t hash(const uint64_t index, const uint64_t key) const {
    return ((key % MARCIN_31) * (index + 1) % MARCIN_31 +
            primes[index] % MARCIN_31) % MARCIN_31 % bitset.size();
  }
};

bool correct(const string &command) {
  return all_of(command.begin(), command.end(),
                [](const char symbol) {
                  return isdigit(symbol);
                });
}

int main() {
  string command;
  string rubbish;
  ios::sync_with_stdio(false);
  uint64_t n = 0;
  double P = 0;
  while (cin >> command) {
    if (command.empty()) {
      continue;
    } else {
      if (command == "set") {
        cin >> n >> P;
        if (n != 0 && P > 0.0 && P < 1.0 &&
            static_cast<uint64_t>(round(-1. * log2(P))) != 0) {
          break;
        } else {
          cout << "error" << endl;
        }
      } else {
        if (command != "print") {
          cin >> rubbish;
        }
        cout << "error" << endl;
      }
    }
  }

  if (n != 0 && P > 0.0 && P < 1.0) {
    string number;
    bloom_filter filter(n, P);
    auto [size, hashes] = filter.construct_data();
    cout << size << " " << hashes << endl;
    while (cin >> command) {
      if (command.empty()) {
        continue;
      } else if (command == "print") {
        filter.print(cout);
        cout << endl;
      } else {
        cin >> number;
        if (command == "add" && correct(number)) {
          filter.add(stoull(number));
        } else if (command == "search" && correct(number)) {
          cout << filter.search(stoull(number)) << "\n";
        } else if (command == "set") {
          cin >> number;
          cout << "error\n";
        } else {
          cout << "error\n";
        }
      }
    }
  }
  return 0;
}