#include <unordered_map>
#include <unordered_set>
#include <set>
#include <utility>
#include <string>
#include <iostream>
#include <stdexcept>
#include <queue>
#include <stack>
#include <functional>

template <typename T>
class Graph {
public:

    // enumeration for understanding which search to use
    enum TraversalType : bool {
        Depth = false,
        Breadth = true
    };

    explicit Graph(bool isDirected, TraversalType type) noexcept;
    void Add(const std::pair<T, T>& edge) noexcept;
    void DepthFirstSearch(const T& vertex) const noexcept;
    void BreadthFirstSearch(const T& vertex) const noexcept;

private:
    // comparator for direct or reverse (from less to more) initialization of a set
    using Comp = std::function<bool(const T&, const T&)>;
    std::unordered_map<T, std::set<T, Comp>> graph_;
    bool isDirected_;
    TraversalType type_;
    Comp comparator_;
};



template <typename T>
Graph<T>::Graph(bool isDirected, const TraversalType type) noexcept
                : isDirected_(isDirected), type_(type) {
     if (type_ == Depth) {
        comparator_ = std::greater<T>{};    // from more to less
     } else if (type_ == Breadth) {
        comparator_ = std::less<T>{};       // from less to more
     }
}

template <typename T>
void Graph<T>::Add(const std::pair<T, T> &edge) noexcept {
    auto itFirst = graph_.find(edge.first);
    auto itSecond = graph_.find(edge.second);

    if (itFirst == graph_.end()) {
        graph_.insert(std::make_pair(edge.first, std::set<T, Comp>(comparator_)));
        graph_[edge.first].insert(edge.second);
    } else {
        itFirst->second.insert(edge.second);
    }
    // adding a reverse edge
    if (!isDirected_ && itSecond == graph_.end()) {
        graph_.insert(std::make_pair(edge.second, std::set<T, Comp>(comparator_)));
        graph_[edge.second].insert(edge.first);
    } else if (!isDirected_ && itSecond != graph_.end()) {
        itSecond->second.insert(edge.first);
    }
}

template<typename T>
void Graph<T>::BreadthFirstSearch(const T &vertex) const noexcept {
    std::unordered_set<T> usedVertex;
    std::queue<T> queueVertex;

    usedVertex.insert(vertex);
    queueVertex.push(vertex);

    while (!queueVertex.empty()) {
        T currentVertex = std::move(queueVertex.front());
        std::cout << currentVertex << std::endl;
        queueVertex.pop();

        auto it = graph_.find(currentVertex);
        if (it == graph_.end()) {
            continue;
        }
        for (const auto& adjacentVertex : it->second) {
            if (!usedVertex.count(adjacentVertex)) {
                usedVertex.insert(adjacentVertex);
                queueVertex.push(adjacentVertex);
            }
        }
    }
}

template<typename T>
void Graph<T>::DepthFirstSearch(const T &vertex) const noexcept {
    std::unordered_set<T> usedVertex;
    std::stack<T> stackVertex;

    stackVertex.push(vertex);

    while (!stackVertex.empty()) {
        T currentVertex = std::move(stackVertex.top());

        stackVertex.pop();
        if (!usedVertex.count(currentVertex)) {
            std::cout << currentVertex << std::endl;
            usedVertex.insert(currentVertex);
        }

        auto it = graph_.find(currentVertex);
        if (it == graph_.end()) {
            continue;
        }
        for (const auto& adjacentVertex : it->second) {
            if (!usedVertex.count(adjacentVertex)) {
                stackVertex.push(adjacentVertex);
            }
        }
    }
}


int main() {
    std::string startVertex, firstVertex, secondVertex;
    char graphType, searchType;

    std::cin >> graphType >> startVertex >> searchType;

    if (graphType != 'd' && graphType != 'u') {
        throw std::invalid_argument("invalid type");
    }
    if (searchType != 'b' && searchType != 'd') {
        throw std::invalid_argument("invalid search");
    }

    Graph<std::string> graph(graphType == 'd',
                         static_cast<Graph<std::string>::TraversalType>(searchType == 'b'));

    while (std::cin >> firstVertex >> secondVertex) {
        graph.Add(std::make_pair(std::move(firstVertex), std::move(secondVertex)));
    }

    if (searchType == 'b') {
        graph.BreadthFirstSearch(startVertex);
    }
    else if (searchType == 'd') {
        graph.DepthFirstSearch(startVertex);
    }

    return 0;
}
