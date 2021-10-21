#include <unordered_map>
#include <vector>
#include <utility>
#include <tuple>
#include <string>
#include <iostream>
#include <stdexcept>


using index_t = size_t;                                     // alias for type of indexation

template <typename K, typename V>
class MinBinaryHeap {
public:                                                     // some useful aliases
    using keyIndexValue_t = std::tuple<K, index_t, V>;
    using keyValue_t = std::pair<K, V>;

private:                                                    // service functions
    [[nodiscard]] auto GetIndexLeftChild(const K& key) const noexcept -> index_t {
        return 2 * mapIndex_.at(key);
    }
    [[nodiscard]] auto GetIndexRightChild(const K& key) const noexcept -> index_t {
        return 2 * mapIndex_.at(key) + 1;
    }

    [[nodiscard]] auto GetIndex(const K& key) const noexcept -> index_t {
        return mapIndex_.at(key);
    }

    [[nodiscard]] auto GetIndexParent(const K& key) const noexcept ->index_t {
        return mapIndex_.at(key) / 2;
    }


    void HeapifyUp(const K& key) noexcept {
        index_t indexCurrent;
        index_t indexParent;

        while(true) {
            indexCurrent = GetIndex(key);
            indexParent = GetIndexParent(key);

            if (tree_[indexCurrent].first < tree_[indexParent].first) {
                std::swap(tree_[indexCurrent], tree_[indexParent]);
                std::swap(mapIndex_[tree_[indexCurrent].first], mapIndex_[tree_[indexParent].first]);
            } else {
                break;
            }
        }
    }

public:                                                     // interaction interface
    MinBinaryHeap() = default;
    void Add(const K& key, const V& value) noexcept {
        if (IsEmpty()) {
            tree_.push_back(keyValue_t(K(), V()));
            tree_.push_back(keyValue_t(key, value));
            mapIndex_[key] = tree_.size() - 1;
            return;
        } else {
            tree_.push_back(keyValue_t(key, value));
            mapIndex_[key] = tree_.size() - 1;
            HeapifyUp(key);
        }
    }

    void Set(const K& key, const V& value) noexcept {
        tree_[mapIndex_[key]].second = value;
    }

    void Delete(const K& key) noexcept;

    [[nodiscard]] auto Search(const K& key) const -> std::tuple<index_t, V> {
        if (IsEmpty() || mapIndex_.count(key) == 0) {
            throw std::runtime_error("error");
        }
        return std::make_tuple(mapIndex_.at(key), tree_[mapIndex_.at(key)].second);
    }

    [[nodiscard]] auto GetMin() const noexcept -> keyIndexValue_t {
        return std::make_tuple(tree_[1].first, 1, tree_[1].second);
    }

    [[nodiscard]] auto GetMax() const noexcept -> keyIndexValue_t {
        K maxKey = tree_[1].first;
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
        return tree_.empty() || tree_.size() == 1;
    }

    [[nodiscard]] auto GetTree() const noexcept -> const std::vector<keyValue_t>* {
        return &tree_;
    }


    auto Extract() noexcept -> keyValue_t;

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

    std::cout << "[" << std::get<0>(heap.GetTree()->operator[](1))
              << " " << std::get<1>(heap.GetTree()->operator[](1)) << "]" << " " << std::endl;

    size_t levelSize = 2;
    size_t counterElementLevel = 0;

    for (size_t i = 2; i < heap.GetSize(); ++i) {
        std::cout << "[" << std::get<0>(heap.GetTree()->operator[](i)) << " "
                         << std::get<1>(heap.GetTree()->operator[](i)) << " "
                         << std::get<0>(heap.GetTree()->operator[](i / 2)) << "]" << " ";

        counterElementLevel++;
        if (counterElementLevel == levelSize) {
            levelSize <<= 1;
            std::cout << std::endl;
            counterElementLevel = 0;
        }
        if (i + 1 == heap.GetSize()) {
            for (size_t j = 0; j < levelSize - counterElementLevel; ++j) {
                std::cout << "_" << " ";
            }
            std::cout << std::endl;
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
            std::cin >> key >> value;
            heap.Add(key, value);
        } else if (command == "set") {
            std::cin >> key >> value;
            heap.Set(key, value);
        } else if (command == "delete") {
            std::cin >> key;
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
            auto tuple = heap.GetMin();
            std::cout << std::get<0>(tuple) << " "
                      << std::get<1>(tuple) << " "
                      << std::get<2>(tuple) << std::endl;
        } else if (command == "max") {
            auto tuple = heap.GetMax();
            std::cout << std::get<0>(tuple) << " "
                      << std::get<1>(tuple) << " "
                      << std::get<2>(tuple) << std::endl;
        } else if (command == "extract") {

        } else if (command == "print") {
            Print(heap);
        } else {
            std::cout << "error" << std::endl;
        }
    }
}


int main() {
    MinBinaryHeap<int64_t, std::string> heap;
    Handler(heap);

    return EXIT_SUCCESS;
}

