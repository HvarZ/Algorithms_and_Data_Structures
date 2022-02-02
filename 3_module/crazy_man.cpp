#include <algorithm>
#include <iostream>
#include <stack>

inline int units(int number) {
  size_t result = 0;
  while (number > 0) {
    if (number & 1) {
      break;
    }
    number >>= 1;
    ++result;
  }
  return result;
}

std::stack<std::string> steps(int number) {
  std::stack<std::string> steps;
  while (number != 1) {
    if (number % 2 == 0) {
      auto numberUnits = units(number);
      for (int i = 0; i < numberUnits; ++i) {
        number >>= 1;
        steps.push("dbl");
      }

    } else {
      auto incNumber = units(number + 1);
      auto decNumber = units(number - 1);
      if (decNumber > incNumber || number == 3) {
        --number;
        steps.push("inc");
      } else if (decNumber < incNumber) {
        ++number;
        steps.push("dec");
      }
    }
  }
  steps.push("inc");
  return steps;
}

int main() {
  std::ios::sync_with_stdio(false);
  int number;
  std::cin >> number;
  auto result = steps(number);
  auto sizeStack = result.size();
  for (int i = 0; i < sizeStack; ++i) {
    std::cout << result.top() << std::endl;
    result.pop();
  }
  return 0;
}