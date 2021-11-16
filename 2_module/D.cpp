#include <iostream>
#include <locale>
#include <set>
#include <string>
#include <unordered_map>
#include <optional>

auto ToLowerCase(const std::wstring &string) noexcept -> std::wstring {
  thread_local std::locale loc{""};
  std::wstring result;
  for (const auto& symbol : string) {
    result += std::tolower(symbol, loc);
  }
  return result;
}

class Trie {
public:
  Trie() : root_(new Node) {}
  ~Trie() {
    Clear(root_);
  }

  // Добавление имеет сложность  O(n), n - длина слова. Из-за цикла for, в котором
  // ровно n итерация (длины слова)
  void Add(const std::wstring &string) noexcept {
    if (string.empty()) {
      return;
    }
    auto tmp = root_;
    std::wstring str = ToLowerCase(string);
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
      if (i == str.size() - 1) {
        tmp->isEnd_ = true;
      }
      ++i;
    }
  }

  // Сложность поиска тоже линейная. Т.к в худшем случае, если слово в словаре есть,
  // то итераций ровно n
  [[nodiscard]] auto Search(const std::wstring &string) const noexcept -> bool {
    auto *node = root_;
    for (auto key_ : string) {
      if (node->children_.count(key_) != 0) {
        node = node->children_.at(key_);
      }
      else {
        return false;
      }
    }
    return node->isEnd_;
  }

  // Поиск похожих слов имеет сложность O(n*m), n - длина слова, m - количество узлов в дереве
  // Т.к нам мы рекурисвно проходимся по дереву
  auto GetCorrections(std::wstring &string) const noexcept -> std::optional<std::set<std::wstring>> {
    std::set<std::wstring> result;
    auto str = ToLowerCase(string);
    if (Search(str)) {
      return std::optional<std::set<std::wstring>>();
    }
    std::set<std::wstring> corrections;
    std::wstring correction;
    RecursiveReplace(str, corrections, root_, correction);
    return std::make_optional(corrections);
  }

private:
  struct Node {
    std::unordered_map<wchar_t, Node *> children_;
    bool isEnd_ = false;
    Node() : isEnd_(false) {}
    explicit Node(bool isEnd) noexcept : isEnd_(isEnd) {}
  };
  Node *root_;


private:
  void RecursiveReplace(const std::wstring &string,
                        std::set<std::wstring> &corrections, Node *currentNode,
                        const std::wstring &correction,
                        const size_t counter = 0, size_t errorCount = 0) const noexcept {

    if (errorCount == 2) {
      return;
    } else if (errorCount == 1 && counter == string.size() && currentNode->isEnd_ == true) {
      corrections.insert(correction);
      return;
    } else if (errorCount == 0 && counter == string.size() - 1 && currentNode->isEnd_ == true) {
      corrections.insert(correction);
    } else if (errorCount == 0 && counter == string.size() && !currentNode->children_.empty()) {
      for (const auto& node : currentNode->children_) {
        if (node.second->isEnd_)
          corrections.insert(correction + node.first);
      }
      return;
    }

    for (const auto& node : currentNode->children_) {
      if (node.first == string[counter]) {
        RecursiveReplace(string, corrections, node.second,
                         correction + string[counter], counter + 1, errorCount);
      } else {
        if (string.size() >= counter + 1 && node.first == string[counter + 1]) {
          RecursiveReplace(string, corrections, currentNode, correction,
                           counter + 1,errorCount + 1);
        }
        if (node.second->children_.count(string[counter]) != 0 && node.first == string[counter + 1]) {
          RecursiveReplace(string, corrections, node.second->children_.at(string[counter]),
             correction + node.first + string[counter],
                           counter + 2, errorCount + 1);
        }
        if (node.second->children_.count(string[counter]) != 0)
          RecursiveReplace(string, corrections, node.second,
             correction + node.first, counter,errorCount + 1);

        if (node.first != string[counter]) {
          RecursiveReplace(string, corrections, node.second,
             correction + node.first, counter + 1,errorCount + 1);
        }
      }
    }
  }

  void Clear(Node *node_p) noexcept {
    if (node_p == nullptr) {
      return;
    }
    for (auto node : node_p->children_) {
      Clear(node.second);
    }
    delete node_p;
  }
};

void Handler(std::wostream& out, const Trie& prefixTree) noexcept {
  std::wstring string;
  while (std::getline(std::wcin, string)) {
    if (string.empty()) {
      continue;
    }
    std::wcout << string;
    auto corrections = prefixTree.GetCorrections(string);
    if (!corrections.has_value()) {
      std::wcout << L" - ok\n";
    } else {
      auto value = corrections.value();
      if (value.empty()) {
        std::wcout << L" -?\n";
      } else {
        std::wstring str = L" ->";
        for (const auto &correction : value) {
          str += L" " + correction + L",";
        }
        str.pop_back();
        std::wcout << str << L"\n";
      }
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::locale loc{""};
  std::wcin.imbue(loc);
  std::wcout.imbue(loc);
  std::wstring string;
  Trie prefixTree;

  size_t mainCount = 0;
  std::wcin >> mainCount;
  std::wcin.ignore();

  for (size_t i = 0; i < mainCount; ++i) {
    std::getline(std::wcin, string);
    prefixTree.Add(string);
  }

  Handler(std::wcout, prefixTree);
  return EXIT_SUCCESS;
}