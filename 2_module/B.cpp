#include <iostream>

template <typename K, typename T>
struct Node {
    K key;
    T value;
    Node* right;
    Node* left;
};

template <typename K, typename T>
class SplayTree {
private:
    void Clear(Node<K, T>* buffer) noexcept {
        if (buffer != nullptr) {
            if (buffer->left != nullptr || buffer->value != nullptr) {
                if (buffer->left != nullptr && buffer->right == nullptr) {
                    Clear(buffer->left);
                    delete buffer;
                } else if (buffer->left == nullptr && buffer->right != nullptr) {
                    Clear(buffer->right);
                    delete buffer;
                } else {
                    Clear(buffer->left);
                }
            } else {
                delete buffer;
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
    void Splay(const K& key) noexcept;
    void Print() const noexcept;

private:
    Node<K, T>* root_;
};


int main() {

    return EXIT_SUCCESS;
}
