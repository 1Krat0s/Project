// This portion of code done by Justin Bryant

#ifndef AIRPLANE_HPP
#define AIRPLANE_HPP

/**
 * Airplane class acts as a weighted directed edge.
 * It tracks the destination, distance, and cost.
 */

class Airplane 
{
    private:
        int destinationIndex; // Integer ID of the destination airport to simplify graph traversal
        int distance;         // Used for shortest distance pathing 
        int cost;             // Used for cost optimization

    public:
        Airplane(int dest, int dist, int c);
        int getDestinationIndex() const;
        int getDistance() const;
        int getCost() const;
};

#endif