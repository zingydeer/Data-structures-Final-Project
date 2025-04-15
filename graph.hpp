#ifndef GRAPH_H
#define GRAPH_H

#include "GraphBase.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <limits>
#include <queue>
#include <algorithm>

using namespace std;

class Graph : public GraphBase {
private:
    // Vertex class to represent nodes in the graph
    class Vertex {
    public:
        string label;
        unordered_map<Vertex*, unsigned long> neighbors; // Adjacent vertices and weights

        Vertex(string label) : label(label) {}
    };

    // Map to store vertices by their labels for quick lookup
    unordered_map<string, Vertex*> vertices;

    // Helper method to find a vertex by label
    Vertex* findVertex(const string& label) const;

public:
    // Constructor
    Graph();

    // Destructor - important for memory management
    ~Graph();

    // Required methods from GraphBase
    void addVertex(string label) override;
    void removeVertex(string label) override;
    void addEdge(string label1, string label2, unsigned long weight) override;
    void removeEdge(string label1, string label2) override;
    unsigned long shortestPath(string startLabel, string endLabel, vector<string>& path) override;

    // Additional utility methods
    void printGraph() const; // For debugging
};

#endif // GRAPH_H