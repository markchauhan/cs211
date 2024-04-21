/*main.cpp*/

//
// Program to input Nodes (positions), Buildings and Footways
// from an Open Street Map file.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211: Winter 2023
// 

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <limits>

#include "graph.h"
#include "dist.h"
#include "building.h"
#include "buildings.h"
#include "footway.h"
#include "footways.h"
#include "node.h"
#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

const double INF = numeric_limits<double>::infinity();
class prioritize{
   public: 
      bool operator() (const pair<long long, int>& p1, const pair<long long, int>& p2) const{
         return p1.second > p2.second;
         }
   };

vector<long long> findFootway(long long id, Footways footways){
  vector<long long> nodeids;
  for(Footway F : footways.MapFootways){
        if(F.ID == id){
          nodeids = F.NodeIDs;
          break;
        }
      }
  return nodeids;
}

vector<long long> dijkstra(graph& G, long long& start, long long& end, map<long long, double>& distances, map<long long, long long>& predecessors, int& nodes_visited) {
  
  vector<long long> visited; 
  

  priority_queue<pair<long long, int>, vector<pair<long long, int>>, prioritize> unvisitedQueue;


  
   for (long long v : G.getVertices()) {
      distances[v] = INF;
    
   }

   distances[start] = 0;

   unvisitedQueue.push(make_pair(start, 0));

   while (!unvisitedQueue.empty()) {

      pair<long long, int> currentV = unvisitedQueue.top();

      unvisitedQueue.pop();

      if (currentV.second == INF) {
         break;
         } 

   else {
      for (long long adjV : G.neighbors(currentV.first)) {

         double edgeWeight;
         G.getWeight(currentV.first, adjV, edgeWeight);
         double alternativePathDistance = distances[currentV.first] + edgeWeight;

      if (alternativePathDistance <= distances[adjV]) {
         distances[adjV] = alternativePathDistance;
         predecessors[adjV] = currentV.first;
         unvisitedQueue.push(make_pair(adjV, alternativePathDistance));
         }

  }
  }


  if (find(visited.begin(), visited.end(), currentV.first) == visited.end()) {
      visited.push_back(currentV.first);
    }

   }

nodes_visited = visited.size();
return visited;


}
void buildGraph(Nodes nodes, Footways footways, graph& G){
 
  //adds vertices
  for (auto& pair : nodes){
    G.addVertex(pair.first);
    // latitude.push_back(pair.second.getLat());
    // longitude.push_back(pair.second.getLon());
  }

  //adds edges

  for(Footway& F : footways.MapFootways){

    int maximum = F.NodeIDs.size() - 1;

    for(int i = 0; i < maximum; i++){

      double lat1 = 0.0; 
      double lat2 = 0.0; 
      double lon1 = 0.0;
      double lon2 = 0.0;
      bool check;

      nodes.find(F.NodeIDs[i], lat1, lon1, check);
      nodes.find(F.NodeIDs[i+1], lat2, lon2, check);

      double distance1 = distBetween2Points(lat1, lon1, lat2, lon2);
      double distance2 = distBetween2Points(lat2, lon2, lat1, lon1);

      G.addEdge(F.NodeIDs[i], F.NodeIDs[i+1], distance1);
      G.addEdge(F.NodeIDs[i+1], F.NodeIDs[i], distance2);
    }

  }


  
  }

void printGraph(long long id, vector<long long>& NodeIDs, graph G){


  cout << "Graph check of Footway ID " << id << endl; 
  int maximum = NodeIDs.size() - 1;

  for(int i = 0; i < maximum; i++){
      double weight1 = 0.0;
      double weight2 = 0.0;
      bool check;

      check = G.getWeight(NodeIDs[i], NodeIDs[i+1], weight1);
      check = G.getWeight(NodeIDs[i+1], NodeIDs[i], weight2);
      cout << " Edge: (" << NodeIDs[i] << ", " << NodeIDs[i+1]<< ", " << weight1 << ")" << endl;
      cout << " Edge: (" << NodeIDs[i+1] << ", " << NodeIDs[i] << ", " << weight2 << ")" << endl;
   //   temp++;
    }

  }


void navigate(Buildings buildings, Nodes nodes, Footways footways, graph G){

    string start_name;
    string des_name;

    Building building(0, start_name, start_name);

    cout << "Enter start building name (partial or complete)>" << endl;
    getline(cin, start_name);
    
    bool start = buildings.search(start_name, building);

    if (start){
      double avglat = 0.0;
      double avglon = 0.0;

      building.navigatePrint(nodes, avglat, avglon);

      vector<long long> footwayIDs;
      
      for (Footway& F : footways.MapFootways){
        if (F.intersectWithBuilding(building)){
          footwayIDs.push_back(F.ID);}
          }
      
      double lat, lon; 
      bool test;
      long long footway_id;
      long long node_id;
      double min = INF;

      for (Footway& footway : footways.MapFootways){

        for(long long nodeid : footway.NodeIDs){
          nodes.find(nodeid, lat, lon, test);
          if(distBetween2Points(lat, lon, avglat, avglon) < min){
            min = distBetween2Points(lat, lon, avglat, avglon);
            footway_id = footway.ID;
            node_id = nodeid;
          }
        }
      }

      cout << " Closest footway ID " << footway_id << ", node ID " << node_id << ", distance " << min << endl;
     
     
      cout << "Enter destination building name (partial or complete)>" << endl;
      getline(cin, des_name);
      bool des = buildings.search(des_name, building);
  
  //
  // sort the footway ids, if any:
  //a

      if (des){
        double avglat_des = 0.0; 
        double avglon_des = 0.0; 

        building.navigatePrint(nodes, avglat_des, avglon_des);
         vector<long long> footwayIDs_des;
      
        for (Footway& F : footways.MapFootways){
        if (F.intersectWithBuilding(building)){
          footwayIDs_des.push_back(F.ID);}
          }
      
      double lat_des = 0.0;
      double lon_des = 0.0;
      bool test_des;
      long long footway_id_des;
      long long node_id_des;
      double min_des = INF;

      for (Footway& footway_des : footways.MapFootways){

        for(long long nodeid_des : footway_des.NodeIDs){

          nodes.find(nodeid_des, lat_des, lon_des, test_des);

          if(distBetween2Points(lat_des, lon_des, avglat_des, avglon_des) < min_des){
            min_des = distBetween2Points(lat_des, lon_des, avglat_des, avglon_des);
            footway_id_des = footway_des.ID;
            node_id_des = nodeid_des;
          }
        }
      }

      cout << " Closest footway ID " << footway_id_des << ", node ID " << node_id_des << ", distance " << min_des << endl;
      
      vector <long long> visited;
      map<long long, double> distance;
      map<long long, long long> predecessors;
      int nodes_visited;
      
     
       visited = dijkstra(G, node_id, node_id_des, distance, predecessors, nodes_visited);
     
       double minimum;

      for (auto kvpair : distance){
        if(kvpair.first == node_id_des){
          minimum = kvpair.second; 
        }
      }

      cout << "Shortest weighted path:" << endl; 
      

      if(minimum != INF){
        cout << " # nodes visited: " << nodes_visited << endl; 
        cout << " Distance: " << minimum << " miles" << endl;
        cout << " Path: ";
        vector <long long> path;
        long long c = node_id_des; 

      while(c != node_id){
        path.push_back(c);
        c = predecessors[c];
      }
        path.push_back(node_id);

        reverse(path.begin(), path.end());

        int maximum = path.size() - 1; 
        for(int i = 0; i < maximum; i++){
          cout << path[i] << "->";
                  }

        cout << node_id_des << endl;
      }
      else{
        cout << "**Sorry, destination unreachable" << endl;
      }

    
     

      
    }
    else{
      cout << "**Destination building not found" << endl; 
    }

  }
  else{
    cout << "**Start building not found" << endl; 
  }
  }

//
// main
//
int main()
{
  cout << setprecision(12);
  XMLDocument xmldoc;
  Nodes nodes;
  Buildings buildings;
  Footways footways;
  graph G;
  
  cout << "** NU open street map **" << endl;

  string filename;

  cout << endl;
  cout << "Enter map filename> " << endl;
  getline(cin, filename);

  //
  // 1. load XML-based map file 
  //
  if (!osmLoadMapFile(filename, xmldoc))
  {
    // failed, error message already output
    return 0;
  }
  
  //
  // 2. read the nodes, which are the various known positions on the map:
  //
  nodes.readMapNodes(xmldoc);

  //
  // NOTE: let's sort so we can use binary search when we need 
  // to lookup nodes.
  //
  nodes.sortByID();

  //
  // 3. read the university buildings:
  //
  buildings.readMapBuildings(xmldoc);

  //
  // 4. read the footways, which are the walking paths:
  //
  footways.readMapFootways(xmldoc);

  buildGraph(nodes, footways, G);
  //
  // 5. stats
  //
  cout << "# of nodes: " << nodes.getNumMapNodes() << endl;
  cout << "# of buildings: " << buildings.getNumMapBuildings() << endl;
  cout << "# of footways: " << footways.getNumMapFootways() << endl;
  cout << "# of graph vertices: " << G.NumVertices() << endl;		  
  cout << "# of graph edges: " << G.NumEdges() << endl;
  //
  // now let the user for search for 1 or more buildings:
  //
  while (true)
  {
    string name;

    cout << endl;
    cout << "Enter building name, * to list, @ to navigate, or $ to end> " << endl;

    getline(cin, name);

    if (name == "$") {
      break;
    }
    else if (name == "*") {
      buildings.print();
    }
    else if (name == "!") {
      vector<long long> node_ids = findFootway(986532630, footways);
      printGraph(986532630, node_ids, G);
    }
    else if (name == "@"){
      navigate(buildings, nodes, footways, G);
    }
    else {
      buildings.findAndPrint(name, nodes, footways);
    }

  }//while

  //
  // done:
  //
  cout << endl;
  cout << "** Done  **" << endl;
  cout << "# of calls to getID(): " << Node::getCallsToGetID() << endl;
  cout << "# of Nodes created: " << Node::getCreated() << endl;
  cout << "# of Nodes copied: " << Node::getCopied() << endl;
  cout << endl;

  return 0;
}