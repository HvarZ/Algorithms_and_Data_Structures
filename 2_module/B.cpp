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

#define ZIG(direction, reverseDirection)        \
auto* tmp = node->direction;                    \
node->parent = nullptr;                         \
node->direction = parent;                       \
parent->parent = node;                          \
parent->reverseDirection = tmp;                 \
if (tmp != nullptr) {                           \
tmp->parent = parent;                           \
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
    void Splay(const K& key) noexcept;
    void Print() const noexcept;

private:
    Node<K, T>* root_;
};


int main() {

    return EXIT_SUCCESS;
}
