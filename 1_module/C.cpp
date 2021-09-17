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
    int64_t front_;
    int64_t tail_;

public:
    Queue();
    ~Queue();
    explicit Queue(const std::string& writingFileName, size_t capacity);
    void push(const std::string& value);
    void print() noexcept;
    void pop();
};

Queue::Queue() : data_(nullptr), capacity_(0), front_(-1), tail_(-1) {}

Queue::~Queue() {
    delete [] data_;
}

Queue::Queue(const std::string &writingFileName, const size_t capacity)
            : data_(new std::string[capacity]), capacity_(capacity), front_(-1), tail_(-1) {
    writingStream_.open(writingFileName, std::ios::app);
}

void Queue::push(const std::string &value) {
    if ((front_ == 0 && tail_ == capacity_ - 1) || (front_ == tail_ + 1)) {
        writingStream_ << "overflow" << std::endl;
        return;
    }
    if (front_ == -1) {
        front_++;
    }
    tail_ = (tail_ + 1) % capacity_;
    data_[tail_] = value;
}

void Queue::print() noexcept {
    if (front_ == -1) {
        writingStream_ << "empty" << std::endl;
        return;
    }
    for(auto i = front_; i != tail_; i = (i + 1) % capacity_) {
        writingStream_ << data_[i] << " ";
    }
    writingStream_ << data_[tail_] << std::endl;
}

void Queue::pop() {
    if (front_ == -1) {
        writingStream_ << "underflow" << std::endl;
        return;
    }
    writingStream_ << data_[front_] << std::endl;

    if (front_ == tail_) {
        front_ = -1; tail_ = -1;
    }
    else {
        front_ = (front_ + 1) % capacity_;
    }
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
            if (symbol == ' ') {
                numberSpace++;
            }
            if (numberSpace > 1) {
                return false;
            }
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

            Queue queue(writingFileName, std::stoi(GetValue(inputLines_[numberStrSetSize])));
            for (size_t i = numberStrSetSize + 1; i < inputLines_.size(); ++i) {
                if (GetCommand(inputLines_[i]) == "push" && IsValidCommand(inputLines_[i])) {
                    queue.push(GetValue(inputLines_[i]));
                }
                else if (GetCommand(inputLines_[i]) == "print" && IsValidCommand(inputLines_[i])) {
                    queue.print();
                }
                else if (GetCommand(inputLines_[i]) == "pop" && IsValidCommand(inputLines_[i])) {
                    queue.pop();
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

