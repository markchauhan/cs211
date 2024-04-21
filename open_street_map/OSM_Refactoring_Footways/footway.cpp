/*footway.cpp*/

//
// A footway / path in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211: Winter 2023
// 

#include "footway.h"

using namespace std;


//
// constructor
//
Footway::Footway(long long id)
  : ID(id)
{
  //
  // the proper technique is to use member initialization list above,
  // in the same order as the data members are declared:
  //
  //this->ID = id;

  // vector is default initialized by its constructor
}



bool Footway::find(long long nodeid, long long& footwayid){
//assigns id each value in NodeIds
  for (long long id : this->NodeIDs){
    //if id == nodeid sets footwayid to this->ID
    //returns true
    if (id == nodeid){
      footwayid = this->ID;
      return true;
    }
    
  }

  return false;

}

//
// adds the given nodeid to the end of the vector
//


void Footway::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}
