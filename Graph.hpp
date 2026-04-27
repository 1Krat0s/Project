// This portion of code done by Justin Bryant


#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>
#include "Airport.hpp"
#include "Queue.hpp"

/*
* Graph class manages the connectivity map.
*/
class Graph 
{
    private:
        std::vector<Airport> airports;
        int findAirportIndex(const std::string& code) const;
        Airplane getPlane(int airport1, int airport2);
        std::vector<std::vector<int>> AllPathsStops(int start, const int dest, int stops) const;

    public:
        

        // Task 1: Construct weighted directed graph from CSV 
        bool loadFromCSV(const std::string& filename);

        // task 2 to find shortest distance
        void shortestPath(const std::string& startcode, const std::string& endcode);
        // task 6 creates a undirected graph
        void undirectedGraph();
        
        // Task 5: Calculate and sort connections 
        void displayConnectionCounts() const;

        int getNumAirports() const;

        //task 3 - all airports in state (TC)
        void allState(const std::string& origin, const std::string& stateAbb);

        //task 4 - shortest path with stops
        void shortestPathStops(const std::string& origin, const std::string& dest, int stops);

        // Task 7: Generate MST using Prim's algorithm
        void primMST() const;

        // Task 8: Generate MSF using Kruskal's algorithm
        void kruskalMSF() const;
};

#endif