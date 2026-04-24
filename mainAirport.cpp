#include "Airport.hpp"
#include "Airplane.hpp"
#include "Graph.hpp"
#include "Queue.hpp"
#include <iostream>


int main(void){
    Graph simGraph;

    if (simGraph.loadFromCSV("airports.csv")){
        std::cout << "SUCCESS" << std::endl;
    }
    else{
        std::cout << "NOT FOUND" << std::endl;
    }
    simGraph.displayConnectionCounts();
    simGraph.shortestPath("IAD","MIA");
    simGraph.undirectedGraph();


}