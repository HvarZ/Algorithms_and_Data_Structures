import java.util.HashMap;
import java.util.TreeSet;
import java.util.Set;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Stack;
import java.util.Scanner;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.BufferedWriter;
import java.io.IOException;
import java.util.Iterator;

public class MainGraph {
    public static class Graph {
        private HashMap<String, TreeSet<String>> graph_;
        private boolean isOriented_;

        public Graph(boolean isOriented) {
            isOriented_ = isOriented;
            graph_ = new HashMap<>();
        }

        public void addEdge(String first, String last) {
            if (!graph_.containsKey(first)) {
                graph_.put(first, new TreeSet<String>());
            }
            graph_.get(first).add(last);

            if (!isOriented_) {
               if (!graph_.containsKey(last)) {
                   graph_.put(last, new TreeSet<String>());
               }
               graph_.get(last).add(first);
            }
        }

        public void breadthSearch(String startVertex, OutputStream out) {
            Queue<String> queue = new LinkedList<>();
            queue.add(startVertex);

            HashSet<String> visited = new HashSet<>();
            visited.add(startVertex);

            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out));
            while (!queue.isEmpty()) {
                String currentVertex = queue.peek();
                try {
                    writer.write(currentVertex + "\n");
                } catch (IOException ex) {
                    System.err.println(ex.getMessage());
                }
                queue.remove();

                if (!graph_.containsKey(currentVertex)) {
                    continue;
                }

                for (String adjacentVertex : graph_.get(currentVertex)) {
                    if (!visited.contains(adjacentVertex)) {
                        queue.add(adjacentVertex);
                        visited.add(adjacentVertex);
                    }
                }
            }
            try {
                writer.flush();
            } catch (IOException ex) {
                System.err.println(ex.getMessage());
            }
        }

        public void depthSearch(String startVertex, OutputStream out) {
            Stack<String> stack = new Stack<>();
            HashSet<String> visited = new HashSet<>();

            stack.push(startVertex);

            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out));
            while (!stack.isEmpty()) {
                String currentVertex = stack.peek();
                stack.pop();

                if (!visited.contains(currentVertex)) {
                    visited.add(currentVertex);
                    try {
                        writer.write(currentVertex + "\n");
                    } catch (IOException ex) {
                        System.err.println(ex.getMessage());
                    }
                }
                if (!graph_.containsKey(currentVertex)) {
                    continue;
                }

                Iterator<String> iterator = graph_.get(currentVertex).descendingIterator();
                while (iterator.hasNext()) {
                    String element = iterator.next();
                    if (!visited.contains(element)) {
                        stack.push(element);
                    }
                }
            }
            try {
                writer.flush();
            } catch (IOException ex) {
                System.err.println(ex.getMessage());
            }
        }
    }

    public static void main(String[] args) {
        char graphType, searchType;
        String startVertex, firstVertex, secondVertex;
        boolean isOriented = false;

        Scanner input = new Scanner(System.in);
        graphType = input.next().charAt(0);
        startVertex = input.next();
        searchType = input.next().charAt(0);

        if (graphType == 'd') {
            isOriented = true;
        } else if (graphType == 'u') {
            isOriented = false;
        }

        Graph graph = new Graph(isOriented);

        while (input.hasNext()) {
            firstVertex = input.next();
            secondVertex = input.next();
            graph.addEdge(firstVertex, secondVertex);
        }

        if (searchType == 'b') {
            graph.breadthSearch(startVertex, System.out);
        } else if (searchType == 'd') {
            graph.depthSearch(startVertex, System.out);
        }
    }
}