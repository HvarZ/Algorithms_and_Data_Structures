#include <iostream>
#include <string>
#include <queue>
#include <ostream>

using namespace std;

class splay_tree final {
public:
  splay_tree() : root(nullptr) {}
  ~splay_tree() { delete_tree(root); }


  string search(const long long key_) {
    auto *target = find(key_);
    Splay(target);
    if (target->key != key_)
      throw runtime_error("error");
    return target->date;
  }

  void add(const long long key_, const string &date_) {
    if (root == nullptr) {
      auto *new_node = new Node;
      new_node->key = key_;
      new_node->date = date_;
      root = new_node;
      return;
    }
    auto *tmp = find(key_);
    if (tmp->key == key_) {
      Splay(tmp);
      throw runtime_error("error");
    }
    auto *new_node = new Node;
    auto *parent_new_node = tmp;
    new_node->parent = parent_new_node;
    new_node->key = key_;
    new_node->date = date_;
    if (parent_new_node->key < key_)
      parent_new_node->right = new_node;
    else if (parent_new_node->key > key_)
      parent_new_node->left = new_node;
    Splay(new_node);
  }


  void set(const long long key_, const string &date_) {
    auto *target = find(key_);
    if (target->key != key_) {
      Splay(target);
      throw runtime_error("error");
    } else
      target->date = date_;
    Splay(target);
  }

  void erase(const long long key_) {
    auto *target = find(key_);
    Splay(target);
    if (target->key != key_)
      throw runtime_error("error");
    if (target == root && target->right == nullptr &&
        target->left == nullptr) {
      delete root;
      root = nullptr;
      return;
    }
    if (target->right == nullptr) {
      root = target->left;
      root->parent = nullptr;
      delete target;
      return;
    } else if (target->left == nullptr) {
      root = target->right;
      root->parent = nullptr;
      delete target;
      return;
    }
    root = target->left;
    root->parent = nullptr;
    auto *max = root;
    while (max->right != nullptr)
      max = max->right;
    Splay(max);
    root->right = target->right;
    root->right->parent = root;
    delete target;
  }

  pair<long long, string> min() {
    if (root == nullptr) {
      throw runtime_error("error");
    }
    auto *min_node = root;
    while (min_node->left != nullptr) {
      min_node = min_node->left;
    }
    Splay(min_node);
    return pair<long long, string>(min_node->key, min_node->date);
  }

  pair<long long, string> max() {
    if (root == nullptr) {
      throw runtime_error("error");
    }
    auto *max_node = root;
    while (max_node->right != nullptr) {
      max_node = max_node->right;
    }
    Splay(max_node);
    return pair<long long, string>(max_node->key, max_node->date);
  }

  friend void print(const splay_tree &tree, ostream& out);

protected:
  struct Node {
    Node *left;
    Node *right;
    Node *parent;
    string date;
    long long key;
    Node() : left(nullptr), right(nullptr), parent(nullptr), key(0) {}
  };
  Node *root;


private:
  void delete_tree(Node *node) {
    if (node != nullptr) {
      delete_tree(node->left);
      delete_tree(node->right);
      delete node;
    }
  }

  Node *find(const long long key_) {
    if (root == nullptr) {
      throw runtime_error("error");
    }
    auto *find_node = root;
    while (find_node->left != nullptr ||
           find_node->right != nullptr) {
      if (key_ == find_node->key) {
        return find_node;
      }
      if (key_ > find_node->key) {
        if (find_node->right == nullptr) {
          break;
        }
        find_node = find_node->right;
      } else if (key_ < find_node->key) {
        if (find_node->left == nullptr) {
          break;
        }
        find_node = find_node->left;
      }
    }
    return find_node;
  }
  void zag(Node *node_z) {
    Node *tmp = node_z->parent;
    node_z->parent = tmp->parent;
    if (node_z->parent == nullptr) {
      root = node_z;
    } else if (tmp->parent->left == tmp) {
      node_z->parent->left = node_z;
    } else if (tmp->parent->right == tmp) {
      node_z->parent->right = node_z;
    }
    tmp->parent = node_z;
    tmp->right = node_z->left;
    if (tmp->right != nullptr)
      tmp->right->parent = tmp;
    node_z->left = tmp;
  }

  void zig(Node *node_z) {
    Node *tmp = node_z->parent;
    node_z->parent = tmp->parent;
    if (node_z->parent == nullptr) {
      root = node_z;
    } else if (tmp->parent->left == tmp)
      node_z->parent->left = node_z;
    else if (tmp->parent->right == tmp)
      node_z->parent->right = node_z;
    tmp->parent = node_z;
    tmp->left = node_z->right;
    if (tmp->left != nullptr)
      tmp->left->parent = tmp;
    node_z->right = tmp;
  }
  void Splay(Node *node_) {
    if (node_ == nullptr)
      return;
    while (node_ != root) {
      if (node_->parent == root) {
        if (node_->parent->right == node_)
          zag(node_);
        else if (node_->parent->left == node_)
          zig(node_);
      } else if (node_->parent->left == node_) {
        if (node_->parent->parent->left == node_->parent) {
          zig(node_->parent);
          zig(node_);
        } else if (node_->parent->parent->right == node_->parent) {
          zig(node_);
          zag(node_);
        }
      } else if (node_->parent->right == node_) {
        if (node_->parent->parent->left == node_->parent) {
          zag(node_);
          zig(node_);
        } else if (node_->parent->parent->right == node_->parent) {
          zag(node_->parent);
          zag(node_);
        }
      }
    }
  }
};

void print(const splay_tree &tree, ostream& out) {
  queue<splay_tree::Node *> q;
  q.push(tree.root);
  while (!q.empty()) {
    queue<splay_tree::Node *> children = q;
    string level;
    bool flag = false;
    while (!children.empty()) {
      auto node = children.front();
      children.pop();
      if (node == nullptr) {
        q.push(nullptr);
        q.push(nullptr);
        level += "_ ";
      } else {
        if (node == tree.root) {
          level += "[" + to_string(node->key) + " ";
          level += node->date + "] ";
        }

        else {
          level += "[" + to_string(node->key) + " ";
          level += node->date;
          level += " " + to_string(node->parent->key) + "] ";
        }
        if (node->left != nullptr) {
          q.push(node->left);
          flag = true;
        } else
          q.push(nullptr);
        if (node->right != nullptr) {
          q.push(node->right);
          flag = true;
        } else
          q.push(nullptr);
      }
      q.pop();
    }
    level.pop_back();
    out << level << "\n";
    if (!flag) {
      return;
    }
  }
}

int main() {
  splay_tree tree;
  while (!cin.eof()) {
    string string_;
    string date;
    long long key;
    cin >> string_;
    try {
      if (string_.empty()) {
        continue;
      } else if (string_ == "add") {
        cin >> key >> date;
        tree.add(key, date);
      } else if (string_ == "set") {
        cin >> key >> date;
        tree.set(key, date);
      } else if (string_ == "delete") {
        cin >> key;
        tree.erase(key);
      } else if (string_ == "search") {
        cin >> key;
        try {
          string search = tree.search(key);
          cout << "1 " << search << "\n";
        } catch (...) {
          cout << "0\n";
        }
      } else if (string_ == "min") {
        pair<long long, string> min = tree.min();
        cout << min.first << " " << min.second << "\n";
      } else if (string_ == "max") {
        pair<long long, string> max = tree.max();
        cout << max.first << " " << max.second << "\n";
      } else if (string_ == "print") {
        print(tree, cout);
      } else {
        cout << "error\n";
      }
    } catch (...) {
      cout << "error\n";
    }
    string_.clear();
  }
  return 0;
}