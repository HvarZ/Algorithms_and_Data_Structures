#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>

using namespace std;

template <typename T>
class Graph {
public:
    Graph(char type) {
        graph_type = type;
    }

    void add_edge(const T& first_vertex, const T& second_vertex) {
        graph[first_vertex].insert(second_vertex);
        if (graph_type == 'u') {
            graph[second_vertex].insert(first_vertex);
        }
    }

    void breadth_search(const T& start_vertex) const  {
        queue<string> queue;
        queue.push(start_vertex);

        unordered_set<string> visited;
        visited.insert(start_vertex);

        while (!queue.empty()) {
            const string currentVertex = queue.front();
            cout << queue.front() << endl;
            queue.pop();

            if (graph.find(currentVertex) == graph.end())
                continue;

            for (auto it_adjacent = graph.at(currentVertex).begin();
                 it_adjacent != graph.at(currentVertex).end();
                 it_adjacent++) {
                if (visited.count(*it_adjacent) == 0) {
                    queue.push(*it_adjacent);
                    visited.insert(*it_adjacent);
                }
            }
        }
    }

    void depth_search(const T& start_vertex) const {
        stack<string> stack;
        stack.push(start_vertex);

        unordered_set<string> visited;

        while (!stack.empty()) {
            const T currentVertex = stack.top();
            stack.pop();

            if (!visited.count(currentVertex)) {
                visited.insert(currentVertex);
                cout << currentVertex << endl;
            }

            if (graph.find(currentVertex) == graph.end())
                continue;

            for (auto it_adjacent = graph.at(currentVertex).rbegin();
                it_adjacent != graph.at(currentVertex).rend();
                it_adjacent++) {
                if (visited.count(*it_adjacent) == 0) {
                    stack.push(*it_adjacent);
                }
            }
        }
    }

private:
    using graph_structure = unordered_map<T, set<T>>;
    graph_structure graph;
    char graph_type;
};


int main() {
    char graph_type, search_type;
    string start_vertex, first_vertex, second_vertex;
    cin >> graph_type >> start_vertex >> search_type;
    Graph<string> graph(graph_type);

    while (cin >> first_vertex >> second_vertex) {
        graph.add_edge(first_vertex, second_vertex);
    }

    if (search_type == 'b') {
        graph.breadth_search(start_vertex);
    }

    else if (search_type == 'd') {
        graph.depth_search(start_vertex);
    }
    return EXIT_SUCCESS;
}