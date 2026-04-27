#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>

template <typename T>
class Queue {
private:
    // Node structure for the linked list
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* head; // Points to the front of the queue
    Node* tail; // Points to the back of the queue
    int count;  // Tracks the number of elements

public:
    // Constructor
    Queue() : head(nullptr), tail(nullptr), count(0) {}

    // Destructor to prevent memory leaks
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Adds an element to the back of the queue
    void enqueue(const T& item) {
        Node* newNode = new Node(item);
        if (isEmpty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }

    // Removes the element at the front of the queue
    void dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Cannot dequeue from an empty queue.");
        }
        Node* temp = head;
        head = head->next;
        
        // If the queue becomes empty after dequeuing, reset tail
        if (head == nullptr) {
            tail = nullptr;
        }
        
        delete temp;
        count--;
    }

    // Returns a reference to the front element
    T& front() {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty.");
        }
        return head->data;
    }

    // Returns a const reference to the front element (for const objects)
    const T& front() const {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty.");
        }
        return head->data;
    }

    // Checks if the queue is empty
    bool isEmpty() const {
        return head == nullptr;
    }

    // Returns the current size of the queue
    int size() const {
        return count;
    }
};

#endif // QUEUE_HPP