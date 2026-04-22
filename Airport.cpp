// This portion of code done by Justin Bryant

#include "Airport.hpp"

/*
* Constructor for the Airport class.
* Every node must store its unique airport code (e.g., PIT) and its corresponding city/state information to fulfill search requirements by state.
*/
Airport::Airport(std::string c, std::string ct) : code(c), city(ct) {}

/*
* Adds a new Airplane (edge) to the airport's local adjacency list.
* Using std::vector to store edges is the only permitted STL usage and provides a flexible way to manage variable numbers of flight connections.
*/
void Airport::addFlight(int destIdx, int dist, int cost) 
{
    flights.push_back(Airplane(destIdx, dist, cost));
}

/*
* Returns the 3-letter airport code.
* Essential for output formatting and identifying the origin/destination in shortest path results.
*/
std::string Airport::getCode() const 
{ 
    return code; 
}

/*
* Returns the city and state associated with the airport.
* Necessary for Task 3, which requires finding paths based on a specific destination state.
*/
std::string Airport::getCity() const 
{ 
    return city; 
}

/*
* Provides access to all outbound flight connections (Edges).
* This is the primary method used by graph traversal algorithms to explore neighboring nodes.
*/
const std::vector<Airplane>& Airport::getFlights() const 
{ 
    return flights; 
}