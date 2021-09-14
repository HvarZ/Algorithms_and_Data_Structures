#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <memory>


class Queue {
private:
    std::string* data_;
    std::ofstream writingStream_;
    size_t capacity_;
    size_t size_;

public:
    Queue();
    ~Queue();
    explicit Queue(const std::string& writingFileName, size_t capacity);
    void push(const std::string& value);
    void print() noexcept;
    void pop();
};

Queue::Queue() : data_(nullptr), capacity_(0), size_(0) {}

Queue::~Queue() {
    delete [] data_;
}

Queue::Queue(const std::string &writingFileName, const size_t capacity)
                            : data_(new std::string[capacity]), capacity_(capacity), size_(0) {
    writingStream_.open(writingFileName, std::ios::app);
}

void Queue::push(const std::string &value) {
    if (size_ == capacity_) {
        writingStream_ << "overflow" << std::endl;
        return;
    }
    data_[size_++] = value;
}

void Queue::print() noexcept {
    if (!size_) {
        writingStream_ << "empty" << std::endl;
        return;
    }
    for (size_t i = 0; i < size_ - 1; ++i) {
        writingStream_ << data_[i] << " ";
    }
    writingStream_ << data_[size_ - 1] << std::endl;
}

void Queue::pop() {
    if (!size_) {
        writingStream_ << "underflow" << std::endl;
        return;
    }
    writingStream_ << data_[0] << std::endl;
    for (size_t i = 0; i < size_ - 1; ++i) {
        data_[i] = data_[i + 1];
    }
    size_--;
}

class Receiver {
private:
    std::vector<std::string> inputLines_;
    std::ifstream readingStream_;
    std::ofstream writingStream_;


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
    explicit Receiver(const std::string &readingFileName) {
        std::string buffer;
        readingStream_.open(readingFileName);
        while(std::getline(readingStream_, buffer, '\n')) {
            if (buffer.empty()) continue;
            inputLines_.push_back(std::move(buffer));
        }
    }

    void Output(const std::string& writingFileName) {
        writingStream_.open(writingFileName, std::ios::app);
        size_t numberStrSetSize = FindSetSize();
        if (numberStrSetSize == -1) {
            for (size_t i = 0; i < inputLines_.size(); i++) {
                writingStream_ << "error" << std::endl;
            }
            return;
        }

        else {
            for (size_t i = 0; i < numberStrSetSize; ++i) {
                writingStream_ << "error" << std::endl;
            }
        }
        if (IsValidCommand(inputLines_[numberStrSetSize])) {

            Queue stack(writingFileName, std::stoi(GetValue(inputLines_[numberStrSetSize])));
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
                    writingStream_ << "error" << std::endl;
                }
            }

        }
    }
};


int main(int argc, char* argv[]) {
    Receiver receiver(argv[1]);
    receiver.Output(argv[2]);
    return 0;
}