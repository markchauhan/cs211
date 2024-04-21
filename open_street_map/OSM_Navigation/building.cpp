/*building.cpp*/

//
// A building in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211: Winter 2023
// 

#include <iostream>

#include "building.h"

using namespace std;


//
// constructor
//
Building::Building(long long id, string name, string streetAddr)
  : ID(id), Name(name), StreetAddress(streetAddr)
{
  //
  // the proper technique is to use member initialization list above,
  // in the same order as the data members are declared:
  //
  //this->ID = id;
  //this->Name = name;
  //this->StreetAddress = streetAddr;

  // vector is default initialized by its constructor
}

pair<double, double> Building::getLocation(Nodes& nodes){
  double avgLat, avgLon = 0.0; 

  double sum_lat = 0.0; 
  double sum_lon = 0.0;
  
  vector <long long> nodeids; 
  nodeids = this->NodeIDs;

  for(long long nodeid : nodeids){
    double lat, lon;
    bool entrance;

    bool found = nodes.find(nodeid, lat, lon, entrance);

    if(found){
      sum_lat = sum_lat + lat;
      sum_lon = sum_lon + lon;
    }   

  }

  avgLat = sum_lat / this->NodeIDs.size();
  avgLon = sum_lon / this->NodeIDs.size();
  
  return make_pair(avgLat, avgLon);

}

//
// containsThisNode
//
// Returns true if the building's nodes contains the given node id,
// false if not.
//
bool Building::containsThisNode(long long nodeid)
{
  for (long long id : this->NodeIDs)
  {
    if (nodeid == id)
      return true;
  }

  // 
  // if get here, not found:
  //
  return false;
}

//
// print
// 
// prints information about a building --- id, name, etc. -- to
// the console. The function is passed the Nodes for searching 
// purposes.
//
void Building::print(Nodes& nodes)
{
  cout << this->Name << endl;
  cout << "Address: " << this->StreetAddress << endl;
  cout << "Building ID: " << this->ID << endl;

  //getLocation
  pair <double, double> avg;
  avg = this->getLocation(nodes); 
  cout << "Approximate location: (" << avg.first << ", " << avg.second << ")" << endl;

  cout << "Nodes:" << endl;
  for (long long nodeid : this->NodeIDs)
  {
    cout << "  " << nodeid << ": ";

    double lat = 0.0;
    double lon = 0.0;
    bool entrance = false;

    bool found = nodes.find(nodeid, lat, lon, entrance);

    if (found) {
      cout << "(" << lat << ", " << lon << ")";

      if (entrance)
        cout << ", is entrance";

      cout << endl;
    }
    else {
      cout << "**NOT FOUND**" << endl;
    }
  }//for
}

void Building::navigatePrint(Nodes nodes, double& avglat, double& avglon){

  cout << " Name: " << this->Name << endl;
  pair <double, double> loc;
  loc =  this->getLocation(nodes);
  avglat = loc.first;
  avglon = loc.second;
  cout << " Approximate location: (" << loc.first << ", " << loc.second << ")" << endl;

}
//
// adds the given nodeid to the end of the vector.
//
void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}
