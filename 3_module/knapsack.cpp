#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>
#include <regex>

template <typename T>
class Knapsack final {
public:
  using treasure = std::pair<T, T>;
private:
  using resultPack = std::tuple<T, T, std::vector<bool>>;
  using weightTrace = std::pair<T, std::vector<bool>>;
  std::vector<treasure> treasures;
  size_t maxWeight;

public:

  explicit Knapsack(std::vector<treasure> gems_, const T maxWeight_)
      : treasures(std::move(gems_)), maxWeight(maxWeight_) {
    if ((long long)maxWeight_ != maxWeight_) {
      throw std::runtime_error("Type is not integer");
    }
  }

  resultPack getBackpack() const {
    int sumWeight = 0;
    int sumCosts = 0;
    for (const auto &[weight, cost] : treasures) {
      sumWeight += weight;
      sumCosts += cost;
    }

    std::vector<weightTrace> matrix(
        sumCosts + 1, {maxWeight + 1, std::vector<bool>(treasures.size() + 1)});
    matrix.front() = {0, std::vector<bool>(treasures.size() + 1)};

    size_t currentTreasure = 1;
    for (const auto &[weight, cost] : treasures) {
      for (T i = sumCosts; i >= cost; --i) {
        if (matrix[i - cost].first < matrix[i].first - weight) {
          matrix[i] = matrix[i - cost];
          matrix[i].first += weight;
          matrix[i].second[currentTreasure] = true;
        }
      }
      ++currentTreasure;
    }

    if (sumWeight < maxWeight) {
      std::vector<bool> resultTrace(treasures.size() + 1);
      resultTrace.flip();
      resultTrace[0] = false;
      return std::make_tuple(sumWeight, sumCosts, resultTrace);
    }

    for (T i = sumCosts; i > 0; --i) {
      if (matrix[i].first <= maxWeight) {
        return std::make_tuple(matrix[i].first, i, matrix[i].second);
      }
    }

    return std::make_tuple(0, 0, std::vector<bool>());
  }
};

void printBitset(std::ostream &out, const std::vector<bool> &bitset) noexcept {
  for (size_t i = 0; i < bitset.size(); ++i) {
    if (bitset.at(i)) {
      out << i << std::endl;
    }
  }
}


int main() {
  std::ios::sync_with_stdio(false);
  std::string buffer;
  getline(std::cin, buffer, '\n');
  std::regex weightRegex("^[0-9][0-9]*$");
  std::regex treasureRegex("^[0-9][0-9]*\\s[0-9][0-9]*$");
  while (!std::regex_match(buffer, weightRegex) || buffer.empty()) {
    if (buffer.empty()) {
      getline(std::cin, buffer, '\n');
      continue;
    }
    std::cout << "error\n";
    getline(std::cin, buffer, '\n');
  }
  auto max_weight = std::stoi(buffer);
  std::vector<Knapsack<int>::treasure> input;

  while (getline(std::cin, buffer, '\n')) {
    if (buffer.empty()) {
      continue;
    }
    if (!std::regex_match(buffer, treasureRegex)) {
      std::cout << "error\n";
      continue;
    }

    auto iterSpace = std::find(buffer.begin(), buffer.end(), ' ');

    input.emplace_back(std::stoi(std::string(buffer.begin(), iterSpace)),
                     std::stoi(std::string(iterSpace + 1, buffer.end())));
  }

  Knapsack<int> knapsack(input, max_weight);
  auto [weight, cost, trace] = knapsack.getBackpack();
  std::cout << weight << " " << cost << std::endl;
  printBitset(std::cout, trace);
  return 0;
}
