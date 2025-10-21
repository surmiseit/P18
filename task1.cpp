#include <bits/stdc++.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;
public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        if (u >= 0 && v >= 0 && u < V && v < V && u != v)
            adj[u].push_back(v);
    }

    bool isCyclicUtil(int u, vector<bool>& visited, vector<bool>& recStack) {
        visited[u] = true;
        recStack[u] = true;

        for (int v : adj[u]) {
            if (!visited[v] && isCyclicUtil(v, visited, recStack))
                return true;
            else if (recStack[v])
                return true;
        }

        recStack[u] = false;
        return false;
    }

    bool isCyclic() {
        vector<bool> visited(V, false), recStack(V, false);
        for (int i = 0; i < V; i++)
            if (!visited[i] && isCyclicUtil(i, visited, recStack))
                return true;
        return false;
    }

    void printGraph() {
        for (int i = 0; i < V; i++) {
            cout << i << " -> ";
            for (int v : adj[i]) cout << v << " ";
            cout << endl;
        }
    }
};
Graph generateRandomGraph(int V, double edgeProb, bool forceCycle = false) {
    Graph g(V);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    if (!forceCycle) {
        // Generate a DAG (Directed Acyclic Graph)
        // Only add edges from smaller index → larger index
        for (int u = 0; u < V; u++) {
            for (int v = u + 1; v < V; v++) {
                if (dis(gen) < edgeProb)
                    g.addEdge(u, v);
            }
        }
    } else {
        // Generate general directed graph (possibly cyclic)
        for (int u = 0; u < V; u++) {
            for (int v = 0; v < V; v++) {
                if (u != v && dis(gen) < edgeProb)
                    g.addEdge(u, v);
            }
        }

        // Ensure at least one cycle exists by adding a back edge
        if (V > 2) {
            int a = gen() % (V - 1);
            int b = (a + 1) % V;
            g.addEdge(b, a); // back edge creates guaranteed cycle
        }
    }

    return g;
}

int main() {
    cout << "Random Graph Cycle Detection Experiment\n";
    srand(time(0));

    int V = 8;
    double edgeProb = 0.3; // 30% chance of edge existing
/*
    // Case 1: Acyclic random graph
    Graph acyclic = generateRandomGraph(V, edgeProb, false);
    cout << "\nAcyclic Random Graph:\n";
    acyclic.printGraph();
    cout << "Cycle? " << (acyclic.isCyclic() ? "Yes" : "No") << endl;
*/
    // Case 1:Random graph
   for(int i = 0 ; i<5;i++){
        Graph cyclic = generateRandomGraph(V, edgeProb,i%2);
        cout << "\n Random Graph:\n";
        cyclic.printGraph();
        cout << "Cycle? " << (cyclic.isCyclic() ? "Yes" : "No") << endl;

       
   }
    return 0;
}
