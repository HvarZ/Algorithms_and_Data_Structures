#include <iostream>

auto Sum(int a, int b) -> int {
    return a + b;
}

int main() {
    int a = 0, b = 0;
    std::cin >> a >> b;
    std::cout << Sum(a, b) << std::endl;
    return 0;
}