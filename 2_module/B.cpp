#include <iostream>
#include <stdexcept>
#include <utility>
#include <queue>
#include <string>
#include <algorithm>

template <typename K, typename T>
struct Node {
public:
    K key;
    T value;
    Node* parent;
    Node* right;
    Node* left;

public:
    explicit Node(const K& _key, const T&  _value) noexcept : key(_key), value(_value),
                                                              parent(nullptr), right(nullptr), left(nullptr) {}

    Node(const Node& _node) : key(_node.key), value(_node.value),
                              parent(_node.parent), right(_node.right), left(_node.left) {}

    Node& operator=(const Node& _node) {
        if (this == &_node) {
            throw std::runtime_error("self - assignment error");
        }
        key = _node.key;
        value = _node.value;
        parent = _node.parent;
        right = _node.right;
        left = _node.left;
        return *this;
    }
};


#define INFINITY while (true)

#define ZIG(direction, reverseDirection)            \
    auto* tmp = node->direction;                    \
    node->parent = nullptr;                         \
    node->direction = parent;                       \
    parent->parent = node;                          \
    parent->reverseDirection = tmp;                 \
    if (tmp != nullptr) {                           \
        tmp->parent = parent;                       \
    }


#define ZIG_ZIG(direction, reverseDirection)        \
    auto* tmp = node->direction;                    \
    auto* tmp2 = parent->direction;                 \
                                                    \
    node->parent = grandparent->parent;             \
    node->direction = parent;                       \
                                                    \
    parent->parent = node;                          \
    parent->reverseDirection = tmp;                 \
    parent->direction = grandparent;                \
                                                    \
    grandparent->parent = parent;                   \
    grandparent->reverseDirection = tmp2;           \
                                                    \
    if (node->parent != nullptr) {                  \
        if (node->parent->left == grandparent) {    \
            node->parent->left = node;              \
        } else {                                    \
            node->parent->right = node;             \
        }                                           \
    }                                               \
                                                    \
    if (tmp != nullptr) {                           \
        tmp->parent = parent;                       \
    }                                               \
                                                    \
    if (tmp2 != nullptr) {                          \
        tmp2->parent = grandparent;                 \
    }

#define ZIG_ZAG(direction, reverseDirection)            \
    auto* tmp = node->reverseDirection;                 \
    auto* tmp2 = node->direction;                       \
                                                        \
    node->parent = grandparent->parent;                 \
    node->reverseDirection = parent;                    \
    node->direction = grandparent;                      \
                                                        \
    parent->parent = node;                              \
    parent->direction = tmp;                            \
                                                        \
    grandparent->parent = node;                         \
    grandparent->reverseDirection = tmp2;               \
                                                        \
    if (node->parent != nullptr) {                      \
        if (node->parent->left == grandparent) {        \
            node->parent->left = node;                  \
        } else {                                        \
            node->parent->right = node;                 \
        }                                               \
    }                                                   \
                                                        \
    if (tmp != nullptr) {                               \
        tmp->parent = parent;                           \
    }                                                   \
                                                        \
    if (tmp2 != nullptr) {                              \
        tmp2->parent = grandparent;                     \
    }

#define ADD_NODE(direction)                             \
    auto* new_node = new Node<K, T>(key, value);        \
    current->direction = new_node;                      \
    new_node->parent = current;                         \
    Splay(new_node);                                    \
    root_ = new_node;

#define DELETE_NODE(direction)                          \
    root_ = target->direction;                          \
    root_->parent = nullptr;

template <typename K, typename T>
class SplayTree {
private:
    void Clear(Node<K, T>* node) noexcept {
        if (node != nullptr) {
            Clear(node->left);
            Clear(node->right);

            delete node;
        }
    }

    void Zig(Node<K, T>* node) noexcept {
        auto* parent = node->parent;
        if (parent->left == node) {
            ZIG(right, left)
        } else {
            ZIG(left, right)
        }
    }

    void ZigZig(Node<K, T>* node) noexcept {
        auto *parent = node->parent;
        auto *grandparent = parent->parent;

        if (parent->left == node) {
            ZIG_ZIG(right, left)
        } else {
            ZIG_ZIG(left, right)
        }
    }


    void ZigZag(Node<K, T>* node) noexcept {
        auto* parent = node->parent;
        auto* grandparent = parent->parent;

        if (parent->right == node) {
            ZIG_ZAG(right, left)
        } else {
            ZIG_ZAG(left, right)
        }
    }

    void Splay(Node<K, T>* node) noexcept {
        while (node->parent != nullptr) {
            auto* parent = node->parent;
            auto* grandparent = parent->parent;
            if (grandparent == nullptr) {
                Zig(node);
            }
            else if ((grandparent->left == parent && parent->left == node) ||
                     (grandparent->right == parent && parent->right == node)) {
                ZigZig(node);
            }
            else {
                ZigZag(node);
            }
        }
        root_ = node;
    }

    [[nodiscard]] auto FindNode(const K& key) -> Node<K, T>* {
        if (IsEmpty()) {
            throw std::runtime_error("error");
        }
        if (root_->key == key) {
            return root_;
        }
        auto* current = root_;
        INFINITY {
            if (key > current->key) {
                if (current->right == nullptr) {
                    Splay(current);
                    return nullptr;
                } else {
                    current = current->right;
                }
            } else if (key < current->key) {
                if (current->left == nullptr) {
                    Splay(current);
                    return nullptr;
                } else {
                    current = current->left;
                }
            } else {
                Splay(current);
                return current;
            }
        }
    }


public:
    SplayTree() : root_(nullptr) {}

    ~SplayTree() {
        Clear(root_);
    }

    [[nodiscard]] auto IsEmpty() const noexcept -> bool {
        return root_ == nullptr;
    }

    void AddNode(const K& key, const T& value) {
        if (root_ == nullptr) {
            root_ = new Node<K, T>(key, value);
            return;
        }
        auto* current = root_;
        INFINITY {
            if (key < current->key) {
                if (current->left == nullptr) {
                    ADD_NODE(left)
                    return;
                } else {
                    current = current->left;
                }
            } else if (key > current->key) {
                if (current->right == nullptr) {
                    ADD_NODE(right)
                    return;
                } else {
                    current = current->right;
                }
            } else {
                Splay(current);
                throw std::runtime_error("error");
            }
        }

    }

    void SetNode(const K& key, const T& value) {
        if (IsEmpty()) {
            throw std::runtime_error("error");
        }
        auto target = FindNode(key);
        if (target == nullptr) {
            throw std::runtime_error("error");
        }
        target->value = value;
        Splay(target);
    }

    void DeleteNode(const K& key) {
        if (IsEmpty()) {
            throw std::runtime_error("error");
        }
        auto* target = FindNode(key);
        if (target == nullptr) {
            throw std::runtime_error("error");
        }

        else if (target->left != nullptr && target->right != nullptr) {
            auto leftMaxChildTarget = target->left;
            while (leftMaxChildTarget->right != nullptr) {
                leftMaxChildTarget = leftMaxChildTarget->right;
            }
            Splay(leftMaxChildTarget);
            Splay(target);

            root_ = target->left;
            root_->right = target->right;
            target->right->parent = root_;
            root_->parent = nullptr;

        } else if (target->left == nullptr && target->right != nullptr) {
            DELETE_NODE(right)


        } else if (target->left != nullptr && target->right == nullptr) {
            DELETE_NODE(left)

        } else {
            root_ = nullptr;
        }

        delete target;
    }

    [[nodiscard]] auto SearchNode(const K& key) -> std::pair<K, T> {
        auto target = FindNode(key);
        if (target == nullptr) {
            throw std::runtime_error("error");
        }
        return std::make_pair(target->key, target->value);
    }

    [[nodiscard]] auto GetMin() -> std::pair<K, T> {
        if (IsEmpty()) {
            throw std::runtime_error("error");
        }

        auto* current = root_;

        INFINITY {
            if (current->left == nullptr) {
                Splay(current);
                return std::make_pair(current->key, current->value);
            } else {
                current = current->left;
            }
        }
    }

    [[nodiscard]] auto GetMax() -> std::pair<K, T> {
        if (IsEmpty()) {
            throw std::runtime_error("error");
        }

        auto* current = root_;

        INFINITY {
            if (current->right == nullptr) {
                Splay(current);
                return std::make_pair(current->key, current->value);
            } else {
                current = current->right;
            }
        }
    }

    void Print() const noexcept {
        if (root_ == nullptr) {
            std::cout << "_\n";
            return;
        }
        std::cout << "[" << std::to_string(root_->key) << " "
                  << root_->value << "]\n";
        std::queue<decltype(root_)> q;

        q.push(root_->left);
        q.push(root_->right);

        decltype(root_) current;

        size_t elementCounter = 0;
        size_t numberElements = 2;
        std::string line;

        while (!q.empty()) {
            current = q.front();
            q.pop();
            if (current != nullptr) {
                line += "[" + std::to_string(current->key) + " " + current->value
                        + " " + std::to_string(current->parent->key) + "]";
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
                    std::cout << line << '\n';
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
    Node<K, T>* root_;
};


template <typename K, typename T>
void Handler(SplayTree<K, T>& tree) {
    std::string buffer , command, value;
    int64_t key;
    while (std::cin >> command) {
        if (command.empty()) {
            continue;
        } else if (command == "add") {
            std::cin >> key >> value;
            try {
                tree.AddNode(key, value);
            } catch (std::runtime_error& e) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "set") {
            std::cin >> key >> value;
            try {
                tree.SetNode(key, value);
            } catch (std::runtime_error& e) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "delete") {
            std::cin >> key;
            try {
                tree.DeleteNode(key);
            } catch (std::exception& e) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "search") {
            std::cin >> key;
            try {
                auto target = tree.SearchNode(key);

                std::cout << "1 " << target.second << std::endl;

            } catch (std::runtime_error& e) {
                std::cout << "0" << std::endl;
            }
        } else if (command == "min") {
            try {
                auto target = tree.GetMin();
                std::cout << target.first << " " << target.second << std::endl;
            } catch (std::runtime_error& e) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "max") {
            try {
                auto target = tree.GetMax();
                std::cout << target.first << " " << target.second << std::endl;
            } catch (std::runtime_error& e) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "print") {
            tree.Print();
        }
    }
}


int main() {
    SplayTree<int64_t, std::string> tree;
    Handler(tree);

    return EXIT_SUCCESS;
}