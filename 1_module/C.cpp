#include <string>
#include <fstream>

using namespace std;

template <typename T>
class Queue {
private:
    T* data;
    size_t max_size;
    const int empty = -1;
    long long head;
    long long tail;

public:
    Queue() {
        data = nullptr;
        max_size = 0;
        reset();
    }

    ~Queue() {
        delete [] data;
    }

    void reset() {
        head = -1;
        tail = -1;
    }

    void push(const string& value, ofstream* write = nullptr) {
        if ((head == 0 and tail == max_size - 1) or (head == tail + 1)) {
            *write << "overflow\n";
            return;
        }
        if (head == empty) {
            head++;
        }
        tail = (tail + 1) % max_size;
        data[tail] = value;
    }

    void set_size(const size_t m_s) {
        max_size = m_s;
        data = new string[m_s];
        reset();
    }

    void print(ofstream* write = nullptr) {
        if (head == empty) {
            *write << "empty\n";
            return;
        }
        for(size_t i = head; i != tail; i = (i + 1) % max_size) {
            *write << data[i] << " ";
        }
        *write << data[tail] << endl;
    }

    void pop(ofstream* write = nullptr) {
        if (head == empty) {
            *write << "underflow\n";
            return;
        }
        *write << data[head] << endl;

        if (head == tail) {
            reset();
        }
        else {
            head = (head + 1) % max_size;
        }
    }
};


int number_space(const string& str) {
    int counter = 0;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == ' ') {
            counter++;
        }
        if (counter > 1) {
            return 2;
        }
    }
    return counter;
}


string command(const string& str) {
    string::const_iterator it = str.begin();
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == ' ') {
            it += i;
            break;
        }
        if (i == str.size() - 1) {
            it += (i + 1);
        }
    }

    return string(str.begin(), it);
}

string value(const string& str) {
    string::const_iterator it = str.begin();
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == ' ') {
            it += i;
            break;
        }
    }
    return string(it + 1, str.end());
}

bool true_command(const string& str) {
    string command_ = command(str);
    if (command_ == "set_size" && number_space(str) == 1) return true;
    else if (command_ == "push" && number_space(str) == 1) return true;
    else if (command_ == "print"  && number_space(str) == 0) return true;
    else if (command_ == "pop" && number_space(str) == 0) return true;
    else return false;
}

bool size_founded(const string& line) {
    if (true_command(line)) {
        return line.find("set_size") != string::npos;
    }
    return false;
}

void result(Queue<string>& queue, char*& input, char*& output) {
    ifstream read;
    ofstream write;

    read.open(input);
    write.open(output, ios::app);

    bool set_size_founded = false;
    string current_string;
    while (getline(read, current_string, '\n')) {
        if (current_string.empty()) {
            continue;
        }
        else if (!set_size_founded && size_founded(current_string)) {
            set_size_founded = true;
            queue.set_size(stoi(value(current_string)));
        }

        else if (set_size_founded  && true_command(current_string)) {
            string command_ = command(current_string);
            if (command_ == "push") {
                queue.push(value(current_string), &write);
            }
            else if (command_ == "print") {
                queue.print(&write);
            }
            else if (command_ == "pop") {
                queue.pop(&write);
            }
            else
                write << "error\n";
        }

        else if (!size_founded(current_string) && !set_size_founded) {
            write << "error\n";
            continue;
        }

        else {
            write << "error\n";
        }
    }
}


int main(int argc, char* argv[]) {
    Queue<string> queue;
    result(queue, argv[1], argv[2]);
    return EXIT_SUCCESS;
}