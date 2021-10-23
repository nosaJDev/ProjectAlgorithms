#ifndef _pqueue
#define _pqueue

struct QueueElement{

    // This struct holds a priority queue element together with its priority
    void * data;
    double priority;

    QueueElement(void * _d, double p){
        data = _d;
        priority = p;
    }

};

class PriorityQueue{

    // This is a priority queue that is implemented with an array.
    
    private:

        int size; // The total size of the array
        int elems; // The number of elements present atm
        
        QueueElement ** array; // The array that holds all the elements

    public:

        // Con/destructor
        PriorityQueue(int);
        ~PriorityQueue();

        // Operators
        void add(void*,double);
        void * peek();
        void * remove();
        QueueElement ** getArray();


};


#endif