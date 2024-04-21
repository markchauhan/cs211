/*buildings.cpp*/

// Prof. Joe Hummel
// Mark Chauhan
// Northwestern University
// CS 211: Winter 2023
// 

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

#include "building.h"
#include "buildings.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2; 

Buildings::Buildings() {}
void Buildings::readMapBuildings(XMLDocument& xmldoc){
    
    XMLElement* osm  = xmldoc.FirstChildElement("osm");
    assert(osm != nullptr);

    XMLElement* way = osm->FirstChildElement("way");


    while (way != nullptr){
        
        if(osmContainsKeyValue(way, "building", "university")){
            string name = osmGetKeyValue(way, "name");
            string streetAddr = osmGetKeyValue(way, "addr:housenumber") + " " + osmGetKeyValue(way, "addr:street");
            
            Building B = Building(way->FindAttribute("id")->Int64Value(), name , streetAddr);  

            XMLElement* nd = way->FirstChildElement("nd");

            while (nd != nullptr){
                const XMLAttribute*ndref = nd->FindAttribute("ref");
                assert(ndref != nullptr);

                long long id = ndref->Int64Value(); 

                B.add(id);

                //advance to next node ref
                nd = nd->NextSiblingElement("nd");
            }

            //Push into Buildings vector
            this->MapBuildings.push_back(B);
        }

        way = way->NextSiblingElement("way");
    }

}
     //
     // accessors / getters
     //
int Buildings::getNumMapBuildings(){
    return (int) this->MapBuildings.size(); 
}