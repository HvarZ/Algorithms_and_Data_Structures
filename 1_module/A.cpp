#include <iostream>
#include <algorithm>
#include <string>

#define MAX_LENGTH_NUMBER 10

class SummerNumberInConsole {
private:
    std::string currentString;
    int64_t result_;

private:
    [[nodiscard]] auto IsMinus(const char symbol) const noexcept -> bool {
        return symbol == '-';
    }

    [[nodiscard]] auto IsDigit(const char symbol) const noexcept -> bool {
        return isdigit(symbol) != 0;
    }


    [[nodiscard]] auto SumNumberInString(const std::string& string) const noexcept -> int64_t {
        int64_t result = 0;
        int64_t intermediateResult;
        decltype(string.end()) nearestCropPtr;
        for (auto currentCharPtr = string.begin(); currentCharPtr != string.end(); currentCharPtr++) {
            if ((IsDigit(*currentCharPtr)) || (IsMinus(*currentCharPtr) && IsDigit(*std::next(currentCharPtr)))) {
                std::distance(currentCharPtr, string.end()) > MAX_LENGTH_NUMBER ?
                        nearestCropPtr = std::next(currentCharPtr, MAX_LENGTH_NUMBER):
                        nearestCropPtr = string.end();
                intermediateResult = std::stoll(std::string(currentCharPtr, nearestCropPtr));
                result += intermediateResult;
                currentCharPtr = std::next(currentCharPtr, std::to_string(intermediateResult).size() - 1);
            }
        }
        return result;
    }

public:
    SummerNumberInConsole() : result_(0) {
        std::string buffer;
        while (std::getline(std::cin, buffer, '\n')) {
            currentString = std::move(buffer);
            result_ += SumNumberInString(currentString);
        }
    }

    [[nodiscard]] auto GetResult() const noexcept -> int64_t {
        return result_;
    }
};

int main() {
    SummerNumberInConsole summer;
    std::cout << summer.GetResult() << std::endl;
    return 0;
}