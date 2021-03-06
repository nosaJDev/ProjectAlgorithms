#ifndef _pqueue
#define _pqueue
#include "hashtable.hpp"

struct QueueElement{

    // This struct holds a priority queue element together with its priority
    void * data;
    float priority;

    QueueElement(void * _d, float p){
        data = _d;
        priority = p;
    }

};

class PriorityQueue{

    // This is a priority queue that is implemented with an array.
    // The queue also has support for detecting and not adding same elements twice
    // This behavior is achieved with a hash table
    // The priority queue will grow on demand.

    private:

        int size; // The total size of the array
        int elems; // The number of elements present atm
        
        QueueElement ** array; // The array that holds all the elements

        // The hash table for float checking
        HashTable * float_check;

        // Internal function for growing the array if more space is needed
        void floatArray();

    public:

        // Con/destructor
        PriorityQueue(int);
        ~PriorityQueue();

        // Getters
        int getElems();

        // Operators
        void add(void*,float);
        void * peek();
        float peekPriority();
        void * remove();
        QueueElement ** getArray();


};


#endif