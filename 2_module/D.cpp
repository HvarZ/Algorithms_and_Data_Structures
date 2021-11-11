#include <utility>
#include <unordered_map>
#include <iostream>
#include <set>

struct Node {
    using keyChild = std::pair<wchar_t, Node*>;

    std::unordered_map<wchar_t, Node*> children_;
    bool isEnd_;

    Node(bool isEnd) noexcept : isEnd_(isEnd) {}
    Node(const keyChild& child, bool isEnd) : isEnd_(false) {
        children_.insert(child);
    }
};

class Trie {
public:
    Trie() : root_(nullptr), isEmpty_(true) {}
    ~Trie() {
        if (root_) {
            Clear(root_);
        }
    }

    void Add(const std::wstring& str) {
        if (str.empty()) {
            return;
        }
        if (isEmpty_) {
            root_ = new Node(false);
            isEmpty_ = false;
        }
        decltype(root_) tmp = root_;
        size_t i = 0;
        for (const auto& symbol : str) {
            if (tmp->children_.count(symbol) == 0) {
                if (i == str.size() - 1) {
                    tmp->children_[symbol] = new Node(true);
                } else {
                    tmp->children_[symbol] = new Node(false);
                }
            }
            tmp = tmp->children_[symbol];
            ++i;
        }
    }

    auto Search(const std::wstring& str) const noexcept -> bool {
        auto tmp = root_;
        for (const auto& symbol : str) {
            if (tmp->children_.count(symbol) == 0) {
                return false;
            }
            tmp = tmp->children_.at(symbol);
        }
        return tmp->isEnd_;
    }

    void getCorrections(std::set<std::wstring>& result, const std::wstring& str) const noexcept {

    }



private:
    void Clear(Node* node) {
        for (auto& i : node->children_) {
            Clear(i.second);
        }
        delete node;
    }

private:
    Node* root_;
    bool isEmpty_;
};

int main() {
    Trie prefixTree;
    std::wstring word, word1;
    std::wcin >> word;
    prefixTree.Add(word);
    std::wcin >> word1;
    prefixTree.Add(word1);
    std::wcout << prefixTree.Search(word) << prefixTree.Search(word1) << std::endl;
    return 0;
}