#include <iostream>

template <typename K, typename T>
struct Node {
public:
    K key;
    T value;
    Node* parent;
    Node* right;
    Node* left;

public:
    explicit Node(const K& _key, const T& _value) noexcept : key(_key), value(_value),
         parent(nullptr), right(nullptr), left(nullptr) {}
};

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
    parent->right = tmp;                                \
                                                        \
    grandparent->parent = node;                         \
    grandparent->left = tmp2;                           \
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

    void Splay(const Node<K, T>* node) noexcept {
        while (node->parent != nullptr) {
            auto* parent = node->parent;
            auto* grandparent = parent->parent;
            if (grandparent == nullptr) {
                zig(node);
            }
            else if ((grandparent->left == parent && parent->left == node) ||
                     (grandparent->right == parent && parent->right == node)) {
                zig_zig(node);
            }
            else {
                zig_zag(node);
            }
        }
    }


public:
    SplayTree() : root_(nullptr) {}

    ~SplayTree() {
        Clear(root_);
    }

    void AddNode(const K& key, const T& value) noexcept {

    }

    void SetNode(const K& key, const T& value) noexcept;
    void DeleteNode(const K& key) noexcept;
    auto SearchNode(const K& key) const -> Node<K, T>;
    [[nodiscard]] auto GetMin() const -> Node<K, T>;
    [[nodiscard]] auto GetMax() const -> Node<K, T>;
    void Print() const noexcept;

private:
    Node<K, T>* root_;
};


int main() {
    SplayTree<int64_t, std::string> tree;
    return EXIT_SUCCESS;
}
