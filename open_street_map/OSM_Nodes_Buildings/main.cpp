/*main.cpp*/

//
// HEADER COMMENT
//

#include <iostream>
#include <string>

#include "building.h"
#include "buildings.h"
#include "footway.h"
#include "node.h"
#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"


using namespace std;
using namespace tinyxml2;


int main()
{
  XMLDocument xmldoc;
  Nodes nodes;
  Buildings buildings; 
  
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
  // 2. read the nodes, which are positions on the map:
  //
  nodes.readMapNodes(xmldoc);
 // nodes.sortByID();


  //
  // 3. read the buildings contained in the map:
  //
  buildings.readMapBuildings(xmldoc); 

  //
  // 4. read the footways / paths on the map:
  //
  //
  // TODO!
  //


  //
  // 5. stats
  //
  cout << "# of nodes: " << nodes.getNumMapNodes() << endl;
  cout << "# of buildings: " << buildings.getNumMapBuildings() << endl; 
  //
  // TODO: output other stats for buildings and footways:
  //

  //
  // now let the user for search for 1 or more buildings:
  //

  
  while (true)
  {
    string name;

    cout << endl;
    cout << "Enter building name (partial or complete), or * to list, or $ to end> " << endl;

    getline(cin, name);  // use getline() because user may type multiple words:

    if (name == "$") {
      break;
    }
    else if (name == "*")
    {
      for (Building temp : buildings.MapBuildings){
        cout << temp.ID << ": " << temp.Name << ", " << temp.StreetAddress << endl; 
      }
      
    }
    else {
     
      // 
      // find every building that CONTAINS name string, output in the 
      // order they appear in the vector (which is the order they appear
      // in the map file):
      //
      for (Building j : buildings.MapBuildings){

        size_t found = j.Name.find(name);
        if (found != string::npos){
          cout << j.Name << endl; 
          cout << "Address: " << j.StreetAddress << endl; 
          cout << "Building ID: " << j.ID << endl;
          cout << "Nodes: " << endl;

          for (int i = 0; i < j.NodeIDs.size(); i++){

          double lat; 
          double lon; 
          bool isEntrance; 
          bool check; 

          check = nodes.find(j.NodeIDs[i], lat, lon, isEntrance); 

            if (check == true){
              if(isEntrance == true){
                cout << " "<< j.NodeIDs[i]<< ": (" << lat << ", " << lon << ")" << ", is entrance" << endl; 
              }
              else{  
                cout << " " << j.NodeIDs[i] << ": (" << lat << ", " << lon << ")" << endl; 
              }
            }
            else{
              cout << "**NOT FOUND**" << endl;
            }

          }
        }
      }

    
      // 
      // TODO: use string's find() function to see if building name
      // contains the name string input by the user. NOTE that find()
      // returns string::npos if NOT found --- it's not a boolean
      // function, but instead returns "no position" if not found.
      //
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
