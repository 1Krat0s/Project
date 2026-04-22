// This portion of code done by Justin Bryant

#ifndef AIRPORT_HPP
#define AIRPORT_HPP

#include <string>
#include <vector>
#include "Airplane.hpp"

/**
 * Airport class represents a node in the graph.
 * It maintains its own identity data and a list of 'Airplane' objects.
 */

class Airport 
{
    private:
        std::string code; // Abbreviated format 
        std::string city; // Full name of city and state 
        std::vector<Airplane> flights; // Adjacency list representation for  edge access 

    public:
        Airport(std::string c, std::string ct);
        void addFlight(int destIdx, int dist, int cost);
        
        // Getters used for pathfinding
        std::string getCode() const;
        std::string getCity() const;
        const std::vector<Airplane>& getFlights() const;
};

#endif