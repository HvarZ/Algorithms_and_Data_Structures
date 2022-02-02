#include <iostream>

using namespace std;

long long sum_in_string(const string& str) {
    long long result_ = 0;
    string::const_iterator it = str.begin();
    while (it != str.end()) {
        if ((*it == '-' and isdigit(*(it + 1)) or isdigit(*it))) {
            result_ += stoll(string(it, it + 10));
            while (isdigit(*(it + 1))) it++;
        }
        it++;
    }
    return result_;
}


int main(int argc, char* argv[]) {
    long long result_ = 0;
    string current_string;
    while(getline(cin, current_string, '\n')) {
        result_ += sum_in_string(current_string);
    }
    cout << result_ << endl;
    return EXIT_SUCCESS;
}