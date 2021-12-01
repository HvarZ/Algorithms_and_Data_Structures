#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace backpack {
  using resultType = std::tuple<size_t, size_t, size_t>;
  using weightIndexes = std::pair<size_t, size_t>;
  using weightCost = std::pair<size_t, size_t>;

  auto getBackpack(const std::vector<weightCost> &gems,
                   const size_t maxWeight) -> resultType {
    size_t sumCosts = std::accumulate(
        gems.begin(), gems.end(), 0,
        [](size_t sum, weightCost pair) { return sum + pair.second; });

    const auto defaultConstructor = std::make_pair(maxWeight + 1, 0);
    std::vector<weightIndexes> dynamicData(sumCosts, defaultConstructor);
    dynamicData.front().first = 0;

    size_t currentIndex = 1;
    for (const auto &[weight, cost] : gems) {
      for (size_t i = sumCosts - 1; i >= weight; --i) {
        if (dynamicData[i - cost].first < dynamicData[i].first - weight) {
          dynamicData[i] = dynamicData[i - cost];
          dynamicData[i].first += weight;
          dynamicData[i].second |= (1 << currentIndex);
        }
      }
      ++currentIndex;
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

int main() {
  size_t maxWeight, currentWeight, currentCost;
  std::cin >> maxWeight;

  std::vector<backpack::weightCost> output;

  while(std::cin >> currentWeight >> currentCost) {
    output.emplace_back(std::make_pair(currentWeight, currentCost));
  }

  auto backpack = backpack::getBackpack(output, maxWeight);
  std::cerr << std::get<0>(backpack) << " " << std::get<1>(backpack) << std::endl;

  backpack::printBitset(std::cerr, std::get<2>(backpack));


  return EXIT_SUCCESS;
}