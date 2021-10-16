#include <iostream>
#include <stdexcept>
#include <utility>

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

    void SetNode(const Node<K, T>& node) noexcept;
    void DeleteNode(const K& key) noexcept;

    [[nodiscard]] auto SearchNode(const K& key) noexcept -> Node<K, T>& {
        if (root_->key == key) {
            return *root_;
        }
        auto* current = root_;
        INFINITY {
            if (key > current->key) {
                if (current->right == nullptr) {
                    Splay(current);
                    return *current;
                } else {
                    current = current->right;
                }
            } else if (key < current->key) {
                if (current->left == nullptr) {
                    Splay(current);
                    return *current;
                } else {
                    current = current->left;
                }
            } else {
                Splay(current);
                return *current;
            }
        }
    }

    [[nodiscard]] auto GetMin() -> Node<K, T>;
    [[nodiscard]] auto GetMax() -> Node<K, T>;
    void Print() const noexcept;

private:
    Node<K, T>* root_;
};


int main() {
    SplayTree<int64_t, std::string> tree;
    tree.AddNode(Node<int64_t, std::string>(1, "10"));
    tree.AddNode(Node<int64_t, std::string>(2, "14"));
    tree.AddNode(Node<int64_t, std::string>(7, "15"));
    tree.AddNode(Node<int64_t, std::string>(4, "13"));
    tree.AddNode(Node<int64_t, std::string>(5, "16"));
    tree.AddNode(Node<int64_t, std::string>(1, "16"));

    std::cout << tree.SearchNode(7).value << std::endl;

    return EXIT_SUCCESS;
}
