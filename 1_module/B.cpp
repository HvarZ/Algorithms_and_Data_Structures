#include <iostream>
#include <string>
#include <utility>
#include <algorithm>

class Stack {
private:
    std::string* data_;
    size_t capacity_;
    size_t size_;
public:
    Stack();
    ~Stack();
    explicit Stack(size_t capacity);
    void push(const std::string& value);
    void pop();
    void print() const noexcept;
};


Stack::Stack() : data_(nullptr), capacity_(0), size_(0) {}

Stack::~Stack() {
    delete [] data_;
}

Stack::Stack(const size_t capacity) : data_(new std::string[capacity]), capacity_(capacity), size_(0) {}

void Stack::push(const std::string &value) {
    if (size_ >= capacity_) {
        std::cout << "overflow" << std::endl;
        return;
    }
    data_[size_++] =  value;
}

void Stack::pop() {
    if (!size_) {
        std::cout << "underflow" << std::endl;
        return;
    }
    std::cout << data_[--size_] << std::endl;
}

void Stack::print() const noexcept {
    if (!size_) {
        std::cout << "empty" << std::endl;
        return;
    }
    for (size_t i = 0; i < size_ - 1; ++i) {
        std::cout << data_[i] << " ";
    }
    std::cout << data_[size_ - 1];
    std::cout << std::endl;
}

auto IsZeroSpace(const std::string& string) noexcept -> bool {
    return std::find(string.begin(), string.end(), ' ') == string.end();
}

auto IsOneSpace(const std::string& string) noexcept -> bool {
    size_t numberSpace = 0;
    for (const auto& symbol : string) {
        if (symbol == ' ') numberSpace++;
        if (numberSpace > 1) return false;
    }
    return true;
}

auto GetCommand(const std::string& string) noexcept -> std::string {
    return std::string(string.begin(), std::find(string.begin(), string.end(), ' '));
}

auto GetValue(const std::string& string) noexcept -> std::string {
    return std::string(std::next(std::find(string.begin(), string.end(), ' ')),
                       string.end());
}

auto IsValidCommand(const std::string& string) noexcept -> bool {
    auto command = GetCommand(string);

    if ((command == "set_size" || command == "push") && IsOneSpace(string)) {
        return true;
    }
    if ((string == "print" || string == "pop") && IsZeroSpace(string)) {
        return true;
    }

    return false;
}

auto IsContainsSetSize(const std::string& line) noexcept -> bool {
    return line.find("set_size") != std::string::npos && IsValidCommand(line);

}

int main() {
    std::string buffer;
    bool isSetSizeFounded = false;
    Stack* ptrStack = nullptr;
    while (getline(std::cin, buffer, '\n')) {
        if (buffer.empty()) continue;
        else if (!IsContainsSetSize(buffer) && !isSetSizeFounded) {
            std::cout << "error" << std::endl;
            continue;
        }
        else if (IsContainsSetSize(buffer) && !isSetSizeFounded) {
            ptrStack = new Stack(std::stoi(GetValue(buffer)));
            isSetSizeFounded = true;
        }

        else if (IsValidCommand(buffer) && isSetSizeFounded) {
            std::string command = GetCommand(buffer);
            if (command == "push") {
                ptrStack->push(GetValue(buffer));
            }
            else if (command == "pop") {
                ptrStack->pop();
            }
            else if (command == "print") {
                ptrStack->print();
            }
            else
                std::cout << "error" << std::endl;
        }

        else {
            std::cout << "error" << std::endl;
        }

    }

    delete ptrStack;

    return 0;
}