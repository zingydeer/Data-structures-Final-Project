#ifndef GRAPHBASE_H
#define GRAPHBASE_H

#include <vector>
#include <string>

using namespace std;

class GraphBase {
public:
    virtual void addVertex(string label) = 0;
    virtual void removeVertex(string label) = 0;
    virtual void addEdge(string label1, string label2, unsigned long weight) = 0;
    virtual void removeEdge(string label1, string label2) = 0;
    virtual unsigned long shortestPath(string startLabel, string endLabel, vector<string> &path) = 0;
    virtual ~GraphBase() {}
};

#endif