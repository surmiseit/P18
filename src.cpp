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

// --- Main Program ---
int main() {
    std::cout << "P18 Cycle Detection Program..." << std::endl;

    std::string filename = "data/dataset.csv";
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error! File not found. Check the path: " << filename << std::endl;
        return 1;
    }
    
    // Initially, we considered hardcoding the size, but realized that's inefficient.
    // It's better to find the max node id first.
    std::cout << "File opened. First pass to find graph size..." << std::endl;
    
    int max_node = 0;
    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string s_src, s_dest;
        
        std::getline(ss, s_src, ',');
        std::getline(ss, s_dest, ',');
        
        try {
            max_node = std::max({max_node, std::stoi(s_src), std::stoi(s_dest)});
        } catch (...) {
            // some lines might be bad, just skip them
        }
    }

    int num_vertices = max_node + 1;
    std::cout << "Max node ID is " << max_node << ". Total vertices needed: " << num_vertices << std::endl;

    // Reset file to read again for adding edges
    file.clear();
    file.seekg(0, std::ios::beg);
    
    Graph g(num_vertices);
    
    std::cout << "Second pass: loading graph edges..." << std::endl;
    int edge_count = 0;
    std::getline(file, line); // Skip header again

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string s_src, s_dest;

        std::getline(ss, s_src, ',');
        std::getline(ss, s_dest, ',');
        
        try {
            int u = std::stoi(s_src);
            int v = std::stoi(s_dest);
            g.addEdge(u, v);
            edge_count++;
        } catch (...) {
            // malformed line, skip
        }
    }
    file.close();

    std::cout << "Done. Loaded " << edge_count << " edges." << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Ok, running DFS to find a cycle..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    if (g.hasCycle()) {
        std::cout << "\nResult: Cycle Detected." << std::endl;
    } else {
        std::cout << "\nResult: No Cycle Found." << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Algorithm finished in " << elapsed.count() << " seconds." << std::endl;

    return 0;
}
