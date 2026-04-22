// This portion of code done by Justin Bryant

#include "Airplane.hpp"

/*
* Constructor for the Airplane class.
* Initializes a directed edge with a destination index and two distinct weights.
* This structure allows a single edge to be used for multiple optimization criteria.
*/
Airplane::Airplane(int dest, int dist, int c) : destinationIndex(dest), distance(dist), cost(c) {}

/*
* Returns the index of the destination airport.
* Using an integer index instead of a string code during traversal improves performance by allowing O(1) access to the adjacency list in the Graph class.
*/
int Airplane::getDestinationIndex() const 
{ 
    return destinationIndex; 
}

/*
* Returns the physical distance of the flight.
* This weight is  required for solving shortest-path routing problems.
*/
int Airplane::getDistance() const 
{ 
    return distance; 
}

/*
* Returns the financial cost of the flight.
* This weight is required for MST/MSF generation and total path cost evaluation.
*/
int Airplane::getCost() const 
{ 
    return cost; 
}