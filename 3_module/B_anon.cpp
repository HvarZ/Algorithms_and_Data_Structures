#include <iostream>
#include <stack>

using std::stack;
using std::cout;
using std::cin;
using std::endl;

#define INC 1
#define DBL 2
#define DEC 3

uint8_t units(size_t n) {
    uint8_t count = 0;
    for (; n; n >>= 1) {
        count += n & 1;
    }
    return count;
}

stack<uint8_t> turns(size_t n) {
    stack<uint8_t> turns;
    while (n > 1 && n != 3) {
        if (n % 2 == 1) {
            uint8_t dec_n = units(n - 1);
            uint8_t inc_n = units(n + 1);
            if (dec_n < inc_n) {
                --n;
                turns.push(INC);
            }
            else {
                ++n;
                turns.push(DEC);
            }
        }
        else {
            n >>= 1;
            turns.push(DBL);
        }
    }

    if (n == 3) {
        turns.push(INC);
        turns.push(DBL);
        turns.push(INC);
        return turns;
    }

    turns.push(INC);

    return turns;
}

int main() {
    size_t n;
    cin >> n;

    stack<uint8_t> moves = turns(n);
    size_t n_moves = moves.size();

    for (size_t i = 0; i < n_moves; ++i) {
        uint8_t current = moves.top();
        moves.pop();
        switch (current) {
            case INC:
                cout << "inc" << endl;
                break;
            case DBL:
                cout << "dbl" << endl;
                break;
            case DEC:
                cout << "dec" << endl;
            default:
                break;
        }
    }
    return 0;
}