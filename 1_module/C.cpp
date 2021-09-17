#include <fstream>
#include <string>
#include <algorithm>
#include <utility>
#include <memory>

#define EMPTY_POSITION -1

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

Queue::Queue() : data_(nullptr), capacity_(0), front_(EMPTY_POSITION), tail_(EMPTY_POSITION) {}

Queue::~Queue() {
    delete [] data_;
}

Queue::Queue(const std::string &writingFileName, const size_t capacity)
            : data_(new std::string[capacity]), capacity_(capacity), front_(EMPTY_POSITION), tail_(EMPTY_POSITION) {
    writingStream_.open(writingFileName, std::ios::app);
}

void Queue::push(const std::string &value) {
    if ((front_ == 0 && tail_ == capacity_ - 1) || (front_ == tail_ + 1)) {
        writingStream_ << "overflow" << std::endl;
        return;
    }
    if (front_ == EMPTY_POSITION) {
        front_++;
    }
    tail_ = (tail_ + 1) % capacity_;
    data_[tail_] = value;
}

void Queue::print() noexcept {
    if (front_ == EMPTY_POSITION) {
        writingStream_ << "empty" << std::endl;
        return;
    }
    for(auto i = front_; i != tail_; i = (i + 1) % capacity_) {
        writingStream_ << data_[i] << " ";
    }
    writingStream_ << data_[tail_] << std::endl;
}

void Queue::pop() {
    if (front_ == EMPTY_POSITION) {
        writingStream_ << "underflow" << std::endl;
        return;
    }
    writingStream_ << data_[front_] << std::endl;

    if (front_ == tail_) {
        front_ = EMPTY_POSITION;
        tail_ = EMPTY_POSITION;
    }
    else {
        front_ = (front_ + 1) % capacity_;
    }
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

int main(int argc, char* argv[]) {
    std::string buffer;

    std::ifstream readingStream;
    std::ofstream writingStream;
    readingStream.open(argv[1]);
    writingStream.open(argv[2], std::ios::app);

    bool isSetSizeFounded = false;

    Queue* ptrQueue = nullptr;

    while (getline(readingStream, buffer, '\n')) {
        if (buffer.empty()) continue;
        else if (!IsContainsSetSize(buffer) && !isSetSizeFounded) {
            writingStream << "error" << std::endl;
            continue;
        }
        else if (IsContainsSetSize(buffer) && !isSetSizeFounded) {
            ptrQueue = new Queue(argv[2],std::stoi(GetValue(buffer)));
            isSetSizeFounded = true;
        }

        else if (IsValidCommand(buffer) && isSetSizeFounded) {
            std::string command = GetCommand(buffer);
            if (command == "push") {
                ptrQueue->push(GetValue(buffer));
            }
            else if (command == "pop") {
                ptrQueue->pop();
            }
            else if (command == "print") {
                ptrQueue->print();
            }
            else
                writingStream << "error" << std::endl;
        }

        else {
            writingStream << "error" << std::endl;
        }

    }

    delete ptrQueue;
    return 0;
}

