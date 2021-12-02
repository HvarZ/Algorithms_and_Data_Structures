#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <cmath>
#include <cstdlib>

namespace backpack {
  using resultType = std::tuple<size_t, size_t, size_t>;
  using weightIndexes = std::pair<size_t, size_t>;
  using weightCost = std::pair<size_t, size_t>;

  auto getBackpack(const std::vector<weightCost> &gems,
                   const size_t maxWeight) -> resultType {
    auto sumCosts = std::accumulate(
        gems.begin(), gems.end(), 0,
        [](size_t sum, weightCost pair) { return sum + pair.second; });

    const auto defaultConstructor = std::make_pair(maxWeight + 1, 0);
    std::vector<weightIndexes> dynamicData(sumCosts, defaultConstructor);
    dynamicData.front().first = 0;

    size_t currentIndex = 1;
    size_t sumWeight = 0;
    for (const auto &[weight, cost] : gems) {
      for (size_t i = sumCosts - 1; i >= cost; --i) {
        if (dynamicData[i - cost].first < dynamicData[i].first - weight) {
          dynamicData[i] = dynamicData[i - cost];
          dynamicData[i].first += weight;
          dynamicData[i].second |= (1 << currentIndex);
        }
      }
      sumWeight += weight;
      ++currentIndex;
    }

    if (sumWeight < maxWeight) {
      return std::make_tuple(sumWeight, sumCosts,
                 static_cast<size_t>(std::pow(2, gems.size()) - 1) << 1);
    }

    for (auto iter = dynamicData.rbegin(); iter != dynamicData.rend(); iter++) {
      if (iter->first <= maxWeight) {
        return std::make_tuple(iter->first,
                               std::distance(iter, dynamicData.rend()) - 1,
                               iter->second);
      }
    }

    throw std::logic_error("invalid input");
  }

  void printBitset(std::ostream &out, size_t bitset) noexcept {
    size_t counter = 0;
    while (bitset > 0) {
      if ((bitset & 1) == 1) {
        out << counter << std::endl;
      }
      ++counter;
      bitset >>= 1;
    }
  }
}

auto isIntegerNumber(const std::string& command) noexcept -> bool {
  return std::all_of(command.begin(), command.end(),
              [](const char symbol) { return isdigit(symbol) > 0; });
}

auto getCommand(const std::string& command) -> std::pair<size_t, size_t> {
  const auto ptrSpace = std::find(command.begin(), command.end(), ' ');
  std::string first(command.begin(), ptrSpace);

  if (ptrSpace == command.end()) {
    throw std::logic_error("error");
  }
  std::string second(std::next(ptrSpace), command.end());

  if (!isIntegerNumber(first) || !isIntegerNumber(second)) {
    throw std::logic_error("error");
  }

  return std::make_pair(std::stoul(first), std::stoul(second));
}



int main() {
  std::string currentWeight, currentCost;
  std::string buffer;
  std::getline(std::cin, buffer, '\n');

  while (!isIntegerNumber(buffer) || buffer.empty()) {
    if (buffer.empty()) {
      std::getline(std::cin, buffer, '\n');
      continue;
    }
    std::cout << "error\n";
    std::getline(std::cin, buffer, '\n');
  }
  auto maxWeight = std::stoul(buffer);
  std::vector<backpack::weightCost> input;

  while (std::getline(std::cin, buffer, '\n')) {
    if (buffer.empty()) {
      continue;
    }
    try {
      auto weightCost = getCommand(buffer);
      input.emplace_back(std::move(weightCost));
    } catch (...) {
      std::cout << "error\n";
    }
  }

  auto backpack = backpack::getBackpack(input, maxWeight);
  std::cout << std::get<0>(backpack) << " " << std::get<1>(backpack) << std::endl;

  backpack::printBitset(std::cout, std::get<2>(backpack));

  return EXIT_SUCCESS;
}

