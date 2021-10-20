#include <unordered_map>
#include <vector>
#include <utility>
#include <tuple>

using index_t = size_t;                                     // alias for type of indexation

template <typename K, typename V>
class MinBinaryHeap {
private:                                                    // some useful aliases
    using keyIndexValue_t = std::tuple<K, index_t, V>;
    using keyValue_t = std::pair<K, V>;

private:                                                    // service functions
    void UpHeapify(const K& key) noexcept;
    void DownHeapify(const K& key) noexcept;

public:                                                     // interaction interface
    MinBinaryHeap() = default;
    void Add(const keyValue_t& node) noexcept;
    void Set(const keyValue_t& node) noexcept;
    void Delete(const K& key) noexcept;
    auto Search(const K& key) const noexcept -> keyValue_t;

    [[nodiscard]] auto GetMin() const noexcept -> keyIndexValue_t;
    [[nodiscard]] auto GetMax() const noexcept -> keyIndexValue_t;

    auto Extract() noexcept -> keyValue_t;

private:
    std::unordered_map<K, index_t> mapIndex_;
    std::vector<keyValue_t> tree_;
};


int main() {

    return EXIT_SUCCESS;
}

