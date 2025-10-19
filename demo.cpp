#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> // for std::max
#include <chrono>    // for timing the algorithm

class Graph {
private:
    int V; // Number of vertices
    std::vector<std::vector<int>> adjList;

    // The main DFS logic is in this helper function
    bool isCyclicUtil(int u, std::vector<bool>& visited, std::vector<bool>& recStack, std::vector<int>& path) {
        visited[u] = true;
        recStack[u] = true;
        path.push_back(u);

        // Go through all neighbours of this vertex
        for (int v : adjList[u]) {
            // If we haven't visited the neighbour, recurse on it
            if (!visited[v]) {
                if (isCyclicUtil(v, visited, recStack, path)) {
                    return true; // Cycle was found deeper in the recursion
                }
            }
            // If the neighbour is already in our current path (recStack), it's a back edge.
            // Cycle found!
            else if (recStack[v]) {
                std::cout << "\n--- Cycle Found! ---" << std::endl;
                std::cout << "Cycle Path: ";
                auto it = std::find(path.begin(), path.end(), v);
                while (it != path.end()) {
                    std::cout << *it << " -> ";
                    it++;
                }
                std::cout << v << std::endl;
                std::cout << "--------------------" << std::endl;
                return true;
            }
        }

        // Backtrack: remove from current path
        recStack[u] = false;
        path.pop_back();
        return false;
    }

public:
    // Constructor
    Graph(int num_vertices) {
        this->V = num_vertices;
        adjList.resize(V);
    }

    // Function to add an edge
    void addEdge(int u, int v) {
        if (u < V && v < V) {
            adjList[u].push_back(v);
        }
    }

    // Main function to check for cycle
    bool hasCycle() {
        std::vector<bool> visited(V, false);
        std::vector<bool> recStack(V, false);
        std::vector<int> path;

        // We have to check from every node in case graph is disconnected
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                if (isCyclicUtil(i, visited, recStack, path)) {
                    return true;
                }
            }
        }
        return false;
    }
};

Graph generateAcyclicGraph(int V) {
    Graph g(V);
    // Add edges only from lower to higher indices to avoid cycles
    for (int u = 0; u < V - 1; u++)
        g.addEdge(u, u + 1);
    return g;
}

Graph generateCyclicGraph(int V) {
    Graph g(V);
    for (int u = 0; u < V - 1; u++)
        g.addEdge(u, u + 1);
    g.addEdge(V - 1, 0); // back edge → creates cycle
    return g;
}

Graph generateDisconnectedGraph(int V) {
    Graph g(V);
    g.addEdge(0, 1);
    g.addEdge(2, 3); // two separate components
    return g;
}

Graph generateSelfLoopGraph(int V) {
    Graph g(V);
    g.addEdge(0, 0); // self loop = immediate cycle
    return g;
}

Graph generateEmptyGraph(int V) {
    Graph g(V);
    // No edges
    return g;
}

// --- Main Program ---
int main() {
    cout << "DFS-Based Cycle Detection Experiment\n\n";

    vector<pair<string, Graph>> tests = {
        {"Acyclic Graph", generateAcyclicGraph(5)},
        {"Cyclic Graph", generateCyclicGraph(5)},
        {"Disconnected Graph", generateDisconnectedGraph(6)},
        {"Self-Loop Graph", generateSelfLoopGraph(4)},
        {"Empty Graph", generateEmptyGraph(3)}
    };

    for (auto& [name, g] : tests) {
        cout << name << ": ";
        if (g.isCyclic())
            cout << "Cycle Detected\n";
        else
            cout << "No Cycle\n";
    }

    return 0;
}
