// This portion of code done by Justin Bryant

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <vector>

/**
 * Queue class provides manual FIFO implementation.
 */
template <typename T>
class Queue 
{
    private:
        std::vector<T> data;
        int frontIndex; 

    public:
        Queue() : frontIndex(0) {}
        
        void enqueue(T item) 
        { 
            data.push_back(item); 
        }
        
        void dequeue() 
        { 
            if (!isEmpty()) frontIndex++; 
        }
        
        T front() const 
        { 
            return data[frontIndex]; 
        }
        
        bool isEmpty() const 
        { 
            return frontIndex >= (int)data.size(); 
        }
};

#endif