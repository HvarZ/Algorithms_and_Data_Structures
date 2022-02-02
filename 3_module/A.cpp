#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <regex>

using namespace std;

struct jewel {
  uint64_t weight;
  uint64_t cost;

  explicit jewel(const uint64_t weight_, const uint64_t cost_)
      : weight(weight_), cost(cost_) {}
};

struct result {
  uint64_t weight;
  uint64_t cost;
  vector<uint64_t> trace;
};

class Knapsack final {
public:
  Knapsack(uint64_t max_mas_, vector<jewel> items_) :
      max_mas(max_mas_), items(move(items_)) {}

  result get_result() {
    uint64_t gpc = common_gpc(items, max_mas);
    std::pair<uint64_t, uint64_t> default_constructor(0, 0);
    vector<pair<uint64_t, uint64_t>> previous_string(max_mas / gpc + 1,
                                                    default_constructor);
    for (uint64_t i = 0; i < items.size(); ++i) {
      vector<pair<uint64_t, uint64_t>> current_string(max_mas / gpc + 1,
                                                   default_constructor);
      for (uint64_t j = 0; j <= max_mas / gpc; ++j) {
        if (j >= items[i].weight / gpc) {
          pair<uint64_t, uint64_t> buf_element(
              previous_string[j - items[i].weight / gpc].first + items[i].cost,
              previous_string[j - items[i].weight / gpc].second | (1 << i));
          if (previous_string[j].first > buf_element.first) {
            current_string[j] = previous_string[j];
          } else {
            current_string[j] = buf_element;
          }
        } else {
          current_string[j] = previous_string[j];
        }
      }
      previous_string = move(current_string);
    }
    auto tmp = get_jewel(previous_string[previous_string.size() - 1].second);
    return {tmp.first, previous_string[previous_string.size() - 1].first,
            tmp.second};
  }

private:
  uint64_t common_gpc(const vector<jewel>& items_, const uint64_t mas) const {
    uint64_t n = 0;
    uint64_t mas_max = mas;
    for (const auto& item : items_) {
      uint64_t current_weight = item.weight;
      while (n && current_weight) {
        current_weight < n ? n %= current_weight : current_weight %= n;
      }
      n =  n | current_weight;
    }

    while (mas_max && n) {
      n < mas_max ? mas_max %= n : n %= mas_max;
    }

    return mas_max | n;
  }

  pair<uint64_t, vector<uint64_t>> get_jewel(uint64_t elements) const {
    int counter = 0;
    uint64_t weight = 0;
    vector<uint64_t> elements_;
    while (elements > 0) {
      if (elements & 1) {
        elements_.push_back(counter + 1);
        weight += items[counter].weight;
      }
      elements = elements / 2;
      ++counter;
    }
    return {weight, elements_};
  }

private:
  uint64_t max_mas;
  vector<jewel> items;
};

int main() {
  ios::sync_with_stdio(false);
  string command;
  getline(cin, command, '\n');
  regex gem_regex("^[0-9]*\\s[0-9]*$");
  regex max_weight_regex("^[0-9]*$");

  while (!regex_match(command, max_weight_regex) || command.empty()) {
    if (command.empty()) {
      getline(cin, command, '\n');
      continue;
    }
    cout << "error\n";
    getline(cin, command, '\n');
  }
  uint64_t max_mas = stoul(command);

  vector<jewel> elements;
  string weight, cost;

  while (getline(cin, command, '\n')) {
    if (command.empty()) {
      continue;
    }
    if (!regex_match(command, gem_regex)) {
      cout << "error\n";
      continue;
    }

    auto iter_space = find(command.begin(), command.end(), ' ');

    elements.emplace_back(stoul(string(command.begin(), iter_space)),
                     stoul(string(iter_space + 1, command.end())));
  }

  Knapsack k(max_mas, elements);
  auto result = k.get_result();

  cout << result.weight << " " << result.cost << endl;
  for (const auto& elem : result.trace) {
    cout << elem << endl;
  }

  return 0;
}