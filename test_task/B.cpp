#include <iostream>
#include <vector>
#include <string_view>
#include <algorithm>

#define ADD_DELETE  result += std::stoll(std::string(subString.begin(), std::next(currentChar)));       \
                    subString.remove_prefix(std::distance(subString.begin(), std::next(currentChar)));  \
                    isDeleting = true;

class SummerNumberInConsole {
private:
    std::vector<std::string> strings_;
    long long result_;

    [[nodiscard]] auto IsMinus(const char symbol) const noexcept -> bool {
        return symbol == '-';
    }

    [[nodiscard]] auto IsDigit(const char symbol) const noexcept -> bool {
        return isdigit(symbol) != 0;
    }

    [[nodiscard]] auto IsDigitOrMinus(const char symbol) const noexcept -> bool {
        return isdigit(symbol) != 0 || symbol == '-';
    }

    [[nodiscard]] auto SumNumberInString(const std::string& string) const noexcept -> long long {
        long long result = 0;
        std::string_view subString(string);
        auto currentChar = subString.begin();
        bool isDeleting = !IsDigitOrMinus(*currentChar);

        while (currentChar != subString.end()) {
            if (IsMinus(*currentChar) && !IsDigit(*std::next(currentChar))) {
                currentChar++;
                isDeleting = true;
                continue;
            }
            if (IsDigitOrMinus(*currentChar) && isDeleting) {
                subString.remove_prefix(std::distance(subString.begin(), currentChar));
                isDeleting = false;
            }
            if (!isDeleting && IsDigitOrMinus(*currentChar)) {
                if ((IsMinus(*currentChar) && !IsDigitOrMinus(*std::next(currentChar))) ||
                    ((IsDigit(*currentChar) && !IsDigit(*std::next(currentChar))))) {
                    ADD_DELETE
                }
            }
            else if (!isDeleting && !IsDigit(*currentChar)) {
                ADD_DELETE
            }
            if (isDeleting && !IsDigitOrMinus(*currentChar)) {
                currentChar++;
                continue;
            }
            currentChar++;
        }
        return result;
    }
public:
    SummerNumberInConsole() : result_(0) {
        std::string buffer;
        while (std::getline(std::cin, buffer, '\n')) {
            strings_.push_back(std::move(buffer));
        }
    }

    [[nodiscard]] auto GetSum() noexcept -> long long {
        for (const auto& string : strings_) {
            result_ += SumNumberInString(string);
        }
        return result_;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    SummerNumberInConsole summer;
    std::cerr << summer.GetSum() << std::endl;
    return 0;
}