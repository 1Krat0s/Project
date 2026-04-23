// This portion of code done by Justin Bryant


#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>
#include "Airport.hpp"

/*
* Graph class manages the connectivity map.
*/
class Graph 
{
    private:
        std::vector<Airport> airports;
        int findAirportIndex(const std::string& code) const;

    public:
        // Task 1: Construct weighted directed graph from CSV 
        bool loadFromCSV(const std::string& filename);

        // task 2 to find shortest distance
        void shortestPath(const std::string& startcode, const std::string& endcode);
        
        // Task 5: Calculate and sort connections 
        void displayConnectionCounts() const;

        int getNumAirports() const;
};

#endif