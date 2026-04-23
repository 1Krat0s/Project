#include "Graph.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

// finds the id of the aiport
int Graph::findAirportIndex(const std::string& code) const {
    for (int i = 0; i < airports.size(); ++i) {
        if (airports[i].getCode() == code) return i;
    }
    return -1;
}

//Load CSV
bool Graph::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    
    std::getline(file, line); // have to skip the first line

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string origin, destination, originCity, originState, destCity, destState, dist, cost;
        std::getline(ss, origin, ',');
        std::getline(ss, destination, ',');
        std::getline(ss, originCity, ',');  // Reads  the origin city
        std::getline(ss, originState, ','); // Reads the origin state
        std::getline(ss, destCity, ',');  // Reads the destination city
        std::getline(ss, destState, ','); // reads the destination state
        std::getline(ss, dist, ',');
        std::getline(ss, cost, ',');

        
        std::string State = originState.substr(1, 2); 

        // 1. Add origin if new
        if (findAirportIndex(origin) == -1) 
            airports.push_back(Airport(origin, State));
        
        // 2. Add destination if new
        if (findAirportIndex(destination) == -1) 
            airports.push_back(Airport(destination, destState.substr(1, 2)));

        // connects all the edges between the origin and destination
        int u = findAirportIndex(origin);
        int v = findAirportIndex(destination);
        airports[u].addFlight(v, std::stoi(dist), std::stod(cost));
    }
    return true;
}

// Count Connections
void Graph::displayConnectionCounts() const {
    std::vector<int> counts(airports.size(), 0);
    for (int i = 0; i < airports.size(); i++) {
        // returns the outbound since all it is just the edges size
        counts[i] += airports[i].getFlights().size();

        //checks through all to make sure inbound exists
        for (auto& edge : airports[i].getFlights()) {
            counts[edge.getDestinationIndex()]++;
        }
    }

    // prints all the codes
    for (int i = 0; i < airports.size(); i++) {
        std::cout << airports[i].getCode() << ": " << counts[i] << " connections\n";
    }
}
// just prints the number of airports shouldn't repeate
int Graph::getNumAirports() const { 
    return airports.size(); 
}