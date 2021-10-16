#include <iostream>
#include <stdexcept>
#include <utility>
#include <queue>
#include <string>

template <typename K, typename T>
struct Node {
public:
    K key;
    T value;
    Node* parent;
    Node* right;
    Node* left;

public:
    explicit Node(const K& _key, T  _value) noexcept : key(_key), value(std::move(_value)),
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
    auto* new_node = new Node(node);                    \
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
        if (node == nullptr) return;
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
        } else {
            if (node->left != nullptr && node->right == nullptr) {
                Clear(node->left);
                delete node;
            } else if (node->left == nullptr && node->right != nullptr) {
                Clear(node->right);
                delete node;
            } else {
                Clear(node->left);
            }
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


public:
    SplayTree() : root_(nullptr) {}

    ~SplayTree() {
        Clear(root_);
    }

    [[nodiscard]] auto IsEmpty() const noexcept -> bool {
        return root_ == nullptr;
    }

    void AddNode(const Node<K, T>& node) noexcept {
        if (root_ == nullptr) {
            root_ = new Node(node.key, node.value);
            return;
        }
        auto* current = root_;
        INFINITY {
            if (node.key < current->key) {
                if (current->left == nullptr) {
                    ADD_NODE(left)
                    return;
                } else {
                    current = current->left;
                }
            } else if (node.key > current->key) {
                if (current->right == nullptr) {
                    ADD_NODE(right)
                    return;
                } else {
                    current = current->right;
                }
            } else {
                Splay(current);
                return;
            }
        }

    }

    void SetNode(const Node<K, T>& node) {
        if (IsEmpty()) {
            throw std::runtime_error("error");
        }
        auto target = SearchNode(node.key);
        target->value = node.value;
        Splay(target);
    }

    void DeleteNode(const K& key) {
        auto* target = SearchNode(key);
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

    [[nodiscard]] auto SearchNode(const K& key) noexcept -> Node<K, T>* {
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

    [[nodiscard]] auto GetMin() -> Node<K, T>& {
        if (IsEmpty()) {
            throw std::runtime_error("error");
        }

        auto* current = root_;

        INFINITY {
            if (current->left == nullptr) {
                return *current;
            } else {
                current = current->left;
            }
        }
    }

    [[nodiscard]] auto GetMax() -> Node<K, T>& {
        if (IsEmpty()) {
            throw std::runtime_error("error");
        }

        auto* current = root_;

        INFINITY {
            if (current->right == nullptr) {
                return *current;
            } else {
                current = current->right;
            }
        }
    }

    [[nodiscard]] auto GetRoot() const noexcept -> Node<K, T>* {
        return root_;
    }

private:
    Node<K, T>* root_;
};


template <typename K, typename T>
void Print(const SplayTree<K, T>& tree) noexcept {
    if (tree.GetRoot() == nullptr) {
        std::cout << "_\n";
        return;
    }
    std::cout << "[" << std::to_string(tree.GetRoot()->key) << " "
                     << tree.GetRoot()->value << "]\n";
    std::queue<decltype(tree.GetRoot())> q;

    q.push(tree.GetRoot()->left);
    q.push(tree.GetRoot()->right);

    decltype(tree.GetRoot()) current;

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

            std::string null_line = "_";
            for (size_t j = 1; j < numberElements; ++j)
                null_line += " _";

            if (line != null_line) {
                std::cout << line << '\n';
                line.clear();
            } else {
                break;
            }
            numberElements *= 2;
            elementCounter = 0;
        } else {
            line += " ";
        }
    }
}

int main() {
    SplayTree<int64_t, std::string> tree;
    tree.AddNode(Node<int64_t, std::string>(8, "10"));
    tree.AddNode(Node<int64_t, std::string>(4, "14"));
    tree.AddNode(Node<int64_t, std::string>(7, "15"));
    tree.SetNode(Node<int64_t, std::string>(8, "11"));
    tree.AddNode(Node<int64_t, std::string>(3, "13"));
    tree.AddNode(Node<int64_t, std::string>(5, "16"));

    auto tmp = tree.SearchNode(88);
    tmp = tree.SearchNode(7);

    tree.DeleteNode(5);

    Print(tree);

    return EXIT_SUCCESS;
}
