#include "Graph.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include "Queue.hpp"

// finds the id of the aiport
int Graph::findAirportIndex(const std::string& code) const {
    for (int i = 0; i < airports.size(); ++i) {
        if (airports[i].getCode() == code) return i;
    }
    return -1;
}
// find the shortest path
void Graph::shortestPath(const std::string& startcode, const std::string& endcode){
    int start = findAirportIndex(startcode);
    int end = findAirportIndex(endcode);
    if (start == -1 || end == -1){
        std::cout << "ERROR: AIRPORT CODES INCORRECT" << std::endl;
        return;
    }
    std::vector<int> distance(airports.size(), 5000000); // longest possible path of distance
    std::vector<int> parent(airports.size(),-1); // the actual path to find distance
    std::vector<int> cost(airports.size(),0); // cost of the path
    std::vector<int> queue1;
    distance[start] = 0;
    cost[start] = 0;
    queue1.push_back(start); // begins the check

    int head = 0;
    while (head < (int)queue1.size()){
        int u = queue1[head];
        head++;
        
        const std::vector<Airplane>& curFlight = airports[u].getFlights();

        for (int i = 0; i < (int)curFlight.size(); i++){
            Airplane flight = curFlight[i];
            int ex = flight.getDestinationIndex();
            int miles = flight.getDistance();
            int price = flight.getCost();

            // updates path to the ex IF its a shorter path
            if (distance[u] + miles < distance[ex]) {
                distance[ex] = distance[u] + miles;
                parent[ex] = u;
                cost[ex] = cost[u] + price;
                queue1.push_back(ex); // Adds to a long list to later check
            }


        }

    }
    if (distance[end] == 5000000 ){ // means nothing changed which means not path exist
        std::cout << "NO PATH EXISTS" << std::endl;
    }
    else {
        std::vector<std::string> path;
        int follow = end; // goes through the path from end to start
        while (follow != -1){
            path.push_back(airports[follow].getCode());
            follow = parent[follow];
        }
        std::cout << "shortest distance is : " << distance[end] << " miles" <<". The cost is " << cost[end] << std::endl;
        std::cout << " using route: ";
        for (int i = (int)path.size() - 1; i >= 0; i--) {
            std::cout << path[i] << (i == 0 ? "" : " -> "); // gives every airport its going thorugh to find the path
        }
        std::cout << std::endl;
    }
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

void Graph::undirectedGraph(){
    int numAir = (int)airports.size();
    // creates a matrix 2x2 hopefully
    std::vector<std::vector<int>> matrix(numAir , std::vector<int>(numAir,-1));

    for (int i = 0; i < numAir; i++){
        const std::vector<Airplane>& flight = airports[i].getFlights();
        // checks every flight leaving insert j airport
        for (int j = 0; j <(int)flight.size();j++){
            Airplane currFlight = flight[j];
            int v = currFlight.getDestinationIndex();
            int cost = currFlight.getCost();
            int row = 0;
            int col = 0;
            if (i < v){
                row = i;
                col = v;
            }
            else {
                row = v;
                col = i;
            }
            if (matrix[row][col] == -1 || cost < matrix[row][col]) {
                matrix[row][col] = cost;
            }

        }
    }
    for (int i = 0; i < numAir; i++){
        for (int j = 0; j < numAir; j++){
            if (matrix[i][j] != -1){ // checks if a connections even exists
                std::cout << "AIRPORT: "<< airports[i].getCode() << "----- " << "AIRPORT: " << airports[j].getCode() << std::endl;
                std::cout << "The Cost is: " << matrix[i][j] << std::endl;
            }
        }
    }
}


