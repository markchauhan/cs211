/*building.cpp*/

//
// A building in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211: Winter 2023
// 

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

//
// adds the given nodeid to the end of the vector.
//

void Building::print(Nodes& nodes, Footways& footways){
  vector<long long> footway_list; 
 
  cout << this->Name << endl;
  cout << "Address: " << this->StreetAddress << endl;
  cout << "Building ID: " << this->ID << endl;

  cout << "Nodes:" << endl;
  int count;

  long long footwayid;
  
  
  for (long long nodeid : this->NodeIDs){
      cout << "  " << nodeid << ": ";

      double lat = 0.0;
      double lon = 0.0;
      bool entrance = false;

      bool found = nodes.find(nodeid, lat, lon, entrance);

      if (found) {
        cout << "(" << lat << ", " << lon << ")";

        if (entrance){
          cout << ", is entrance";
          }
      cout << endl;
    
      //finds in footways
      bool check = footways.find(nodeid, footwayid);

        if(check){
          //If past value == footwayid then skip
          // if(checker == footwayid){
          //   continue;
          // }

          footway_list.push_back(footwayid);   
  
        }
            }
            else {
              cout << "**NOT FOUND**" << endl;
            }
          }//for
          
        cout << "Footways that intersect:" << endl;

        //sorts the vector
        sort(footway_list.begin(), footway_list.end());
        //If Empty
        if(footway_list.size() == 0){
          cout << " None" << endl;
        }
        else{
        //Parses through vector
        
        for(int j = 0; j < footway_list.size(); j++){
          if(j != 0){
            if(footway_list.at(j-1) == footway_list.at(j)){
              continue;
            }
            cout << " " << "Footway" << " "<< footway_list.at(j) << endl; 
            }
          else{
            cout << " " << "Footway" << " "<< footway_list.at(j) << endl;
          }

          }
        }

          }
          



void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}
