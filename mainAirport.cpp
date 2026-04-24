#include "Airport.hpp"
#include "Airplane.hpp"
#include "Graph.hpp"
#include "Queue.hpp"
#include <iostream>


int main(void){
    Graph simGraph;

    if (simGraph.loadFromCSV("airports.csv")){ //task 1
        std::cout << "SUCCESS" << std::endl;
    }
    else{
        std::cout << "NOT FOUND" << std::endl;
    }

    simGraph.displayConnectionCounts();
    simGraph.shortestPath("IAD","MIA");
    simGraph.undirectedGraph(); // task 6

    simGraph.displayConnectionCounts(); //task 5
    simGraph.shortestPath("IAD","MIA"); //task 2
    simGraph.allState("ATL", "FL"); //task 3
    simGraph.shortestPathStops("IAD", "MIA", 3); //task 4



}