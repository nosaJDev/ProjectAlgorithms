#include "pqueue.hpp"
#include <cstdio>

PriorityQueue::PriorityQueue(int _size){

    // Initiate the size and the array of elements
    size = _size;
    elems = 0;
    
    // Size + 2 because it's easier for adding elements
    array = new QueueElement*[size];
    for(int i = 0; i < size; i++){
        array[i] = nullptr;
    }

    // Initiate the hash table for double checking
    double_check = new HashTable(size);


}

PriorityQueue::~PriorityQueue(){

    // Delete all QueueElement structs
    for(int i = 1; i <= elems; i++){
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
    for(int i = 0; i < elems+1; i++){
        newarray[i] = array[i];
    }
    
    // Set the rest elements to null
    for(int i = elems+1; i < newsize; i++){
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
    if( double_check->exists(elem, (long long) elem)){
        // If it exists, then don't add it again
        return;
    }

    // If it does not exist, mark it as added on the double checker
    double_check->add(elem, (long long) elem);

    // Check if you need to double the space to fit
    if (elems+1 >= size)
        doubleArray();

    // Add the element to the last spot
    QueueElement * my_elem = new QueueElement(elem,priority);

    // Then perform swapping to get it to the correct spot
    int spot_at = elems+1;
    array[spot_at] = my_elem;

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

    // Increment the number of elements
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

double PriorityQueue::peekPriority(){

    // It returns the priority of the first element
    // retruns zero if no first element
    
    if(elems == 0)
        return 0;
    
    return array[1]->priority;
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

    // Leave early if only one element
    if(elems == 1){
        elems = 0;
        array[1] = nullptr;
        return ret_elem;
    }

    // Replace with the last element and perform descend
    array[1] = array[elems];
    array[elems] = nullptr;
    int spot_at = 1;
    while(true){

        int down_spot = -1;
        if (2*spot_at >= size || array[2*spot_at] == nullptr){
            // No children
            // Nowhere to go from here
            break;
        }else if (2*spot_at+1 > size || array[2*spot_at+1] == nullptr){
            // Only left child
            // Check if you need to decend left
            if (array[2*spot_at]->priority < array[spot_at]->priority)
                down_spot = 2*spot_at;
            else break;
        }else{
            // Both children are present
            // Descend to the largest
            double pri;
            if (array[2*spot_at]->priority < array[2*spot_at+1]->priority){
                down_spot = 2*spot_at;
                pri = array[2*spot_at]->priority;
            }else{
                down_spot = 2*spot_at+1;
                pri = array[2*spot_at+1]->priority;
            }
            
            // Check if you are good
            if(array[spot_at]->priority < pri)
                break;
        }

        // Perform the swap
        QueueElement * temp = array[spot_at];
        array[spot_at] = array[down_spot];
        array[down_spot] = temp;
        spot_at = down_spot;
    }

    // Decrement the element counter
    elems--;

    // Finally return the removed element
    return ret_elem;

}

QueueElement ** PriorityQueue::getArray(){

    // Returns the array with all the QueueElements
    // Read only, do not tamper on the outside
    return array+1;

}
