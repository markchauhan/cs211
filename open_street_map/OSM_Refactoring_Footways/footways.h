/*footways.h*/

//
// A collection of footways in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211: Winter 2023
// 

#pragma once

#include <vector>
#include <iostream>

#include "footway.h"
#include "node.h"
#include "nodes.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


//
// Keeps track of all the footways in the map.
//
class Footways
{
public:
  vector<Footway> MapFootways;

  //
  // readMapFootways
  //
  // Given an XML document, reads through the document and 
  // stores all the footways into the given vector.
  //
  void readMapFootways(XMLDocument& xmldoc);

  bool find(long long nodeid, long long& footwayid);


  //
  // accessors / getters
  //
  int getNumMapFootways();

};


