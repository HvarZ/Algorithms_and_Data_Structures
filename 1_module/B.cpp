#include <iostream>
#include <string>

using namespace std;

template <typename T>
class Stack {
private:
    T* data;
    size_t max_size;
    size_t size;

public:
    Stack() {
        data = nullptr;
        max_size = 0;
        size = 0;
    }

    Stack& operator=(const Stack& stack) {
        if (this == &stack) {
            return *this;
        }
        delete [] data;
        max_size = stack.max_size;
        size = stack.size;
        data = new T[stack.max_size];

        for (size_t i = 0; i < size; i++) {
            data[i] = stack.data[i];
        }
        return *this;
    }

    void set_size(const size_t m_s) {
        data = new T[m_s];
        max_size = m_s;
        size = 0;
    }

    ~Stack() {
        delete [] data;
    }


    void push(const T &value) {
        if (size == max_size) {
            cout << "overflow\n";
            return;
        }

        data[size] =  value;
        size++;
    }
    void pop() {
        if (size == 0) {
            cout << "underflow\n";
            return;
        }
        size--;
        cout << data[size] << endl;
    }

    void print() {
        if (size == 0) {
            cout << "empty\n";
            return;
        }
        for (size_t i = 0; i < size - 1; ++i) {
            cout << data[i] << " ";
        }
        cout << data[size - 1] << endl;
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

void result(Stack<std::string>& stack) {
    bool set_size_founded = false;
    string current_string;
    while (getline(cin, current_string, '\n')) {
        if (current_string.empty()) {
            continue;
        }
        else if (!set_size_founded && size_founded(current_string)) {
            set_size_founded = true;
            stack.set_size(stoi(value(current_string)));
        }

        else if (set_size_founded  && true_command(current_string)) {
            string command_ = command(current_string);
        if (command_ == "push") {
            stack.push(value(current_string));
        }
        else if (command_ == "print") {
            stack.print();
        }
        else if (command_ == "pop") {
            stack.pop();
        }
        else
            cout << "error\n";
        }

        else if (!size_founded(current_string) && !set_size_founded) {
            cout << "error\n";
            continue;
        }

        else {
            cout << "error\n";
        }
    }
}


int main() {
    Stack<std::string> stack;
    result(stack);
    return 0;
}