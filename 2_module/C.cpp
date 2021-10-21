#include <unordered_map>
#include <vector>
#include <utility>
#include <tuple>
#include <string>


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


    void Heapify(const K& key) noexcept {
        index_t index;
        index_t indexCurrent;
        index_t indexLeftChild;
        index_t indexRightChild;

        while(true) {
            indexCurrent = index = GetIndex(key);
            indexLeftChild = GetIndexLeftChild(key);
            indexRightChild = GetIndexRightChild(key);
            if (indexLeftChild < GetSize() && tree_[indexLeftChild] < tree_[indexCurrent]) {
                indexCurrent = indexLeftChild;
            }
            if (indexRightChild < GetSize() && tree_[indexRightChild] < tree_[indexCurrent]) {
                indexCurrent = indexRightChild;
            }
            if (indexCurrent == index) {
                break;
            }

            std::swap(tree_[index], tree_[indexCurrent]);
            std::swap(mapIndex_[tree_[index].first], mapIndex_[tree_[indexCurrent].first]);
            index = indexCurrent;
        }
    }

public:                                                     // interaction interface
    MinBinaryHeap() = default;
    void Add(const keyValue_t& node) noexcept {
        if (IsEmpty()) {
            tree_.push_back(keyValue_t(0, "infinity"));
            tree_.push_back(node);
            mapIndex_[node.first] = tree_.size() - 1;
            return;
        } else {
            tree_.push_back(node);
            mapIndex_[node.first] = tree_.size() - 1;
            Heapify(tree_[GetIndexParent(node.first)].first);
        }
    }

    void Set(const keyValue_t& node) noexcept;
    void Delete(const K& key) noexcept;
    auto Search(const K& key) const noexcept -> keyValue_t;

    [[nodiscard]] auto GetMin() const noexcept -> keyIndexValue_t;
    [[nodiscard]] auto GetMax() const noexcept -> keyIndexValue_t;

    [[nodiscard]] auto GetSize() const noexcept -> size_t {
        return tree_.size();
    }

    [[nodiscard]] auto IsEmpty() const noexcept -> bool {
        return tree_.empty();
    }


    auto Extract() noexcept -> keyValue_t;

private:
    std::unordered_map<K, index_t> mapIndex_;
    std::vector<keyValue_t> tree_;
};


int main() {
    MinBinaryHeap<int64_t, std::string> heap;
    heap.Add(std::pair<int64_t, std::string>(1, "12"));
    heap.Add(std::pair<int64_t, std::string>(3, "13"));
    heap.Add(std::pair<int64_t, std::string>(5, "14"));
    heap.Add(std::pair<int64_t, std::string>(4, "15"));
    heap.Add(std::pair<int64_t, std::string>(6, "16"));
    heap.Add(std::pair<int64_t, std::string>(7, "17"));
    heap.Add(std::pair<int64_t, std::string>(2, "18"));

    return EXIT_SUCCESS;
}

