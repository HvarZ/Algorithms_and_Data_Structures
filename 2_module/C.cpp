#include <unordered_map>
#include <vector>
#include <utility>
#include <tuple>
#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>

using index_t = size_t;                                     // alias for type of indexation

template <typename K, typename V>
class MinBinaryHeap {
public:                                                     // some useful aliases
    using keyIndexValue_t = std::tuple<K, index_t, V>;
    using keyValue_t = std::pair<K, V>;

private:                                                    // service functions
    [[nodiscard]] auto GetIndexLeftChild(const K& key) const noexcept -> index_t {
        return 2 * mapIndex_.at(key) + 1;
    }
    [[nodiscard]] auto GetIndexRightChild(const K& key) const noexcept -> index_t {
        return 2 * mapIndex_.at(key) + 2;
    }

    [[nodiscard]] auto GetIndex(const K& key) const noexcept -> index_t {
        return mapIndex_.at(key);
    }

    [[nodiscard]] auto GetIndexParent(const K& key) const noexcept -> index_t {
        return (mapIndex_.at(key) - 1) / 2;
    }

    [[nodiscard]] auto GetIndexLastNode() const noexcept -> index_t {
        return GetSize() - 1;
    }


    void HeapifyUp(const K& key) noexcept {
        index_t indexCurrent;
        index_t indexParent;

        while(true) {
            indexCurrent = GetIndex(key);
            indexParent = GetIndexParent(key);
            if (mapIndex_.at(key) == 0) {
                return;
            }

            if (tree_[indexCurrent].first < tree_[indexParent].first) {
                std::swap(mapIndex_.at(tree_[indexCurrent].first), mapIndex_.at(tree_[indexParent].first));
                std::swap(tree_[indexCurrent], tree_[indexParent]);
            } else {
                return;
            }
        }
    }

    void HeapifyDown(const K& key) noexcept {
        index_t indexCurrent;
        index_t indexLeftChild;
        index_t indexRightChild;
        K minKeyChild;
        index_t indexMinKey;
        const K keyCurrent = key;

        while(true) {
            indexCurrent = GetIndex(keyCurrent);
            indexLeftChild = GetIndexLeftChild(keyCurrent);
            indexRightChild = GetIndexRightChild(keyCurrent);


            if ((tree_[indexCurrent].first > tree_[indexLeftChild].first && indexLeftChild < GetSize()) &&
                (tree_[indexCurrent].first > tree_[indexRightChild].first && indexRightChild < GetSize())) {
                minKeyChild = std::min(tree_[indexLeftChild].first, tree_[indexRightChild].first);
                indexMinKey = mapIndex_.at(minKeyChild);
                std::swap(mapIndex_.at(tree_[indexCurrent].first), mapIndex_.at(minKeyChild));
                std::swap(tree_[indexCurrent], tree_[indexMinKey]);
            } else if ((tree_[indexCurrent].first > tree_[indexLeftChild].first && indexLeftChild < GetSize()) &&
                       (tree_[indexCurrent].first > tree_[indexRightChild].first && indexRightChild >= GetSize())) {
                minKeyChild = tree_[indexLeftChild].first;
                indexMinKey = mapIndex_.at(minKeyChild);
                std::swap(mapIndex_.at(tree_[indexCurrent].first), mapIndex_.at(minKeyChild));
                std::swap(tree_[indexCurrent], tree_[indexMinKey]);
            } else {
                return;
            }
        }
    }

    void Heapify(const K& key) noexcept {
        HeapifyUp(key);
        HeapifyDown(key);
    }


    public:                                                     // interaction interface
    MinBinaryHeap() = default;
    void Add(const K& key, const V& value) {
        if (mapIndex_.count(key) != 0) {
            throw std::runtime_error("error");
        }
        tree_.push_back(keyValue_t(key, value));
        mapIndex_[key] = tree_.size() - 1;
        Heapify(key);
    }

    void Set(const K& key, const V& value) {
        auto target = Search(key);
        tree_[target.first].second = value;
    }

    void Delete(const K& key) {
        auto target = Search(key);
        index_t newIndexLast = mapIndex_.at(key);

        std::swap(mapIndex_.at(key), mapIndex_.at(tree_[GetIndexLastNode()].first));
        mapIndex_.erase(key);

        std::swap(tree_[target.first], tree_[GetIndexLastNode()]);
        tree_.pop_back();


        if (newIndexLast != GetSize()) {
            Heapify(tree_[newIndexLast].first);
        }
    }

    [[nodiscard]] auto Search(const K& key) const -> std::pair<index_t, V> {
        if (IsEmpty() || mapIndex_.count(key) == 0) {
            throw std::runtime_error("error");
        }
        return std::make_pair(mapIndex_.at(key), tree_[mapIndex_.at(key)].second);
    }

    [[nodiscard]] auto GetMin() const -> keyIndexValue_t {
        if (GetSize() == 0) {
            throw std::runtime_error("error");
        }
        return std::make_tuple(tree_[0].first, 0, tree_[0].second);
    }

    [[nodiscard]] auto GetMax() const -> keyIndexValue_t {
        if (GetSize() == 0) {
            throw std::runtime_error("error");
        }
        K maxKey = tree_[0].first;
        index_t index;
        for (size_t i = 1; i < GetSize(); ++i) {
            if (GetIndexLeftChild(tree_[i].first) >= GetSize() &&
                GetIndexRightChild(tree_[i].first) >= GetSize()) {
                if (tree_[i].first > maxKey) {
                    maxKey = tree_[i].first;
                    index = i;
                }
            }
        }
        return std::make_tuple(maxKey, index, tree_[mapIndex_.at(maxKey)].second);
    }

    [[nodiscard]] auto GetSize() const noexcept -> size_t {
        return tree_.size();
    }

    [[nodiscard]] auto IsEmpty() const noexcept -> bool {
        return tree_.empty();
    }

    [[nodiscard]] auto GetTree() const noexcept -> const std::vector<keyValue_t>* {
        return &tree_;
    }


    auto Extract() -> keyValue_t {
        if (IsEmpty()) {
            throw std::runtime_error("error");
        }
        auto tmp = tree_[0];
        Delete(tree_[0].first);
        return tmp;
    }

private:
    std::unordered_map<K, index_t> mapIndex_;
    std::vector<keyValue_t> tree_;
};

template <typename K, typename V>
void Print(const MinBinaryHeap<K, V> heap) {
    if (heap.IsEmpty()) {
        std::cout << "_" << std::endl;
        return;
    }

    std::cout << "[" << std::get<0>(heap.GetTree()->operator[](0))
              << " " << std::get<1>(heap.GetTree()->operator[](0)) << "]" << " " << std::endl;

    size_t levelSize = 2;
    size_t counterElementLevel = 0;

    for (size_t i = 1; i < heap.GetSize(); ++i) {
        std::cout << "[" << std::get<0>(heap.GetTree()->operator[](i)) << " "
                         << std::get<1>(heap.GetTree()->operator[](i)) << " "
                         << std::get<0>(heap.GetTree()->operator[]((i - 1) / 2)) << "]" << " ";

        counterElementLevel++;
        if (i + 1 == heap.GetSize()) {
            for (size_t j = 0; j < levelSize - counterElementLevel; ++j) {
                std::cout << "_" << " ";
            }
            std::cout << std::endl;
            break;
        }
        if (counterElementLevel == levelSize) {
            levelSize <<= 1;
            std::cout << std::endl;
            counterElementLevel = 0;
        }
    }
}

template <typename K, typename V>
void Handler(MinBinaryHeap<K, V>& heap) noexcept {
    std::string command, value;
    int64_t key;


    while (std::cin >> command) {
        if (command.empty()) {
            continue;
        } else if (command == "add") {
            try {
                std::cin >> key >> value;
                heap.Add(key, value);
            } catch (std::runtime_error& e) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "set") {
            try {
                std::cin >> key >> value;
                heap.Set(key, value);
            } catch (std::runtime_error& e) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "delete") {
            try {
                std::cin >> key;
                heap.Delete(key);
            } catch (std::runtime_error& e) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "search") {
            try {
                std::cin >> key;
                auto tuple = heap.Search(key);
                std::cout << "1 " << std::get<0>(tuple) << " "
                                  << std::get<1>(tuple) << std::endl;
            } catch (std::runtime_error& e) {
                std::cout << "0" << std::endl;
            }
        } else if (command == "min") {
            try {
                auto tuple = heap.GetMin();
                std::cout << std::get<0>(tuple) << " "
                          << std::get<1>(tuple) << " "
                          << std::get<2>(tuple) << std::endl;
            } catch (std::runtime_error& e) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "max") {
            try {
                auto tuple = heap.GetMax();
                std::cout << std::get<0>(tuple) << " "
                          << std::get<1>(tuple) << " "
                          << std::get<2>(tuple) << std::endl;
            } catch (std::runtime_error& e) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "extract") {
            try {
                auto pairKeyValue = heap.Extract();
                std::cout << pairKeyValue.first << " " << pairKeyValue.second << std::endl;
            } catch (std::runtime_error& e) {
                std::cout << "error" << std::endl;
            }
        } else if (command == "print") {
            Print(heap);
        } else {
            std::cout << "error" << std::endl;
        }
    }
}


int main() {
    MinBinaryHeap<int64_t, std::string> heap;
    Handler<int64_t, std::string>(heap);

    return EXIT_SUCCESS;
}


