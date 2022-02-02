#include <iostream>
#include <stdexcept>
#include <queue>
#include <ostream>

using std::string;
using std::cout;
using std::cin;
using std::endl;


struct key_value {
    long long key;
    string value;
};

class splay_tree {
public:
    splay_tree() : root(nullptr) {}

    ~splay_tree() {
        clear(root);
    }


    void add(const key_value& target) {
        if (!root) {
            root = new node(target.key, target.value);
            return;
        }
        node* current = root;
        for (;;) {
            if (target.key < current->key) {
                if (!current->left) {
                    node* new_node = new node(target.key, target.value);
                    current->left = new_node;
                    new_node->parent = current;
                    splay(new_node);
                    root = new_node;
                    return;
                } else {
                    current = current->left;
                }
            }
            else if (target.key > current->key) {
                if (!current->right) {
                    node* new_node = new node(target.key, target.value);
                    current->right = new_node;
                    new_node->parent = current;
                    splay(new_node);
                    root = new_node;
                    return;
                } else {
                    current = current->right;
                }
            } else {
                splay(current);
                throw std::logic_error("not found");
            }
        }

    }

    void set(const key_value& target) {
        if (!root) {
            throw std::logic_error("empty tree");
        }
        node* tmp = find(target.key);
        if (!tmp) {
            throw std::logic_error("not found");
        }
        tmp->value = target.value;
        splay(tmp);
    }

    void erase(const long long key) {
        if (!root) {
            throw std::logic_error("empty tree");
        }
        node* target = find(key);
        if (!target) {
            throw std::logic_error("not found");
        }

        else if (target->left && target->right) {
            node* tmp = target->left;
            while (tmp->right) {
                tmp = tmp->right;
            }

            splay(tmp);
            splay(target);

            root = target->left;
            root->right = target->right;
            target->right->parent = root;
            root->parent = nullptr;

        }
        else if (!target->left && target->right) {
            root = target->right;
            root->parent = nullptr;
        }
        else if (target->left && !target->right) {
            root = target->left;
            root->parent = nullptr;
        }
        else {
            root = nullptr;
        }

        delete target;
    }

    key_value search(const long long key) {
        node* ptr_node = find(key);
        if (ptr_node == nullptr) {
            throw std::logic_error("not found");
        }
        key_value result;
        result.key = ptr_node->key;
        result.value = ptr_node->value;
        return result;
    }

    key_value min() {
        if (!root) {
            throw std::logic_error("empty tree");
        }

        node* current = root;
        key_value result;
        for (;;) {
            if (current->left) {
                current = current->left;
            }
            else {
                splay(current);
                result.value = current->value;
                result.key = current->key;
                return result;
            }
        }
    }

    key_value max() {
        if (!root) {
            throw std::logic_error("empty tree");
        }

        node* current = root;
        key_value result;

        for (;;) {
            if (current->right) {
                current = current->right;
            }
            else {
                splay(current);
                result.value = current->value;
                result.key = current->key;
                return result;
            }
        }
    }

    friend void print(std::ostream&, const splay_tree&);

protected:
    struct node {
    public:
        long long key;
        string value;
        node* parent;
        node* right;
        node* left;

        node(long long _key, const string&  _value) :
                key(_key), value(_value), parent(nullptr), right(nullptr), left(nullptr) {}

        node(const node& node) = default;
    };
    node* root;

private:
    void clear(node* target) {
        if (target) {
            clear(target->left);
            clear(target->right);

            delete target;
        }
    }

    void left_rotate(node* target) {
        node* tmp = target->right;
        target->right = tmp->left;
        if (tmp->left) {
            tmp->left->parent = target;
        }
        tmp->parent = target->parent;
        if (!target->parent) {
            root = tmp;
        }
        else if (target == target->parent->left) {
            target->parent->left = tmp;
        }

        else {
            target->parent->right = tmp;
        }
        tmp->left = target;
        target->parent = tmp;
    }

    void right_rotate(node* target) {
        node* tmp = target->left;
        target->left = tmp->right;
        if (tmp->right) {
            tmp->right->parent = target;
        }
        tmp->parent = target->parent;
        if (!target->parent) {
            root = tmp;
        }
        else if (target == target->parent->right) {
            target->parent->right = tmp;
        }
        else {
            target->parent->left = tmp;
        }
        tmp->right = target;
        target->parent = tmp;
    }

    void zig(node* target) {
        node* parent = target->parent;
        if (parent->left == target) {
            right_rotate(target->parent);
        }
        else {
            left_rotate(target->parent);
        }
    }

    void zig_zig(node* target) {
        node *parent = target->parent;
        node *grandparent = parent->parent;

        if (parent->left == target) {
            right_rotate(grandparent);
            right_rotate(parent);
        }
        else {
            left_rotate(grandparent);
            left_rotate(parent);
        }
    }


    void zig_zag(node* target) {
        node* parent = target->parent;

        if (parent->right == target) {
            left_rotate(parent);
            right_rotate(target->parent);
        }
        else {
            right_rotate(parent);
            left_rotate(target->parent);
        }
    }

    void splay(node* target) {
        while (target->parent) {
            node* parent = target->parent;
            node* grandparent = parent->parent;
            if (!grandparent) {
                zig(target);
            }
            else if (grandparent->left == parent && parent->left == target) {
                zig_zig(target);
            }
            else if (grandparent->right == parent && parent->right == target) {
                zig_zig(target);
            }
            else {
                zig_zag(target);
            }
        }
        root = target;
    }

    node* find(const long long key) {
        if (!root) {
            throw std::logic_error("empty tree");
        }
        if (root->key == key) {
            return root;
        }
        node* current = root;
        for (;;) {
            if (key < current->key) {
                if (!current->left) {
                    splay(current);
                    return nullptr;
                }
                else {
                    current = current->left;
                }
            }
            else if (key > current->key) {
                if (!current->right) {
                    splay(current);
                    return nullptr;
                }
                else {
                    current = current->right;
                }
            }
            else {
                splay(current);
                return current;
            }
        }
    }
};

void print(std::ostream &out, const splay_tree& tree) {
    if (!tree.root) {
        out << "_" << endl;
        return;
    }
    out << "[" << tree.root->key << " " << tree.root->value << "]" << endl;
    std::queue<splay_tree::node *> q;

    q.push(tree.root->left);
    q.push(tree.root->right);

    splay_tree::node *current;

    size_t counter = 0;
    size_t number = 2;
    string str;

    while (current = q.front(), !q.empty()) {
        q.pop();
        if (current) {
            str += "[" + std::to_string(current->key) + " " + current->value + " " +
                   std::to_string(current->parent->key) + "]";
            q.push(current->left);
            q.push(current->right);
        }
        else {
            q.push(nullptr);
            q.push(nullptr);
            str += "_";
        }

        if (++counter, counter == number) {

            string str_null = "_";
            for (size_t j = 1; j < number; ++j) {
                str_null += " _";
            }

            if (str != str_null) {
                out << str << '\n';
                str.clear();
            }
            else {
                break;
            }
            number *= 2;
            counter = 0;
        }
        else {
            str += " ";
        }
    }
}


int main() {
    splay_tree tree;
    long long key;
    string buffer , command, value;
    while (!cin.eof()) {
        cin >> command;
        if (command.empty()) {
            continue;
        }
        else if (command == "add") {
            cin >> key >> value;
            try {
                tree.add({key, value});
            }
            catch (...) {
                cout << "error" << endl;
            }
        }
        else if (command == "set") {
            cin >> key >> value;
            try {
                tree.set({key, value});
            }
            catch (...) {
                cout << "error" << endl;
            }
        }
        else if (command == "delete") {
            cin >> key;
            try {
                tree.erase(key);
            }
            catch (...) {
                cout << "error" << endl;
            }
        }
        else if (command == "search") {
            cin >> key;
            try {
                key_value target = tree.search(key);
                cout << "1 " << target.value << endl;
            }
            catch (...) {
                cout << "0" << endl;
            }
        }
        else if (command == "min") {
            try {
                key_value target = tree.min();
                cout << target.key << " " << target.value << endl;
            }
            catch (...) {
                cout << "error" << endl;
            }
        }
        else if (command == "max") {
            try {
                key_value target = tree.max();
                cout << target.key << " " << target.value << endl;
            }
            catch (...) {
                cout << "error" << endl;
            }
        }
        else if (command == "print") {
            print(cout, tree);
        }
        command.clear();
    }
    return 0;
}