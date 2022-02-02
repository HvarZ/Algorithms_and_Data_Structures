#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <bitset>

#include <regex>

using namespace std;

struct result {
    unsigned long weight = 0;
    unsigned long cost = 0;
    unsigned long long trace = 0;
};

struct gem {
    unsigned long weight = 0;
    unsigned long cost = 0;
};

struct weight_trace {
    unsigned long weight = 0;
    unsigned long trace = 0;
};


result backpack(const vector<gem> &gems,
                 const unsigned long max_weight) {
    unsigned long sum_cost = 0, sum_weight = 0;

    for (const auto& gem : gems) {
        sum_cost += gem.cost;
        sum_weight += gem.weight;
    }

    if (max_weight > sum_weight) {
        return {sum_weight, sum_cost,(unsigned long)(pow(2, gems.size()) - 1) * 2};
    }

    vector<weight_trace> matrix(sum_cost, {max_weight + 1, 0});
    matrix[0].weight = 0;

    for (size_t i = 0; i < gems.size(); ++i) {
        for (size_t j = sum_cost - 1; j >= gems[i].cost; --j) {
            if (gems[i].weight + matrix[j - gems[i].cost].weight < matrix[j].weight) {
                matrix[j] = matrix[j - gems[i].cost];
                matrix[j].weight += gems[i].weight;
                matrix[j].trace |= (1 << (i + 1));
            }
        }
    }

    auto iter_result = find_if(matrix.rbegin(), matrix.rend(), [&max_weight](const weight_trace& pair) {
        return pair.weight <= max_weight;
    });

    if (iter_result != matrix.rbegin()) {
        return {iter_result->weight, (unsigned long)distance(iter_result, matrix.rend()) - 1, iter_result->trace};
    } else {
        return {0, 0, 0};
    }
}



int main() {
    ios::sync_with_stdio(false);
    string buffer;
    getline(cin, buffer, '\n');
    regex max_weight_regex("^[0-9]*$");
    regex gem_regex("^[0-9]*\\s[0-9]*$");
    while (!regex_match(buffer, max_weight_regex) || buffer.empty()) {
        if (buffer.empty()) {
            getline(cin, buffer, '\n');
            continue;
        }
        cout << "error\n";
        getline(cin, buffer, '\n');
    }
    auto max_weight = stoul(buffer);
    vector<gem> input;

    while (getline(cin, buffer, '\n')) {
        if (buffer.empty()) {
            continue;
        }
        if (!regex_match(buffer, gem_regex)) {
            cout << "error\n";
            continue;
        }

        auto iter_space = find(buffer.begin(), buffer.end(), ' ');

        input.push_back({stoul(string(buffer.begin(), iter_space)),
                           stoul(string(iter_space + 1, buffer.end()))});
    }

    result result = backpack(input, max_weight);
    cout << result.weight << " " << result.cost << endl;


    const unsigned long unsigned_long_bits = sizeof(unsigned_long_bits) * 8;
    bitset<unsigned_long_bits> bitset(result.trace);

    for (unsigned long i = 0; i < unsigned_long_bits; ++i) {
        if (bitset[i] == 1) {
            cout << i << endl;
        }
    }
    return 0;
}