#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> // for max
#include <chrono>    // for timing the algorithm

using namespace std;

class Graph {
private:
    int V; // Number of vertices
    vector<vector<int>> adjList;

    // The main DFS logic is in this helper function
    bool isCyclicUtil(int u, vector<bool>& visited, vector<bool>& recStack, vector<int>& path) {
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
                cout << "\n--- Cycle Found! ---" << endl;
                cout << "Cycle Path: ";
                auto it = find(path.begin(), path.end(), v);
                while (it != path.end()) {
                    cout << *it << " -> ";
                    it++;
                }
                cout << v << endl;
                cout << "--------------------" << endl;
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
        vector<bool> visited(V, false);
        vector<bool> recStack(V, false);
        vector<int> path;

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
    string filename ;
    cout << "P18 Cycle Detection Program..." <<"\nEnter name of a file : ";
    cin>>filename;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error! File not found. Check the path: " << filename << endl;
        return 1;
    }
    
    // Initially, we considered hardcoding the size, but realized that's inefficient.
    // It's better to find the max node id first.
    cout << "File opened. First pass to find graph size..." << endl;
    
    int max_node = 0;
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string s_src, s_dest;
        
        getline(ss, s_src, ',');
        getline(ss, s_dest, ',');
        
        try {
            max_node = max({max_node, stoi(s_src), stoi(s_dest)});
        } catch (...) {
            // some lines might be bad, just skip them
        }
    }

    int num_vertices = max_node + 1;
    cout << "Max node ID is " << max_node << ". Total vertices needed: " << num_vertices << endl;

    // Reset file to read again for adding edges
    file.clear();
    file.seekg(0, ios::beg);
    
    Graph g(num_vertices);
    
    cout << "Second pass: loading graph edges..." << endl;
    int edge_count = 0;
    getline(file, line); // Skip header again

    while (getline(file, line)) {
        stringstream ss(line);
        string s_src, s_dest;

        getline(ss, s_src, ',');
        getline(ss, s_dest, ',');
        
        try {
            int u = stoi(s_src);
            int v = stoi(s_dest);
            g.addEdge(u, v);
            edge_count++;
        } catch (...) {
            // malformed line, skip
        }
    }
    file.close();

    cout << "Done. Loaded " << edge_count << " edges." << endl;
    cout << "---------------------------------------" << endl;
    cout << "Ok, running DFS to find a cycle..." << endl;

    auto start = chrono::high_resolution_clock::now();

    if (g.hasCycle()) {
        cout << "\nResult: Cycle Detected." << endl;
    } else {
        cout << "\nResult: No Cycle Found." << endl;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Algorithm finished in " << elapsed.count() << " seconds." << endl;

    return 0;
}
