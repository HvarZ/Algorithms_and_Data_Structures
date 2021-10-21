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
    void Add(const keyValue_t& node) noexcept {
        if (IsEmpty()) {
            tree_.push_back(keyValue_t(0, "infinity"));
            tree_.push_back(node);
            mapIndex_[node.first] = tree_.size() - 1;
            return;
        } else {
            tree_.push_back(node);
            mapIndex_[node.first] = tree_.size() - 1;
            HeapifyUp(node.first);
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
    heap.Add(std::pair<int64_t, std::string>(2, "12"));
    heap.Add(std::pair<int64_t, std::string>(3, "13"));
    heap.Add(std::pair<int64_t, std::string>(4, "14"));
    heap.Add(std::pair<int64_t, std::string>(5, "15"));
    heap.Add(std::pair<int64_t, std::string>(6, "16"));
    heap.Add(std::pair<int64_t, std::string>(7, "17"));
    heap.Add(std::pair<int64_t, std::string>(8, "18"));
    heap.Add(std::pair<int64_t, std::string>(1, "11"));

    return EXIT_SUCCESS;
}

