#include <unordered_map>
#include <vector>
#include <utility>
#include <tuple>
#include <ostream>
#include <string>
#include <stdexcept>
#include <iostream>

using std::string;
using std::cout;
using std::cin;
using std::endl;

template<typename K, typename V>
class min_heap final {
private:
    void heapify_down(const size_t index) {
        size_t index_current = index;
        size_t index_left = (2 * index) + 1;
        size_t index_right = (2 * index) + 2;

        while (true) {
            if ((index_left < heap.size() - 1 &&
                 heap[index_current].first > heap[index_left].first) ||
                (index_right < heap.size() &&
                 heap[index_current].first > heap[index_right].first)) {
                K min_key =
                        std::min(heap[index_left].first,
                                 heap[index_right].first);
                size_t index_min = indexes.at(min_key);
                std::swap(indexes.at(heap[index_current].first),
                          indexes.at(min_key));
                std::swap(heap[index_current], heap[index_min]);
                index_current = index_min;
                index_left = (2 * index_current) + 1;
                index_right = (2 * index_current) + 2;
            } else if ((index_left < heap.size() &&
                        heap[index_current].first > heap[index_left].first) &&
                       (index_right >= heap.size())) {
                K min_key = heap[index_left].first;
                size_t index_min = indexes.at(min_key);
                std::swap(indexes.at(heap[index_current].first),
                          indexes.at(min_key));
                std::swap(heap[index_current], heap[index_min]);
                index_current = index_min;
                index_left = (2 * index_current) + 1;
                index_right = (2 * index_current) + 2;
            } else {
                return;
            }
        }
    }

    void heapify_up(const size_t index) {
        size_t index_current = index;
        size_t index_parent = (index - 1) / 2;

        while (index_current != 0 &&
               heap[index_current].first < heap[index_parent].first) {
            indexes[heap[index_parent].first] = index_current;
            indexes[heap[index_current].first] = index_parent;
            std::swap(heap[index_current], heap[index_parent]);
            index_current = index_parent;
            index_parent = (index_current - 1) / 2;
        }
    }

public:
    min_heap() = default;

    ~min_heap() = default;

    void add(const K &key, const V &value) {
        if (indexes.find(key) != indexes.end()) {
            throw std::logic_error("error");
        }
        heap.push_back(std::pair<K, V>(key, value));
        size_t new_index = heap.size() - 1;
        indexes[key] = new_index;
        heapify_up(new_index);
    }

    std::pair<size_t, V> search(const K &key) const {
        auto iter = indexes.find(key);
        if (iter == indexes.end()) {
            throw std::logic_error("error");
        }
        return std::make_pair(iter->second, heap[indexes.at(key)].second);
    }

    void set(const K &key, const V &value) {
        std::pair<size_t, V> target = search(key);
        heap[target.first].second = value;
    }

    void erase(const K &key) {
        std::pair<size_t, V> target = search(key);
        K old = key;
        size_t new_index = indexes.at(key);

        std::swap(indexes.at(key), indexes.at(heap[heap.size() - 1].first));
        indexes.erase(key);

        std::swap(heap[target.first], heap[heap.size() - 1]);
        heap.pop_back();

        if (new_index != heap.size()) {
            if (old < heap[new_index].first || target.first == 0) {
                heapify_down(new_index);
            } else {
                heapify_up(new_index);
            }
        }
    }

    std::pair<K, V> extract() {
        if (heap.empty()) {
            throw std::logic_error("error");
        }
        std::pair<K, V> tmp = heap[0];
        erase(heap[0].first);
        return tmp;
    }

    std::tuple<K, size_t, V> min() const {
        if (heap.empty()) {
            throw std::logic_error("error");
        }
        return std::make_tuple(heap[0].first, 0, heap[0].second);
    }

    std::tuple<K, size_t, V> max() const {
        if (heap.empty()) {
            throw std::logic_error("error");
        }
        K max = heap[0].first;
        size_t index;
        for (size_t i = 1; i < heap.size(); ++i) {
            if (2 * i + 1 >= heap.size() &&
                2 * i + 2 >= heap.size()) {
                if (heap[i].first > max) {
                    max = heap[i].first;
                    index = i;
                }
            }
        }
        return std::make_tuple(max, index, heap[indexes.at(max)].second);
    }


public:
    template<typename Key, typename Value>
    friend void print_node(std::ostream &out, const min_heap<Key, Value> &heap,
                           size_t i);

    template<typename Key, typename Value>
    friend void print(std::ostream &out, const min_heap<Key, Value> &heap);


private:
    std::unordered_map<K, size_t> indexes;
    std::vector<std::pair<K, V>> heap;
};

template<typename Key, typename Value>
void print_node(std::ostream &out, const min_heap<Key, Value> &heap,
                const size_t i) {
    out << "[" << heap.heap[i].first << " "
        << heap.heap[i].second << " "
        << heap.heap[(i - 1) / 2].first << "]" << " ";
}

template<typename K, typename V>
void print(std::ostream &out, const min_heap<K, V> &heap) {
    if (heap.heap.empty()) {
        out << "_" << endl;
        return;
    }

    out << "[" << heap.heap[0].first
        << " " << heap.heap[0].second << "]" << " " << endl;

    size_t level = 2;
    size_t counter = 0;

    for (size_t i = 1; i < heap.heap.size(); ++i) {
        print_node(out, heap, i);

        counter++;
        if (i + 1 == heap.heap.size()) {
            for (size_t j = 0; j < level - counter; ++j) {
                out << "_" << " ";
            }
            out << endl;
            break;
        }
        if (counter == level) {
            counter = 0;
            level <<= 1;
            out << endl;
        }
    }
}

template<typename K, typename V>
void receiver(std::ostream &out, min_heap<K, V> &heap) {
    string command, value;
    int64_t key = 0;

    while (cin >> command) {
        try {
            if (command.empty()) {
                continue;
            } else if (command == "add") {
                cin >> key >> value;
                heap.add(key, value);
            } else if (command == "set") {
                cin >> key >> value;
                heap.set(key, value);
            } else if (command == "delete") {
                cin >> key;
                heap.erase(key);
            } else if (command == "search") {
                try {
                    cin >> key;
                    std::pair<size_t, string> tuple = heap.search(key);
                    out << "1 " << tuple.first << " " << tuple.second
                        << endl;
                } catch (std::logic_error &e) {
                    out << "0" << endl;
                }
            } else if (command == "min") {
                std::tuple<int64_t, size_t, string> tuple = heap.min();
                out << std::get<0>(tuple) << " " << std::get<1>(tuple) << " "
                    << std::get<2>(tuple) << endl;
            } else if (command == "max") {
                std::tuple<int64_t, size_t, string> tuple = heap.max();
                out << std::get<0>(tuple) << " " << std::get<1>(tuple) << " "
                    << std::get<2>(tuple) << endl;
            } else if (command == "extract") {
                std::pair<int64_t, string> pair = heap.extract();
                out << pair.first << " " << pair.second << endl;
            } else if (command == "print") {
                print(out, heap);
            } else {
                out << "error" << endl;
            }
        } catch (...) {
            out << "error" << endl;
        }
    }
}


int main() {
    min_heap<int64_t, string> heap;
    receiver(cout, heap);
    return 0;
}