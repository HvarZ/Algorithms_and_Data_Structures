#include <ios>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <algorithm>

template <typename Key, typename Value>
class MinHeap {
public:
  using minMaxResult = std::tuple<Key, size_t, Value>;
  using searchResult = std::tuple<bool, size_t, Value>;
  using extractResult = std::pair<Key, Value>;

  [[nodiscard]] bool empty() const { return data.empty(); }

  void add(const Key &key, const Value &value) {
    if (isExistKey(key)) {
      throw std::logic_error("error");
    }
    matrix[key] = data.size();
    data.emplace_back(key, value);
    heapifyUp(data.size() - 1);
  }

  void set(const Key &key, const Value &value) {
    auto targetPtr =
        std::find_if(data.begin(), data.end(),
                     [&key](const Node &node) { return node.key == key; });
    if (targetPtr == data.end()) {
      throw std::logic_error("error");
    }

    data[std::distance(data.begin(), targetPtr)].value = value;
  }

  void erase(const Key &key) {
    auto [f, i, v] = search(key);
    if (!f) {
      throw std::logic_error("error");
    }
    std::swap(matrix[key], matrix[data[data.size() - 1].key]);
    matrix.erase(key);
    data[i] = data[data.size() - 1];
    data.pop_back();
    if (i == 0 || key < data[i].key) {
      heapifyDown(i);
    } else {
      heapifyUp(i);
    }
  }

  extractResult extract() {
    if (empty()) {
      throw std::logic_error("error!");
    }
    auto result = extractResult(data[0].key, data[0].value);
    erase(data[0].key);
    return result;
  }

  [[nodiscard]] searchResult search(const Key &key) const {
    auto iter = matrix.find(key);
    if (iter == matrix.end()) {
      return searchResult(false, 0, std::string());
    }
    return searchResult(true, iter->second, data[iter->second].value);
  }

  [[nodiscard]] minMaxResult max() const {
    if (empty()) {
      throw std::logic_error("error");
    }
    auto max = data[data.size() / 2].key;
    for (size_t i = data.size() / 2 + 1; i < data.size(); ++i) {
      if (max < data[i].key)
        max = data[i].key;
    }
    return minMaxResult(max, matrix.at(max), data[matrix.at(max)].value);
  }

  [[nodiscard]] minMaxResult min() const {
    if (empty()) {
      throw std::logic_error("error!");
    }
    return minMaxResult(data[0].key, 0, data[0].value);
  }

  void print(std::ostream &out) const {
    if (!empty()) {
      std::cout << "[" << std::to_string(data.front().key) << " "
                << data.front().value << "]\n";
    } else {
      std::cout << "_\n";
      return;
    }
    size_t index = 1;
    size_t degree = 2;
    while (index < data.size()) {
      std::string level;
      for (int i = 0; i < degree; ++i) {
        if (index < data.size()) {
          level += "[" + std::to_string(data[index].key) + " ";
          level += data[index].value;
          if (index % 2 == 0)
            level += " " + std::to_string(data[(index - 2) / 2].key) + "] ";
          else
            level += " " + std::to_string(data[(index - 1) / 2].key) + "] ";
          index++;
        } else
          level += "_ ";
      }
      level.pop_back();
      std::cout << level << "\n";
      degree <<= 1;
    }
  }

private:
  struct Node {
    explicit Node(Key key_, Value v) : key(key_), value(std::move(v)) {}

    bool operator<(const Node &node) const { return key < node.key; }

    Key key;
    Value value;
  };

  std::vector<Node> data;
  std::unordered_map<Key, size_t> matrix;

private:
  [[nodiscard]] bool isExistKey(const Key key) const {
    return matrix.count(key) != 0;
  }

  void heapifyDown(size_t i) {
    while (2 * i + 1 < data.size()) {
      auto left = 2 * i + 1;
      auto right = 2 * i + 2;
      auto less = left;
      if (right < data.size() && data[right] < data[left]) {
        less = right;
      }
      if (data[i] < data[less]) {
        break;
      }
      std::swap(matrix[data[i].key], matrix[data[less].key]);
      std::swap(data[i], data[less]);
      i = less;
    }
  }

  void heapifyUp(size_t i) {
    while (i && data[i] < data[(i - 1) / 2]) {
      std::swap(matrix[data[i].key], matrix[data[(i - 1) / 2].key]);
      std::swap(data[i], data[(i - 1) / 2]);
      i = (i - 1) / 2;
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  MinHeap<long long, std::string> heap;
  std::string command, value;
  long long key;

  while (std::cin >> command) {
    if (command.empty()) {
      continue;
    }
    if (command == "add") {
      try {
        std::cin >> key >> value;
        heap.add(key, value);
      } catch (...) {
        std::cout << "error" << std::endl;
      }
    } else if (command == "set") {
      try {
        std::cin >> key >> value;
        heap.set(key, value);
      } catch (...) {
        std::cout << "error" << std::endl;
      }
    } else if (command == "delete") {
      try {
        std::cin >> key;
        heap.erase(key);
      } catch (...) {
        std::cout << "error" << std::endl;
      }
    } else if (command == "min") {
      try {
        auto [key_, i, v] = heap.min();
        std::cout << key_ << " " << i << " " << v << std::endl;
      } catch (...) {
        std::cout << "error" << std::endl;
      }
    } else if (command == "max") {
      try {
        auto [key_, i, v] = heap.max();
        std::cout << key_ << " " << i << " " << v << std::endl;
      } catch (...) {
        std::cout << "error" << std::endl;
      }
    } else if (command == "extract") {
      try {
        auto [key_, v] = heap.extract();
        std::cout << key_ << " " << v << std::endl;
      } catch (...) {
        std::cout << "error" << std::endl;
      }
    } else if (command == "print") {
      heap.print(std::cout);
    } else if (command == "search") {
      std::cin >> key;
      auto [f, i, v] = heap.search(key);
      if (f) {
        std::cout << std::noboolalpha << f << " " << i << " " << v << std::endl;
      } else {
        std::cout << "0" << std::endl;
      }
    } else {
      std::cout << "error" << std::endl;
    }
  }
  return 0;
}