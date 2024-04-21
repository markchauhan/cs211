#include <iostream>
#include <string>


#include "gtest/gtest.h"
#include "graph.h"

using namespace std;


// Each Unit Test will look through each function 

//Basic Graph
TEST(testing, test01)
{
	graph G; 

  ASSERT_TRUE(G.NumVertices() == 0);
  ASSERT_TRUE(G.NumEdges() == 0);

  //addVertices
  ASSERT_TRUE(G.addVertex("A"));
  ASSERT_TRUE(G.addVertex("B"));
  ASSERT_TRUE(G.addVertex("C"));
  ASSERT_TRUE(G.addVertex("D"));
  ASSERT_TRUE(G.addVertex("E"));
  ASSERT_TRUE(G.addVertex("F"));
  ASSERT_TRUE(G.addVertex("G"));
  ASSERT_TRUE(G.addVertex("H"));

  

  //addEdges
  ASSERT_TRUE(G.addEdge("A", "B", 2.5));
  ASSERT_TRUE(G.addEdge("A", "D", 3.4));
  ASSERT_TRUE(G.addEdge("B", "C", 9.3));
  ASSERT_TRUE(G.addEdge("B", "H", 2.5));
  ASSERT_TRUE(G.addEdge("C", "G", 9.3));
  ASSERT_TRUE(G.addEdge("C", "E", 8.4));
  ASSERT_TRUE(G.addEdge("D", "F", 4.3));
  ASSERT_TRUE(G.addEdge("D", "E", 9.3));
  ASSERT_TRUE(G.addEdge("E", "G", 7.3));
  ASSERT_TRUE(G.addEdge("E", "B", 4.3));
  ASSERT_TRUE(G.addEdge("F", "A", 9.3));
  ASSERT_TRUE(G.addEdge("F", "G", 3.4));
  ASSERT_TRUE(G.addEdge("G", "F", 5.4));
  ASSERT_TRUE(G.addEdge("G", "A", 3.4));
  ASSERT_TRUE(G.addEdge("H", "C", 6.4));
  ASSERT_TRUE(G.addEdge("H", "B", 5.3));

  //tests repetivness
  ASSERT_TRUE(G.addEdge("A", "B", 2.5));
  ASSERT_FALSE(G.addVertex("A"));

  //Looks if it connects fo false 
  ASSERT_FALSE(G.addEdge("B", "Q", 3));
  ASSERT_FALSE(G.addEdge("Q", "C", 5.3));

  ASSERT_TRUE(G.NumVertices() == 8);
  ASSERT_TRUE(G.NumEdges() == 16);
  
}

//test Functions returning false
TEST(testing, test02)
{
	graph G; 

  ASSERT_TRUE(G.NumVertices() == 0);
  ASSERT_TRUE(G.NumEdges() == 0);

  ASSERT_TRUE(G.addVertex("A"));
  ASSERT_TRUE(G.addVertex("B"));
  ASSERT_TRUE(G.addVertex("C"));
  ASSERT_TRUE(G.addVertex("D"));
  ASSERT_TRUE(G.addVertex("E"));
  ASSERT_TRUE(G.addVertex("F"));
  ASSERT_TRUE(G.addVertex("G"));
  ASSERT_TRUE(G.addVertex("H"));

  ASSERT_FALSE(G.addVertex("A"));

  ASSERT_FALSE(G.addEdge("A", "Q", 1.2));
  ASSERT_FALSE(G.addEdge("Q", "B", 4.4));
  ASSERT_FALSE(G.addEdge("D", "L", 1.2));
  ASSERT_FALSE(G.addEdge("L", "F", 4.4));
  ASSERT_FALSE(G.addEdge("R", "L", 1.2));
  ASSERT_FALSE(G.addEdge("C", "R", 4.4));
  ASSERT_FALSE(G.addEdge("B", "Z", 1.2));
  ASSERT_FALSE(G.addEdge("E", "Z", 4.4));
  ASSERT_FALSE(G.addEdge("0", "B", 1.2));
  ASSERT_FALSE(G.addEdge("B", "T", 4.4));
  ASSERT_FALSE(G.addEdge("P", "W", 1.2));
  ASSERT_FALSE(G.addEdge("F", "p", 4.4));


}

/*test getVertices( ) on an empty graph, *test_1
neighbors(v) on an empty graph / *test2
where v does not exist /         *test3
 where v has no neighbors        *test4  
*/

TEST(testing, test03){
  graph G; 

  vector<string> test1 = G.getVertices();
  ASSERT_TRUE(test1.empty());

  set<string> test2 = G.neighbors("A");
  ASSERT_TRUE(test2.empty());

  G.addVertex("A"); 
  set<string> test4 = G.neighbors("A");
  ASSERT_TRUE(test4.empty());

  G.addVertex("A"); 
  set<string> test3 = G.neighbors("Q");
  ASSERT_TRUE(test3.empty());

}

//The following are test of the indivdual functions OR
//Tests of all the functions in various combinations

//NumVertices()
TEST(testing, test04){
  graph G;

  ASSERT_TRUE(G.NumVertices() == 0);
  G.addVertex("A");
  ASSERT_TRUE(G.NumVertices() == 1);
  G.addVertex("B");
  G.addVertex("C");
  G.addVertex("D");
  G.addVertex("E");
  G.addVertex("F");
  G.addVertex("G");
  G.addVertex("H");

  ASSERT_TRUE(G.NumVertices() == 8);

  G.addVertex("B");
  G.addVertex("C");
  G.addVertex("D");
  G.addVertex("E");
  G.addVertex("F");
  G.addVertex("G");
  G.addVertex("H");

  ASSERT_TRUE(G.NumVertices() == 8);

}

//NumEdges()
TEST(testing, test05){
  graph G; 

  ASSERT_TRUE(G.NumEdges() == 0);

  G.addVertex("A");
  G.addVertex("B");

  G.addEdge("A", "B", 3.4);
  G.addEdge("B", "A", 4.3);
  
  ASSERT_TRUE(G.NumEdges() == 2);

  G.addVertex("C");
  G.addVertex("D");
  G.addVertex("E");
  G.addVertex("F");
  G.addVertex("G");
  G.addVertex("H");

  G.addEdge("C", "A", 2.4);
  G.addEdge("D", "B", 3.56);
  G.addEdge("E", "F", 1.23);
  G.addEdge("F", "C", 2.31);
  G.addEdge("G", "D", 2.43);
  G.addEdge("H", "G", 1.23);
  G.addEdge("E", "C", 1.23);
  G.addEdge("F", "B", 1.23);
  G.addEdge("E", "H", 3.42);
  G.addEdge("H", "C", 32.3);

  ASSERT_TRUE(G.NumEdges() == 12);

}

//addVertex()
TEST(testing, test06){
  graph G;

  ASSERT_TRUE(G.addVertex("A")); 
  ASSERT_TRUE(G.addVertex("B")); 
  ASSERT_TRUE(G.addVertex("C")); 
  ASSERT_TRUE(G.addVertex("D")); 
  ASSERT_TRUE(G.addVertex("E")); 
  ASSERT_TRUE(G.addVertex("F")); 
  ASSERT_TRUE(G.addVertex("G")); 
  ASSERT_TRUE(G.addVertex("H")); 

  ASSERT_FALSE(G.addVertex("A")); 
  ASSERT_FALSE(G.addVertex("B")); 
  ASSERT_FALSE(G.addVertex("C")); 
  ASSERT_FALSE(G.addVertex("D")); 
  ASSERT_FALSE(G.addVertex("E")); 
  ASSERT_FALSE(G.addVertex("F")); 
  ASSERT_FALSE(G.addVertex("G")); 
  ASSERT_FALSE(G.addVertex("H")); 

}

//addEdge()
TEST(testing, test07){

  graph G; 

  ASSERT_FALSE(G.addEdge("F", "C", 4.0));

  G.addVertex("A");

  ASSERT_FALSE(G.addEdge("A", "C", 5.0));

  G.addVertex("B");
  G.addVertex("C");
  G.addVertex("D");
  G.addVertex("E");
  G.addVertex("F");
  G.addVertex("G");
  G.addVertex("H");

  ASSERT_TRUE(G.addEdge("B", "H", 8.3));
  ASSERT_TRUE(G.addEdge("A", "C", 7.3));
  ASSERT_TRUE(G.addEdge("C", "A", 2.4));
  ASSERT_TRUE(G.addEdge("D", "B", 3.56));
  ASSERT_TRUE(G.addEdge("E", "F", 1.23));
  ASSERT_TRUE(G.addEdge("F", "C", 2.31));
  ASSERT_TRUE(G.addEdge("G", "D", 2.43));
  ASSERT_TRUE(G.addEdge("H", "G", 1.23));
  ASSERT_TRUE(G.addEdge("E", "C", 1.23));
  ASSERT_TRUE(G.addEdge("F", "B", 1.23));
  ASSERT_TRUE(G.addEdge("E", "H", 3.42));
  ASSERT_TRUE(G.addEdge("H", "C", 32.3));
  
}

//getWeight()
TEST(testing, test08){

  graph G;
  double test;
  
  ASSERT_FALSE(G.getWeight("H", "B", test));

  G.addVertex("A");
  G.addVertex("B");
  G.addVertex("C");
  G.addVertex("D");
  G.addVertex("E");
  G.addVertex("F");
  G.addVertex("G");
  G.addVertex("H");

  ASSERT_FALSE(G.getWeight("H", "B", test));

  G.addEdge("B", "H", 8.3);
  G.addEdge("A", "C", 7.3);
  G.addEdge("C", "A", 2.4);
  G.addEdge("D", "B", 3.56);
  G.addEdge("E", "F", 1.23);
  G.addEdge("F", "C", 2.31);
  G.addEdge("G", "D", 2.43);
  G.addEdge("G", "H", 1.23);
  G.addEdge("E", "C", 1.23);
  G.addEdge("F", "B", 1.23);
  G.addEdge("E", "H", 3.42);
  G.addEdge("H", "C", 32.3);
  
  ASSERT_TRUE(G.getWeight("H", "C", test));
  ASSERT_TRUE(test == 32.3);

  ASSERT_TRUE(G.getWeight("E", "H", test));
  ASSERT_TRUE(test == 3.42);

  ASSERT_TRUE(G.getWeight("F", "C", test));
  ASSERT_TRUE(test == 2.31);


}

//neighbors
TEST(testing, test09){

  graph G; 

  ASSERT_TRUE(G.neighbors("Q").empty());
  ASSERT_TRUE(G.neighbors("B").empty());

  G.addVertex("A");
  G.addVertex("B");
  G.addVertex("C");
  G.addVertex("D");
  G.addVertex("E");
  G.addVertex("F");
  G.addVertex("G");
  G.addVertex("H");
  

  G.addEdge("A", "B", 2.4);
  G.addEdge("D", "A", 7.3);
  G.addEdge("C", "A", 1.8);
  G.addEdge("H", "A", 5.56);
  G.addEdge("G", "H", 1.23);
  G.addEdge("A", "C", 4.2);
  G.addEdge("G", "C", 9.3);
  G.addEdge("A", "G", 8.3);
  G.addEdge("F", "B", 1.23);
  G.addEdge("A", "E", 3.4);
  G.addEdge("E", "B", 35.6);
  G.addEdge("A", "C", 32.3);

  set<string> test{"B", "C", "G", "E", "C"}; 
  ASSERT_TRUE(G.neighbors("A") == test);


  G.addVertex("R");
  ASSERT_TRUE(G.neighbors("R").empty());

}

//getVertices()
TEST(testing, test10){
  graph G; 

  ASSERT_TRUE(G.getVertices().empty());

  G.addVertex("A");
  G.addVertex("B");
  G.addVertex("C");
  G.addVertex("D");
  G.addVertex("E");
  G.addVertex("F");
  G.addVertex("G");
  G.addVertex("H");

  vector<string> test{"A", "B", "C", "D", "E", "F", "G", "H"};
  vector<string> data = G.getVertices();

  ASSERT_TRUE(test.size( )== data.size());

}

//millions
 TEST(testing, test11){
  graph G; 

  int num_v = 1000000;
  int vertex_count;

  for(int i = 0; i < num_v; i++){
    int x = rand() % num_v + 1;
    int y = rand() % num_v + 1;
    string x_text = to_string(x);
    string y_text = to_string(y);
    double w = (double) rand() / RAND_MAX;


    bool m = G.addVertex(x_text);
    if(m){
      vertex_count++;}
    
    bool n =G.addVertex(y_text); 
    if(n){
      vertex_count++;
      }
  
    G.addEdge(x_text, y_text, w);
  }

  ASSERT_TRUE(G.NumVertices() == vertex_count);

 }
 
int main()
{
   ::testing::InitGoogleTest();

    //
    // run all the tests, returns 0 if they
    // all pass and 1 if any fail:
//     //
// cout << "** Graphs **" << endl; 
//
//build a graph: 
//
  // graph G;

 // G(10);
  // G.addVertex("F");
  // G.addVertex("A");
  // G.addVertex("B");
  // G.addVertex("E");
  // G.addVertex("C");
  // G.addVertex("D");

  // G.addEdge("A", "B", 1.23);
  // G.addEdge("A", "D", 4.342);
  // G.addEdge("B", "A", 34.34);
  // G.addEdge("B", "C", 5.342);
  // G.addEdge("B", "D", 12.0);
  // G.addEdge("C", "E", 74.3);
  // G.addEdge("D", "C", 753.3);
  // G.addEdge("E", "C", 34.2);


  // G.print(cout);
  // return 0; 

 // return G;

  // graph G;
  // G.addVertex(1);
  // G.addVertex(2);
  // G.addVertex(3);
  // G.addVertex(4);
  // G.addVertex(5);
  // G.addVertex(6);
  // G.addVertex(7);
  // G.addVertex(8);
  

  // G.addEdge(3, 8, 2.4);
  // G.addEdge(2, 1, 7.3);
  // G.addEdge(3, 4, 1.8);
  // G.addEdge(8, 1, 5.56);
  // G.addEdge(7, 8, 1.23);
  // G.addEdge(6, 3, 4.2);
  // G.addEdge(7, 4, 9.3);
  // G.addEdge(8, 7, 8.3);
  // G.addEdge(6, 2, 1.23);
  // G.addEdge(1, 2, 3.4);
  // G.addEdge(5, 2, 35.6);
  // G.addEdge(1, 3, 32.3);

 


    int result = RUN_ALL_TESTS();

  return result;
}

