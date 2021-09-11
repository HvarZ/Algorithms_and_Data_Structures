#include <iostream>
#include <vector>
#include <string_view>
#include <algorithm>

class SummerNumberInConsole {
private:
    std::vector<std::string> strings_;
    int result_;

    [[nodiscard]] auto IsDigit(const char symbol) const noexcept -> bool {
        return isdigit(symbol) != 0;
    }

    [[nodiscard]] auto SumNumberInString(const std::string& string) const noexcept -> int {
        int result = 0;
        std::string_view subString(string);
        auto currentChar = subString.begin();
        bool isDeleting = !IsDigit(*currentChar);

        while (currentChar++ != subString.end()) {
            if (IsDigit(*currentChar) && isDeleting) {
                subString.remove_prefix(std::distance(subString.begin(), currentChar));
                isDeleting = false;
            }
            else if ((!isDeleting && IsDigit(*currentChar)) || (isDeleting && !IsDigit(*currentChar))) {
                continue;
            }
            else if (!isDeleting && !IsDigit(*currentChar)) {
                result += std::stoi(std::string(subString.begin(), currentChar));
                subString.remove_prefix(std::distance(subString.begin(), currentChar));
                isDeleting = true;
            }
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

    [[nodiscard]] auto GetSum() noexcept -> int{
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
    std::cout << summer.GetSum() << std::endl;
    return 0;
}