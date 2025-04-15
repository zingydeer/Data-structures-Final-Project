#include "Graph.hpp"
#include <iostream>
#include <queue>
#include <set>
#include <limits>
#include <algorithm>

using namespace std;

// Constructor
Graph::Graph() {}

// Destructor
Graph::~Graph() {
    // Clean up memory by deleting all vertex objects
    for (auto& pair : vertices) {
        delete pair.second;
    }
    vertices.clear();
}

// Helper method to find a vertex by label
Graph::Vertex* Graph::findVertex(const string& label) const {
    auto it = vertices.find(label);
    if (it != vertices.end()) {
        return it->second;
    }
    return nullptr;
}

// Add a vertex to the graph
void Graph::addVertex(string label) {
    // Check if vertex with this label already exists
    if (findVertex(label) != nullptr) {
        throw runtime_error("Vertex with label '" + label + "' already exists");
    }
    
    // Create new vertex and add to the map
    Vertex* newVertex = new Vertex(label);
    vertices[label] = newVertex;
}

// Remove a vertex from the graph
void Graph::removeVertex(string label) {
    Vertex* vertexToRemove = findVertex(label);
    
    // Check if vertex exists
    if (vertexToRemove == nullptr) {
        throw runtime_error("Vertex with label '" + label + "' not found");
    }
    
    // Remove all edges connected to this vertex
    for (auto& pair : vertices) {
        Vertex* currentVertex = pair.second;
        currentVertex->neighbors.erase(vertexToRemove);
    }
    
    // Remove the vertex from the map and delete it
    vertices.erase(label);
    delete vertexToRemove;
}

// Add an edge between two vertices
void Graph::addEdge(string label1, string label2, unsigned long weight) {
    // Find the vertices
    Vertex* vertex1 = findVertex(label1);
    Vertex* vertex2 = findVertex(label2);
    
    // Check if both vertices exist
    if (vertex1 == nullptr) {
        throw runtime_error("Vertex with label '" + label1 + "' not found");
    }
    if (vertex2 == nullptr) {
        throw runtime_error("Vertex with label '" + label2 + "' not found");
    }
    
    // Check if vertices are the same (no self-loops allowed)
    if (label1 == label2) {
        throw runtime_error("Cannot add edge from a vertex to itself");
    }
    
    // Check if edge already exists
    if (vertex1->neighbors.find(vertex2) != vertex1->neighbors.end()) {
        throw runtime_error("Edge between '" + label1 + "' and '" + label2 + "' already exists");
    }
    
    // Add the edge (in both directions as it's an undirected graph)
    vertex1->neighbors[vertex2] = weight;
    vertex2->neighbors[vertex1] = weight;
}

// Remove an edge between two vertices
void Graph::removeEdge(string label1, string label2) {
    // Find the vertices
    Vertex* vertex1 = findVertex(label1);
    Vertex* vertex2 = findVertex(label2);
    
    // Check if both vertices exist
    if (vertex1 == nullptr) {
        throw runtime_error("Vertex with label '" + label1 + "' not found");
    }
    if (vertex2 == nullptr) {
        throw runtime_error("Vertex with label '" + label2 + "' not found");
    }
    
    // Check if edge exists
    if (vertex1->neighbors.find(vertex2) == vertex1->neighbors.end()) {
        throw runtime_error("Edge between '" + label1 + "' and '" + label2 + "' not found");
    }
    
    // Remove the edge (from both directions as it's an undirected graph)
    vertex1->neighbors.erase(vertex2);
    vertex2->neighbors.erase(vertex1);
}

// Find shortest path using Dijkstra's algorithm
unsigned long Graph::shortestPath(string startLabel, string endLabel, vector<string>& path) {
    // Clear the path vector
    path.clear();
    
    // Find the start and end vertices
    Vertex* startVertex = findVertex(startLabel);
    Vertex* endVertex = findVertex(endLabel);
    
    // Check if both vertices exist
    if (startVertex == nullptr) {
        throw runtime_error("Start vertex with label '" + startLabel + "' not found");
    }
    if (endVertex == nullptr) {
        throw runtime_error("End vertex with label '" + endLabel + "' not found");
    }
    
    // If start and end vertices are the same, return 0 and add the label to the path
    if (startVertex == endVertex) {
        path.push_back(startLabel);
        return 0;
    }

    // Map to store distances from start vertex to each vertex
    unordered_map<Vertex*, unsigned long> distance;
    
    // Map to store previous vertex in the shortest path
    unordered_map<Vertex*, Vertex*> previous;
    
    // Set of vertices to be processed
    set<Vertex*> unvisited;
    
    // Initialize distances
    for (auto& pair : vertices) {
        Vertex* vertex = pair.second;
        distance[vertex] = numeric_limits<unsigned long>::max(); // Set to infinity
        previous[vertex] = nullptr;
        unvisited.insert(vertex);
    }
    
    // Set distance to start vertex as 0
    distance[startVertex] = 0;
    
    // Custom comparator for priority queue
    auto comparator = [&distance](Vertex* a, Vertex* b) {
        return distance[a] > distance[b]; // Min-heap based on distance
    };
    
    // Priority queue to get vertex with minimum distance
    priority_queue<Vertex*, vector<Vertex*>, decltype(comparator)> pq(comparator);
    pq.push(startVertex);
    
    // Main Dijkstra's algorithm loop
    while (!pq.empty() && !unvisited.empty()) {
        // Get vertex with minimum distance
        Vertex* current = pq.top();
        pq.pop();
        
        // If we reached the end vertex, break
        if (current == endVertex) {
            break;
        }
        
        // Remove from unvisited if it's there (might not be due to duplicates in priority queue)
        auto unvisitedIt = unvisited.find(current);
        if (unvisitedIt == unvisited.end()) {
            continue; // Already processed
        }
        unvisited.erase(unvisitedIt);
        
        // Check all neighbors of current vertex
        for (auto& neighborPair : current->neighbors) {
            Vertex* neighbor = neighborPair.first;
            unsigned long weight = neighborPair.second;
            
            // If neighbor is already visited, skip
            if (unvisited.find(neighbor) == unvisited.end()) {
                continue;
            }
            
            // Calculate new distance to neighbor
            unsigned long newDistance = distance[current] + weight;
            
            // If new distance is smaller, update distance and previous
            if (newDistance < distance[neighbor]) {
                distance[neighbor] = newDistance;
                previous[neighbor] = current;
                pq.push(neighbor); // Add to priority queue for processing
            }
        }
    }
    
    // If end vertex is not reachable from start vertex
    if (distance[endVertex] == numeric_limits<unsigned long>::max()) {
        throw runtime_error("No path exists from '" + startLabel + "' to '" + endLabel + "'");
    }
    
    // Reconstruct the path from end to start
    vector<string> reversePath;
    Vertex* current = endVertex;
    
    while (current != nullptr) {
        reversePath.push_back(current->label);
        current = previous[current];
    }
    
    // Reverse the path to get from start to end
    path.assign(reversePath.rbegin(), reversePath.rend());
    
    // Return the total distance
    return distance[endVertex];
}

// Print the graph (for debugging)
void Graph::printGraph() const {
    for (auto& pair : vertices) {
        cout << "Vertex " << pair.first << " is connected to:" << endl;
        for (auto& neighborPair : pair.second->neighbors) {
            cout << "  " << neighborPair.first->label << " with weight " << neighborPair.second << endl;
        }
    }
}