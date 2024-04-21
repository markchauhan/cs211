

#include <iostream>
#include <unordered_map>
#include <set>
#include <stdexcept>
#include <vector>

#include "graph.h"

using namespace std; 

//returns the number of vertices

graph::graph(){
    temp_graph = unordered_map<string, vector<pair<string, double>>> ();
}

int graph::NumVertices() const{
    return temp_graph.size();
}

int graph::NumEdges() const{

    int total = 0;

    for(auto& vertex : temp_graph){

        total += vertex.second.size();
    
    }

    return total; 
}

bool graph::addVertex(string v){

    if(temp_graph.find(v) == temp_graph.end()){
        temp_graph[v] = vector<pair<string, double>>();
        return true;
    }

    return false;
}

bool graph::addEdge(string from, string to, double weight){
    
    if(temp_graph.find(from) != temp_graph.end() && temp_graph.find(to) != temp_graph.end()){
        auto& neighbors = temp_graph[from];

        for(auto& neighbor : neighbors){

            if(neighbor.first == to){
                neighbor.second = weight;
                return true;
            }
        }
        neighbors.push_back(make_pair(to, weight));
        return true;

    }
    return false;
}

bool graph::getWeight(string from, string to, double& weight) const{

    if(temp_graph.find(from) != temp_graph.end()){
        auto& neighbors = temp_graph.at(from);
        
        for (auto& neighbor : neighbors){
            if(neighbor.first == to){
                weight = neighbor.second;
                return true;
            }
        }
    }

    return false;
}

set<string> graph::neighbors(string v) const{
    set<string> result; 

    auto ite = temp_graph.find(v);

    if(ite == temp_graph.end()){
        return result;
    }

    auto& edges = ite->second;

    for(auto& edge : edges){
        result.insert(edge.first);
    }

    return result;
}

vector<string> graph::getVertices() const{

    vector<string> vertices; 
    for (auto& v : temp_graph){
        vertices.push_back(v.first);
    }

    return vertices;
}

void graph::print(ostream& output) const{

    output << "***************************************************" << endl;
    output << "********************* GRAPH ***********************" << endl;

    output << "**Num vertices " << NumVertices() << endl; 
    output << "**Num edges " << NumEdges() << endl; 
    output << "**Vertices map**" << endl;

    for (auto& vertex : temp_graph){
        output << vertex.first << " ";  
    }

    output << endl; 
    output << "**Edges map**" << endl;

    for (auto& vertex : temp_graph){
        auto& neighbors = vertex.second;

        for(auto& neighbor : neighbors){
            output << "(" << vertex.first << "," << neighbor.first << "," << neighbor.second << ") "; 
        }
    }

    output << endl << "**************************************************" << endl;
}