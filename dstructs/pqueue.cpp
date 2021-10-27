#include "pqueue.hpp"


PriorityQueue::PriorityQueue(int _size){

    // Initiate the size and the array of elements
    size = _size;
    elems = 0;
    
    // Size + 2 because it's easier for adding elements
    array = new QueueElement*[size+2];

    // Initiate the hash table for double checking
    double_check = new HashTable(size);


}

PriorityQueue::~PriorityQueue(){

    // Delete all QueueElement structs
    for(int i = 0; i < elems; i++){
        delete array[i];
    }
    delete[] array;

    // Delete the hash table for double checking
    delete double_check;

}

void PriorityQueue::doubleArray(){
    // This will double the array space
    // it is needed for when there is no more space but we are adding elements

    // Get the new size and the new array
    int newsize = 2*size;
    QueueElement ** newarray = new QueueElement*[newsize];

    // Copy the elements to the new array
    for(int i = 0; i < elems; i++){
        newarray[i] = array[i];
    }
    
    // Set the rest elements to null
    for(int i = elems; i < newsize; i++){
        newarray[i] = nullptr;
    }

    // Delete the old array and reset the size
    delete[] array;
    array =  newarray;
    size = newsize;


}

int PriorityQueue::getElems(){

    // Returns the number of elements currently in the queue
    return elems;

}

void PriorityQueue::add(void * elem, double priority){

    // This will add an element to the priority queue
    // Elements that do not fit will be deleted.

    // First, check if the element is already present on the priority queue
    if( double_check->exists(elem, (int) elem)){
        // If it exists, then don't add it again
        return;
    }

    // If it does not exist, mark it as added on the double checker
    double_check->add(elem, (int) elem);

    // Check if you need to double the space to fit
    if (elems == size)
        doubleArray();

    // Add the element to the last spot
    array[elems+1] = new QueueElement(elem,priority);

    // Then perform swapping to get it to the correct spot
    int spot_at = elems+1;
    QueueElement * my_elem = array[elems+1];
    while(spot_at > 1){

        // Get the contesting element
        QueueElement * other = array[spot_at/2];

        // Check if you need to swap
        if(other->priority > my_elem->priority){
            // Perform the swap
            array[spot_at/2] = my_elem;
            array[spot_at] = other;
            spot_at = spot_at/2;
        }else{
            // If you don't need to perform a swap, you are done
            break;
        }
        
    }

    // Check if you pushed an element out
    if (elems == size){
        // If you have, delete the QueueElement at the tail
        delete array[size];
        array[size] = nullptr;
    }

    // Increment the number of elements
    if (elems < size)
        elems++;

}

void * PriorityQueue::peek(){
    
    // This will return the first element of the priority queue
    // It won't remove the element

    // Check if the queue is empty
    if (elems == 0)
        return nullptr;

    // If it's not empty return the first one
    return array[1]->data;

}

void * PriorityQueue::remove(){

    // This will remove and return the first priority queue element

    // Check if the queue is empty
    if (elems == 0)
        return nullptr;

    // If it's not, get the element you will be returning
    void * ret_elem = array[1]->data;

    // Delete the corresponding QueueElement struct
    delete array[1];

    // Replace with the last element and perform descend
    array[1] = array[elems];
    array[elems] = nullptr;
    int spot_at = 1;
    while(true){

        int down_spot = -1;
        if (2*spot_at < size && array[2*spot_at] == nullptr){
            down_spot = 2*spot_at;
        }else if (2*spot_at+1 < size && array[2*spot_at+1] == nullptr){
            down_spot = 2*spot_at+1;
        }else{

            // Special case for end of queue
            if (2*spot_at < size){
                // You are at the bottom, nowhere to go
                break;
            }if (2*spot_at+1){
                down_spot = 2*spot_at;
            }else{
                // Descend to the largest
                if (array[2*spot_at]->priority < array[2*spot_at+1]->priority){
                    down_spot = 2*spot_at;
                }else{
                    down_spot = 2*spot_at+1;
                }
            }
        }

        // Perform the swap
        QueueElement * temp = array[spot_at];
        array[spot_at] = array[down_spot];
        QueueElement * other = array[down_spot];
        spot_at = down_spot;
    }

    // Decrement the element counter
    elems--;

}

QueueElement ** PriorityQueue::getArray(){

    // Returns the array with all the QueueElements
    // Read only, do not tamper on the outside
    return array;

}
