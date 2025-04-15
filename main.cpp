#include "Graph.hpp"
#include <iostream>
#include <iomanip>
#include <tuple>

using namespace std;

// Function to create and test a graph based on the examples
void testGraph(const vector<string>& vertices,
               const vector<tuple<string, string, unsigned long>>& edges,
               const vector<pair<string, string>>& pathTests) {

    // Create a new graph
    Graph g;

    //we bussin

    cout << "Creating graph..." << endl;

    // Add vertices
    for (const auto& label : vertices) {
        g.addVertex(label);
    }

    // Add edges
    for (const auto& edge : edges) {
        string from = get<0>(edge);
        string to = get<1>(edge);
        unsigned long weight = get<2>(edge);
        g.addEdge(from, to, weight);
    }

    // Print the graph for debugging
    cout << "Graph structure:" << endl;
    g.printGraph();
    cout << endl;

    // Test shortest paths
    for (const auto& test : pathTests) {
        string start = test.first;
        string end = test.second;

        vector<string> path;
        try {
            unsigned long distance = g.shortestPath(start, end, path);

            cout << "Shortest path from " << start << " to " << end << ":" << endl;
            cout << "Distance: " << distance << endl;
            cout << "Path: ";
            for (size_t i = 0; i < path.size(); ++i) {
                cout << path[i];
                if (i < path.size() - 1) {
                    cout << " -> ";
                }
            }
            cout << endl << endl;
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}

int main() {
    // Test Graph 1 from the example
    vector<string> vertices1 = {"1", "2", "3", "4", "5", "6"};
    vector<tuple<string, string, unsigned long>> edges1 = {
        {"1", "2", 7},
        {"1", "3", 9},
        {"1", "6", 14},
        {"2", "3", 10},
        {"2", "4", 15},
        {"3", "4", 11},
        {"3", "6", 2},
        {"4", "5", 6},
        {"5", "6", 9}
    };
    vector<pair<string, string>> pathTests1 = {
        {"1", "5"}
    };

    cout << "========== Testing Graph 1 ==========" << endl;
    testGraph(vertices1, edges1, pathTests1);

    // Test Graph 2 from the example
    vector<string> vertices2 = {"BSN", "LIB", "ENB", "MSC", "CAS", "SUB", "SUN"};
    vector<tuple<string, string, unsigned long>> edges2 = {
        {"BSN", "LIB", 871},
        {"BSN", "CAS", 1672},
        {"BSN", "MSC", 2355},
        {"SUN", "SUB", 1265},
        {"LIB", "MSC", 1615},
        {"LIB", "SUN", 1847},
        {"ENB", "SUN", 2885},
        {"ENB", "CAS", 454},
        {"ENB", "LIB", 1078}
    };
    vector<pair<string, string>> pathTests2 = {
        {"ENB", "SUN"},
        {"LIB", "CAS"}
    };

    cout << "========== Testing Graph 2 ==========" << endl;
    testGraph(vertices2, edges2, pathTests2);

    return 0;
}