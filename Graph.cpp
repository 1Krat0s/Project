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

//origin airport to all airports in state (TC)
void Graph::allState(const std::string& origin, const std::string& stateAbb){
    std::vector<Airport> dests; //vector to store all the airports in the given state
    for (int i = 0; i < airports.size(); i++) {
        if(airports[i].getCity() == stateAbb){ //checks if the state stored in the current airport matches the given state
            dests.push_back(airports[i]); //adds airports to dests
        }
    }
    std::cout << "Shortest paths from " << origin <<  " to " <<  stateAbb << " state airports are:\n";
    for (int i = 0; i < dests.size(); i++){ //iterates through all airports in state
        std::cout << "From " << origin << " to " << dests[i].getCode() << std::endl;
        shortestPath(origin, dests[i].getCode()); //sends current airport and origin airport to the shortest path function
        std::cout << std::endl;
    }
}

//Returns the shortest path with the given number of stops (TC)
void Graph::shortestPathStops(const std::string& origin, const std::string& dest, int stops){
    //shortest path with specified stops based on distance

    //finds the start and end indices
    int start = findAirportIndex(origin); 
    int end = findAirportIndex(dest);

    std::vector<std::vector<int>> v = AllPathsStops(start, end, stops); //get paths in index form

    //checks that at least one path was returned
    if(v.empty()){
        //output for no path found
        std::cout << "Shortest route from " << origin << " to " << dest << " with " << stops << " stops: None.\n";
        return;
    }

    //intialization for distance, cost, and final index
    std::vector<int> dist(v.size(), 0);
    std::vector<int> cost (v.size(), 0);
    int distance = 5000000;
    int endcost = 0;
    int idx = 0;

    //Iterates through the returned routes
    for(int i = 0; i < v.size(); i++){
        for(int j = 0; j < v[i].size(); j++){ //index of stops in path
            Airplane plane = getPlane(v[i][j], v[i][j+1]);
            //stores the distance and cost of the current route
            dist[i] = dist[i] + plane.getDistance();
            cost[i] = cost[i] + plane.getCost();
        }

        //compares the distance and saves the smallest
        if(dist[i] < distance){
            distance = dist[i];
        }
        //stores the cost and index for the smallest distance
        if(dist[i] == distance){
            endcost = cost[i];
            idx = i;
        }
    }
    //outputs the final path
    std::cout << "Shortest route from " << origin << " to " << dest << " with " << stops << " stops: ";
    for(int k = 0; k < v[idx].size(); k++){
        if(k == v[idx].size() -1 ){
            std::cout << airports[v[idx][k]].getCode();
        }
        else{
            std::cout << airports[v[idx][k]].getCode() << "->";
        }
    }
    
    //outputs the final distance and cost
    std::cout << ". The length is: " << distance;
    std::cout << ". The cost is: " << endcost << ".\n";
}

//takes two airport indexes and returns the plane between them (TC)
Airplane Graph::getPlane(int airport1, int airport2){ 
    std::vector<Airplane> v = airports[airport1].getFlights();
    Airplane b (0,0,0);
    for(Airplane a : v){
        if(a.getDestinationIndex() == airport2){
            return a;
        }
    }
    return b; //returns an empty plane if the second airport is not found
}

//Finds all possible routes between two points with a given number of stops (TC)
std::vector<std::vector<int>> Graph::AllPathsStops(int start, const int dest, int stops) const{
    std::vector<std::vector<int>> allRoutes; //vector for storing all routes
    Queue<std::vector<int>> q; //queue for temporary tracking
    q.enqueue({start}); //the starting airport loaded into the queue

    int lim = 0;
    while(!q.isEmpty() ){
        lim++;
       std::vector<int> route = q.front(); //begins the route with the front of the queue
       q.dequeue(); //removes the front from the queue
       int cur = route.back(); //sets the current value
       
       //checks if the current value is the destination and if it has the correct amount of stops
       if(cur == dest && route.size() == stops + 2){ 
        allRoutes.push_back(route);
       }

       //iterates through the neighbors (flights) of the current airport 
       for(Airplane a : airports[cur].getFlights()){
       std::vector<int> newRoute = route; //creates a new variable with the route

        //checks that the flight is not the start and makes sure that stops have not exceeded the limit
        if(a.getDestinationIndex() != start && newRoute.size() <= (stops+2)){
            newRoute.push_back(a.getDestinationIndex()); //adds the stop to the new route
            q.enqueue(newRoute); //adds the newroute to the queue
        }
    }
}
    return allRoutes; //returns all the possible routes with the given number of stops

}


// task 6

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

// Task 7: Prim's Algorithm (MST)
void Graph::primMST() const 
{
    int V = airports.size();
    if (V == 0) return;

    const int INF = 1000000000;
    // Create an undirected adjacency matrix based on minimum costs
    std::vector<std::vector<int>> matrix(V, std::vector<int>(V, INF));
    
    for (int i = 0; i < V; i++) 
    {
        const std::vector<Airplane>& flights = airports[i].getFlights();

        for (int j = 0; j < (int)flights.size(); j++) 
        {
            int v = flights[j].getDestinationIndex();
            int cost = flights[j].getCost();

            // Keep the minimum cost for the undirected edge
            if (cost < matrix[i][v]) 
            {
                matrix[i][v] = cost;
                matrix[v][i] = cost; 
            }
        }
    }

    std::vector<int> minCost(V, INF);
    std::vector<int> parent(V, -1);
    std::vector<bool> inMST(V, false);

    minCost[0] = 0; // Start from the first airport
    int totalCost = 0;

    for (int count = 0; count < V; count++) 
    {
        int u = -1;
        int minVal = INF;

        // Manually find the vertex with the minimum cost
        for (int i = 0; i < V; i++) 
        {
            if (!inMST[i] && minCost[i] < minVal) 
            {
                minVal = minCost[i];
                u = i;
            }
        }

        // If we can't find a reachable vertex, the graph is disconnected
        if (u == -1) 
        {
            std::cout << "Graph is disconnected. An MST cannot be formed.\n";
            return;
        }

        inMST[u] = true;
        if (parent[u] != -1) 
        {
            totalCost += matrix[parent[u]][u];
        }

        // Update the adjacent vertices
        for (int v = 0; v < V; v++) 
        {
            if (matrix[u][v] != INF && !inMST[v] && matrix[u][v] < minCost[v]) 
            {
                minCost[v] = matrix[u][v];
                parent[v] = u;
            }
        }
    }

    // Output formatting matching the required sample
    std::cout << "Minimum Spanning Tree:\n";
    std::cout << "Edge\tWeight\n";

    for (int i = 1; i < V; i++) 
    {
        if (parent[i] != -1) 
        {
            std::cout << airports[parent[i]].getCode() << " - " << airports[i].getCode() << "\t" << matrix[parent[i]][i] << "\n";
        }
    }

    std::cout << "Total Cost of MST: " << totalCost << "\n";
}

// Task 8: Kruskal's Algorithm (MSF)

// Manual struct to hold edge data for sorting
struct Edge 
{
    int u, v, cost;
};

// Manual Disjoint Set implementation
class DisjointSet 
{
    private:
        std::vector<int> parent;

    public:
        DisjointSet(int n) 
        {
            parent.assign(n, 0);

            for(int i = 0; i < n; i++) 
            {
                parent[i] = i;
            }
        }
        
        int find(int i) 
        {
            if (parent[i] == i)
            {
                return i;
            }

            return parent[i] = find(parent[i]); // Path compression
        }
        
        void unite(int i, int j) 
        {
            int root_i = find(i);
            int root_j = find(j);

            if (root_i != root_j) 
            {
                parent[root_i] = root_j;
            }
        }
};

void Graph::kruskalMSF() const 
{
    int V = airports.size();
    if (V == 0) return;

    const int INF = 1000000000;
    std::vector<std::vector<int>> matrix(V, std::vector<int>(V, INF));
    
    // Create the undirected matrix representations
    for (int i = 0; i < V; i++) 
    {
        const std::vector<Airplane>& flights = airports[i].getFlights();

        for (int j = 0; j < (int)flights.size(); j++) 
        {
            int v = flights[j].getDestinationIndex();
            int cost = flights[j].getCost();

            if (cost < matrix[i][v]) 
            {
                matrix[i][v] = cost;
                matrix[v][i] = cost;
            }
        }
    }

    // Extract all unique undirected edges
    std::vector<Edge> edges;
    for (int i = 0; i < V; i++) 
    {
        for (int j = i + 1; j < V; j++) 
        {
            if (matrix[i][j] != INF) 
            {
                Edge e;
                e.u = i;
                e.v = j;
                e.cost = matrix[i][j];
                edges.push_back(e);
            }
        }
    }

    // Manually sort edges by cost using Selection Sort 
    for (int i = 0; i < (int)edges.size() - 1; i++) 
    {
        int min_idx = i;
        for (int j = i + 1; j < (int)edges.size(); j++) 
        {
            if (edges[j].cost < edges[min_idx].cost) 
            {
                min_idx = j;
            }
        }
        Edge temp = edges[i];
        edges[i] = edges[min_idx];
        edges[min_idx] = temp;
    }

    DisjointSet ds(V);
    int totalCost = 0;
    
    std::cout << "Minimum Spanning Forest:\n";
    std::cout << "Edge\tWeight\n";

    // Build the MSF
    for (int i = 0; i < (int)edges.size(); i++) 
    {
        int u = edges[i].u;
        int v = edges[i].v;
        int cost = edges[i].cost;

        // If including this edge doesn't cause a cycle
        if (ds.find(u) != ds.find(v)) 
        {
            std::cout << airports[u].getCode() << " - " 
                      << airports[v].getCode() << "\t" 
                      << cost << "\n";
            totalCost += cost;
            ds.unite(u, v);
        }
    }
    std::cout << "Total Cost of MSF: " << totalCost << "\n";
}