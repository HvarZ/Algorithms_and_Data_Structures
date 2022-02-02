#include <algorithm>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <string_view>
#include <vector>

#define OFF false

using std::getline;
using std::locale;
using std::min;
using std::set;
using std::vector;
using std::wcin;
using std::wcout;
using std::wstring;
using std::wstring_view;

using result_pack = std::optional<set<wstring>>;
using set_w = set<wstring>;
using trace = vector<size_t>;

wstring to_lowercase(const wstring &str) {
    locale locale_{""};
    wstring result;
    for (const auto &symbol : str) {
        result.push_back(std::tolower(symbol, locale_));
    }
    return result;
}

struct node_trie {
    explicit node_trie(wstring key_, bool end_ = false) {
        key = std::move(key_);
        brother = nullptr;
        son = nullptr;
        is_end = end_;
    }

    node_trie *brother;
    node_trie *son;
    wstring key;
    bool is_end;
};

class trie final {
public:
    trie() {
        root = nullptr;
    }

    ~trie() {
        if (root) {
            clear(root);
        }
    }

    [[nodiscard]] result_pack get_correct(const wstring &word) const {
        auto goal = to_lowercase(word);
        trace curr;
        for (size_t i = 0; i < goal.size() + 2; ++i) {
            curr.push_back(i);
        }

        if (find(root, word)) {
            return result_pack();
        }
        set_w result;

        wstring curr_word;
        trace prev;
        auto *node = root;
        while (node) {
            correct(node, node->key, goal, prev, curr, result, curr_word);
            node = node->brother;
        }
        return std::make_optional(result);
    }

    void insert(const wstring &word) {
        auto result = to_lowercase(word);
        root = insert(root, result);
    }

private:
    node_trie *root;

private:
    // Функция поиска реализована для проверки наличия слова в префиксном дереве.
    // wstring_view нужна для исключения дополнительного копирования при рекурсии
    node_trie* find(node_trie* t, wstring_view x, size_t n = 0) const {
        if (!n) {
            n = x.size();
        }
        if (!t) {
            return nullptr;
        }
        auto k = prefix(x.data(), t->key);
        if (k == 0) {
            return find(t->brother, x, n);
        }
        if (x == t->key && t->is_end) {
            return t;
        }
        if( k == t->key.size() ) {\
            x.remove_prefix(k);
            return find(t->son, x , n - k);
        }
        return nullptr;
    }

    // Сложность добавления O(n*m), где n - длина слова, m - количество узлов в дереве
    // В отличии от классического префикного дерева, где добавление имеет линейное время
    // в данном случае это не работает, потому что у нас нет константного доступа к детям

    node_trie *insert(node_trie *node, const wstring &word, size_t count = 0) {
        auto goal = to_lowercase(word);
        if (count == 0) {
            count = goal.size();
        }
        if (node == nullptr) {
            return new node_trie(goal, true);
        }
        auto k = prefix(goal, node->key);
        if (k == 0) {
            node->brother = insert(node->brother, goal, count);
        } else if (k < count) {
            if (k < node->key.size()) {
                split(node, k);
                node->is_end = false;
            }

            node->son =
                    insert(node->son, wstring(word.begin() + k, word.end()), count - k);
        } else if (k == count) {
            if (k < node->key.size()) {
                split(node, k);
            }
            node->is_end = true;
        }
        return node;
    }

    // Сложность O(n*m), где n - длина слова, m - количество узлов в дереве
    // Объясняется это рекурсией, которая проходит по всем узлам в дереве если не найдется 2 ошибки

    void correct(const node_trie *node, const wstring &letters,
                 const wstring &word, trace row,
                 trace previous, set<wstring> &result,
                 wstring curr_word) const {
        auto columns = word.size() + 2;
        trace curr;
        for (size_t i = 0; i < letters.size(); ++i) {
            curr_word.push_back(letters[i]);
            curr.clear();
            curr.push_back(previous[0] + 1);

            for (size_t column = 1; column < columns; ++column) {
                size_t insert_cost = curr[column - 1] + 1;
                size_t delete_cost = previous[column] + 1;

                size_t replace_cost;
                if (word[column - 1] != letters[i]) {
                    replace_cost = previous[column - 1] + 1;
                } else {
                    replace_cost = previous[column - 1];
                }

                curr.push_back(min(min(insert_cost, delete_cost), replace_cost));
                if (!row.empty() && column >= 2 &&
                    word[column - 2] == letters[i] &&
                    word[column - 1] == curr_word[curr_word.size() - 2]) {
                    curr[column] = min(curr[column], row[column - 2] + 1);
                }
            }

            if (1 >= curr[curr.size() - 2] && node->is_end &&
                i == letters.size() - 1) {
                result.insert(curr_word);
            }

            if (1 >= *min_element(curr.begin(), curr.end()) &&
                i != letters.size() - 1) {
                row = previous;
                previous = curr;
            } else {
                break;
            }
        }

        if (1 >= *min_element(curr.begin(), curr.end())) {
            node_trie *child_node = node->son;
            while (child_node) {
                correct(child_node, child_node->key, word, previous,
                        curr, result, curr_word);
                child_node = child_node->brother;
            }
        }
    }

    static size_t prefix(const wstring &str, const wstring &key) {
        for (unsigned int k = 0; k < str.size(); ++k) {
            if (k == key.size() || str[k] != key[k])
                return k;
        }
        return str.size();
    }

    static void split(node_trie *node, const size_t k) {
        auto *goal = new node_trie(
                wstring(node->key.begin() + k, node->key.end()), node->is_end);
        goal->son = node->son;
        node->son = goal;
        node->key = wstring(node->key.begin(), node->key.begin() + k);
    }

    void clear(node_trie *node) {
        if (node->brother) {
            clear(node->brother);
        }
        if (node->son) {
            clear(node->son);
        }
        delete node;
    }
};

void handler(std::wostream &out, const trie &tree) {
    wstring string;
    while (getline(wcin, string)) {
        if (string.empty()) {
            continue;
        }
        wcout << string;
        auto goal_str = to_lowercase(string);
        auto corrections = tree.get_correct(goal_str);
        if (!corrections.has_value()) {
            wcout << L" - ok\n";
        } else {
            auto value = corrections.value();
            if (value.empty()) {
                wcout << L" -?\n";
            } else {
                std::wstring str = L" ->";
                for (const auto &correction : value) {
                    str += L" " + correction + L",";
                }
                str.pop_back();
                wcout << str << L"\n";
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(OFF);
    locale loc{""};
    wcin.imbue(loc);
    wcout.imbue(loc);
    wstring string;
    trie tree;

    size_t mainCount = 0;
    wcin >> mainCount;
    wcin.ignore();

    for (size_t i = 0; i < mainCount; ++i) {
        getline(wcin, string);
        tree.insert(string);
    }

    handler(wcout, tree);
    return 0;
}