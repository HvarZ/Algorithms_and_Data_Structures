#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

enum class command {
    Inc,
    Dbl,
    Dec
};

size_t units(size_t n) {
    size_t count = 0;
    while (n > 0) {
        count += (n & 1);
        n >>= 1;
    }
    return count;
}

vector<command> get_moves(size_t n) {
    vector<command> result;
    while (n > 1 && n != 3) {
        if (n % 2 == 1) {
            size_t dec_n = units(n - 1);
            size_t inc_n = units(n + 1);
            if (dec_n < inc_n) {
                --n;
                result.push_back(command::Inc);
            } else {
                ++n;
                result.push_back(command::Dec);
            }
        } else {
            n >>= 1;
            result.push_back(command::Dbl);
        }
    }

    if (n == 3) {
        result.push_back(command::Inc);
        result.push_back(command::Dbl);
        result.push_back(command::Inc);
        return result;
    }

    result.push_back(command::Inc);

    return result;
}

int main() {
    size_t n;
    cin >> n;
    vector<command> moves_r = get_moves(n);

    for_each(moves_r.rbegin(), moves_r.rend(), [](const command command) {
        if (command == command::Inc) {
            cout << "inc" << endl;
        } else if (command == command::Dbl) {
            cout << "dbl" << endl;
        } else if (command == command::Dec) {
            cout << "dec" << endl;
        }
    });
    return 0;
}