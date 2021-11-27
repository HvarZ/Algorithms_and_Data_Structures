#include <iostream>
#include <algorithm>
#include <vector>

enum class Step {
  INC,
  DEC,
  DBL
};

auto getNumberUnitsBinary(size_t number) noexcept -> size_t {
  size_t counter = 0;
  while (number > 0) {
    counter += number & 1;
    number >>= 1;
  }

  return counter;
}

auto getNumberSteps(size_t number) noexcept -> std::vector<Step> {
  std::vector<Step> steps;
  while (number > 1) {
    if (number % 2 == 0) {
      number >>= 1;
      steps.push_back(Step::DBL);
    } else {
      if (number == 3) {
        steps.push_back(Step::INC);
        steps.push_back(Step::DBL);
        steps.push_back(Step::INC);
        return steps;
      }
      auto unitsLeftNeighbour = getNumberUnitsBinary(number - 1);
      auto unitsRightNeighbour = getNumberUnitsBinary(number + 1);
      if (unitsLeftNeighbour >= unitsRightNeighbour) {
        steps.push_back(Step::DEC);
        ++number;
      } else {
        steps.push_back(Step::INC);
        --number;
      }
    }
  }

  steps.push_back(Step::INC);

  return steps;
}

int main() {
  size_t number;
  std::cin >> number;

  auto result = getNumberSteps(number);

  for (auto iter = result.rbegin(); iter != result.rend(); ++iter) {
    if (*iter == Step::INC) {
      std::cout << "inc\n";
    } else if (*iter == Step::DEC) {
      std::cout << "dec\n";
    } else {
      std::cout << "dbl\n";
    }
  }

  return EXIT_SUCCESS;
}