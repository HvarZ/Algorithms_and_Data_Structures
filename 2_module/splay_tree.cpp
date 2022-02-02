#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <tuple>
#include <algorithm>

template <typename Key, typename Value>
class SplayTree {
public:
  using minMaxResult = std::pair<Key, Value>;
  SplayTree() : root(nullptr) {}
  ~SplayTree() {
    clear(root);
  }

  std::string search(const Key key_) {
    auto *aim = find(key_);
    Splay(aim);
    if (aim->key != key_) {
      throw std::logic_error("error");
    }
    return aim->value;
  }

  void add(const Key &key_, const Value &value_) {
    if (root == nullptr) {
      Node *aim = new Node;
      aim->key = key_;
      aim->value = value_;
      root = aim;
      return;
    }
    auto *tmp = find(key_);
    if (tmp->key == key_) {
      Splay(tmp);
      throw std::logic_error("error");
    }
    auto *aim = new Node;
    auto *parent_node_add = tmp;
    aim->parent = parent_node_add;
    aim->key = key_;
    aim->value = value_;
    if (parent_node_add->key < key_)
      parent_node_add->right = aim;
    else if (parent_node_add->key > key_)
      parent_node_add->left = aim;
    Splay(aim);
  }

  void set(const Key key_, const Value &value_) {
    auto* aim = find(key_);
    if (aim->key != key_){
      Splay(aim);
      throw std::logic_error("error");
    } else {
      aim->value = value_;
    }
    Splay(aim);
  }

  void erase(const Key key_) {
    Node *aim = find(key_);
    Splay(aim);
    if (aim->key != key_)
      throw std::logic_error("error");
    if (aim == root && aim->right == nullptr &&
        aim->left == nullptr) {
      delete root;
      root = nullptr;
      return;
    }
    if (aim->right == nullptr) {
      root = aim->left;
      root->parent = nullptr;
      delete aim;
      return;
    } else if (aim->left == nullptr) {
      root = aim->right;
      root->parent = nullptr;
      delete aim;
      return;
    }
    root = aim->left;
    root->parent = nullptr;
    Node *max = root;
    while (max->right != nullptr)
      max = max->right;
    Splay(max);
    root->right = aim->right;
    root->right->parent = root;
    delete aim;
  }

  minMaxResult min() {
    if (root == nullptr) {
      throw std::logic_error("error");
    }
    auto *current = root;
    while (true) {
      if (current->left == nullptr) {
        Splay(current);
        return minMaxResult(current->key, current->value);
      } else {
        current = current->left;
      }
    }
  }

  minMaxResult max() {
    if (root == nullptr) {
      throw std::logic_error("error");
    }
    auto *current = root;
    while (true) {
      if (current->right == nullptr) {
        Splay(current);
        return minMaxResult(current->key, current->value);
      } else {
        current = current->right;
      }
    }
  }

  void print(std::ostream &out) const noexcept {
    if (root == nullptr) {
      out << "_\n";
      return;
    }
    out << "[" << std::to_string(root->key) << " " << root->value << "]\n";
    std::queue<decltype(root)> q;

    q.push(root->left);
    q.push(root->right);

    decltype(root) current;

    size_t elementCounter = 0;
    size_t numberElements = 2;
    std::string line;

    while (!q.empty()) {
      current = q.front();
      q.pop();
      if (current != nullptr) {
        line += "[" + std::to_string(current->key) + " " + current->value +
                " " + std::to_string(current->parent->key) + "]";
        q.push(current->left);
        q.push(current->right);
      } else {
        line += "_";
        q.push(nullptr);
        q.push(nullptr);
      }
      ++elementCounter;
      if (elementCounter == numberElements) {

        std::string lineNull = "_";
        for (size_t j = 1; j < numberElements; ++j)
          lineNull += " _";

        if (line != lineNull) {
          out << line << '\n';
          line.clear();
        } else {
          break;
        }
        numberElements <<= 1;
        elementCounter = 0;
      } else {
        line += " ";
      }
    }
  }

private:
  struct Node {
    Node *left;
    Node *right;
    Node *parent;
    Key key;
    Value value;
    Node() : left(nullptr), right(nullptr), parent(nullptr), key(0) {}
  };
  Node *root;

private:
  void clear(Node *node) {
    if (node != nullptr) {
      clear(node->left);
      clear(node->right);
      delete node;
    }
  }

  Node *find(const Key key_) {
    if (root == nullptr) {
      throw std::logic_error("error");
    }
    Node *aim = root;
    while (aim->left != nullptr || aim->right != nullptr) {
      if (key_ == aim->key)
        return aim;
      if (key_ > aim->key) {
        if (aim->right == nullptr)
          break;
        aim = aim->right;
      } else if (key_ < aim->key) {
        if (aim->left == nullptr)
          break;
        aim = aim->left;
      }
    }
    return aim;
  }
  void Zag(Node *current) {
    Node *tmp = current->parent;
    current->parent = tmp->parent;
    if (current->parent == nullptr) {
      root = current;
    } else if (tmp->parent->left == tmp)
      current->parent->left = current;
    else if (tmp->parent->right == tmp)
      current->parent->right = current;
    tmp->parent = current;
    tmp->right = current->left;
    if (tmp->right != nullptr)
      tmp->right->parent = tmp;
    current->left = tmp;
  }

  void Zig(Node *current) {
    Node *tmp = current->parent;
    current->parent = tmp->parent;
    if (current->parent == nullptr) {
      root = current;
    } else if (tmp->parent->left == tmp)
      current->parent->left = current;
    else if (tmp->parent->right == tmp)
      current->parent->right = current;
    tmp->parent = current;
    tmp->left = current->right;
    if (tmp->left != nullptr)
      tmp->left->parent = tmp;
    current->right = tmp;
  }

  void Splay(Node *current) {
    if (current == nullptr)
      return;
    while (current != root) {
      if (current->parent == root) {
        if (current->parent->right == current)
          Zag(current);
        else if (current->parent->left == current)
          Zig(current);
      } else if (current->parent->left == current) {
        if (current->parent->parent->left == current->parent) {
          Zig(current->parent);
          Zig(current);
        } else if (current->parent->parent->right == current->parent) {
          Zig(current);
          Zag(current);
        }
      } else if (current->parent->right == current) {
        if (current->parent->parent->left == current->parent) {
          Zag(current);
          Zig(current);
        } else if (current->parent->parent->right == current->parent) {
          Zag(current->parent);
          Zag(current);
        }
      }
    }
  }
};

std::tuple<std::string, long long, std::string> parsingString(std::string &command) {
  std::tuple<std::string, long long, std::string> result;
  auto iterSpace = std::find(command.begin(), command.end(), ' ');
  std::get<0>(result) = std::string(command.begin(), iterSpace);
  if (std::get<0>(result) == "add" || std::get<0>(result) == "set") {
    if (iterSpace == command.end()) {
      throw std::logic_error("error");
    }
    auto iterSecondSpace = std::find(iterSpace + 1, command.end(), ' ');
    std::string stringKey = {iterSpace + 1, iterSecondSpace};
    if (iterSecondSpace == command.end()) {
      std::get<2>(result) = std::string();
      try {
        std::get<1>(result) = std::stoll(stringKey);
      } catch (...) {
        throw std::logic_error("error");
      }
      return result;
    }
    std::string stringValue = {iterSecondSpace + 1, command.end()};

    try {
      std::get<1>(result) = std::stoll(stringKey);
    } catch (...) {
      throw std::logic_error("error");
    }
    std::get<2>(result) = stringValue;
  } else if (std::get<0>(result) == "search" || std::get<0>(result) == "delete") {
    if (iterSpace == command.end()) {
      throw std::logic_error("error");
    }

    std::string stringKey = {iterSpace + 1, command.end()};
    try {
      std::get<1>(result) = std::stoll(stringKey);
    } catch (...) {
      throw std::logic_error("error");
    }
  } else if (std::get<0>(result) == "min" || std::get<0>(result) == "max" ||
      std::get<0>(result) == "print") {
    if (iterSpace != command.end()) {
      throw std::logic_error("error");
    }
  }
  return result;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::string buffer;
  SplayTree<long long, std::string> splayTree;
  while (getline(std::cin, buffer, '\n')) {
    try {
      auto [command, key, value] = parsingString(buffer);
      if (command.empty()) {
        continue;
      } else if (command == "add") {
        splayTree.add(key, value);
      } else if (command == "set") {
        splayTree.set(key, value);
      } else if (command == "delete") {
        splayTree.erase(key);
      } else if (command == "search") {
        try {
          auto value_ = splayTree.search(key);
          std::cout << 1 << " " << value_ << std::endl;
        } catch (std::logic_error &e) {
          std::cout << "0" << std::endl;
        }
      } else if (command == "min") {
        auto [key_, value_] = splayTree.min();
        std::cout << key_ << " " << value_ << std::endl;
      } else if (command == "max") {
        auto [key_, value_] = splayTree.max();
        std::cout << key_ << " " << value_ << std::endl;
      } else if (command == "print") {
        splayTree.print(std::cout);
      } else {
        std::cout << "error" << std::endl;
      }
    } catch (std::logic_error &e) {
      std::cout << "error" << std::endl;
    }
  }

  return 0;
}