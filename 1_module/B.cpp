#include <iostream>
#include <vector>
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

class Receiver {
private:
    std::vector<std::string> inputLines_;

private:
    [[nodiscard]] auto FindSetSize() const noexcept -> size_t {
        for (size_t i = 0; i < inputLines_.size(); ++i) {
            if (inputLines_[i].find("set_size") != std::string::npos) {
                return i;
            }
        }
        return -1;
    }

    [[nodiscard]] auto IsZeroSpace(const std::string& string) const noexcept -> bool {
        return std::find(string.begin(), string.end(), ' ') == string.end();
    }

    [[nodiscard]] auto IsOneSpace(const std::string& string) const noexcept -> bool {
        size_t numberSpace = 0;
        for (const auto& symbol : string) {
            if (symbol == ' ') numberSpace++;
            if (numberSpace > 1) return false;
        }
        return true;
    }

    [[nodiscard]] auto GetCommand(const std::string& string) const noexcept -> std::string {
        return std::string(string.begin(), std::find(string.begin(), string.end(), ' '));
    }

    [[nodiscard]] auto GetValue(const std::string& string) const noexcept -> std::string {
        return std::string(std::next(std::find(string.begin(), string.end(), ' ')),
                        string.end());
    }


    [[nodiscard]] auto IsValidCommand(const std::string& string) const noexcept -> bool {
        auto command = GetCommand(string);

        if ((command == "set_size" || command == "push") && IsOneSpace(string)) {
            return true;
        }
        if ((string == "print" || string == "pop") && IsZeroSpace(string)) {
            return true;
        }

        return false;
    }

public:
    Receiver() {
        std::string buffer;
        while(std::getline(std::cin, buffer, '\n')) {
            if (buffer.empty()) continue;
            inputLines_.push_back(std::move(buffer));
        }
    }

    void Output() const {
        size_t numberStrSetSize = FindSetSize();
        if (numberStrSetSize == -1) {
            for (size_t i = 0; i < inputLines_.size(); i++) {
                std::cout << "error" << std::endl;
            }
            return;
        }

        else {
            for (size_t i = 0; i < numberStrSetSize; ++i) {
                std::cout << "error" << std::endl;
            }
        }
        if (IsValidCommand(inputLines_[numberStrSetSize])) {

            Stack stack(std::stoi(GetValue(inputLines_[numberStrSetSize])));
            for (size_t i = numberStrSetSize + 1; i < inputLines_.size(); ++i) {
                if (GetCommand(inputLines_[i]) == "push" && IsValidCommand(inputLines_[i])) {
                    stack.push(GetValue(inputLines_[i]));
                }
                else if (GetCommand(inputLines_[i]) == "print" && IsValidCommand(inputLines_[i])) {
                    stack.print();
                }
                else if (GetCommand(inputLines_[i]) == "pop" && IsValidCommand(inputLines_[i])) {
                    stack.pop();
                }
                else {
                    std::cout << "error" << std::endl;
                }
            }

        }
    }
};


int main() {
    Receiver receiver;
    receiver.Output();
    return 0;
}