// This file is full of tests of various parts of the code
// It won't be used in the final cuts

#include <cstdlib>
#include <cstdio>
#include "dstructs/list.hpp"
#include "dstructs/hashtable.hpp"
#include "dstructs/pqueue.hpp"

void test_list(){

    // Create an empty list
    List * mylist = new List();

    // Fill the list with random elements
    for(int i = 0; i < 20; i++)
        mylist->add((void *) new int(rand() % 50));

    // Print the elements in order
    for(int i = 0; i < 20; i++){
        int el = *((int*)mylist->get(i));
        printf("%d ",el);
    }
    printf("\n");

    // Print the number of elements
    printf("The list contains %d elements.\n",mylist->getElems());

    // Delete the elements and perform a reset

    for(int i = 0; i < 20; i++){
        delete (int *) mylist->get(i);
    }
    printf("Reseting\n");
    mylist->reset();

    // Print the number of elements after reset
    printf("Elements after reset: %d\n",mylist->getElems());

    // Finally delete the list
    delete mylist;

}

void print_hashtable(HashTable * mytable){

    // Print the chain contents
    for(int i = 0; i < mytable->getSize(); i++){

        // Get the specific chain
        List * chain = mytable->getChain(i);

        // Print out its contents
        printf("\n[%d] -> ",i);
        for(int j = 0; j < chain->getElems(); j++){
            HashElement * he = (HashElement *) chain->get(j);
            printf("%d:%d ",he->key,*((int*)he->data));
        }
        
    }
    printf("\n\n");

}

void test_hashtable(){

    // First create an empty hashtable
    HashTable * mytable = new HashTable(7);

    // Create some new objects and keys to add to the hash table
    int * objects[30];
    int keys[30];
    printf("Adding objects to the table\n");
    for(int i = 0; i < 30; i++){
        objects[i] = new int(rand()%200);
        keys[i] = rand();

        // Add only the first 10 objects
        if( i < 20)
            mytable->add((void*) objects[i],keys[i]);
    }

    // Print the amount of elements
    printf("The table contains %d elements\n",mytable->getElems());

    // Check that only the first 10 objects exist in the table
    int obj_exist = 0;
    for(int i = 0; i < 30; i++){
        if ( mytable->exists(objects[i],keys[i]))
            obj_exist++;
    }
    printf("Exists gave %d/20 result, %s\n",obj_exist,(obj_exist == 20)?"Correct":"Wrong");

    // Print the whole hashtable
    print_hashtable(mytable);

    // Reset the hashtable and print it again
    printf("Reseting the table\n");
    mytable->reset();
    printf("Table size after reset: %d\n",mytable->getElems());
    print_hashtable(mytable);

    // Delete the elements and the hashtable
    for(int i = 0; i < 30; i++){
        delete objects[i];
    }
    delete mytable;
    
}

void test_priority_queue(){

    // Create an empty priority queue
    PriorityQueue * myqueue = new PriorityQueue(5);

    // Create some elements to add to the queue
    int * objects[30];
    double priorities[30];
    for(int i = 0; i < 30; i++){
        
        objects[i] = new int(rand() % 300);
        priorities[i] = 20.0*rand()/RAND_MAX;

    }

    // Add the objects in the queue
    printf("Adding objects in the queue\n");
    for(int i = 0; i < 30; i++){
        printf("Adding %d\n",i);
        fflush(stdout);
        myqueue->add((void*) objects[i],priorities[i]);
    }

    
    // Display the whole array and the elements and the size
    printf("The queue elements are %d\n",myqueue->getElems());
    QueueElement ** els = myqueue->getArray();
    for(int i = 0; i < 30; i++){
        QueueElement * el = els[i];
        printf("%d:%.2lf ", *((int*)el->data),el->priority);
    }
    printf("\n\n");

    // Try to reinsert old elements and check the size
    printf("Trying to reinsert past elements\n");
    for(int i = 0; i < 5; i++){
        myqueue->add((void*) objects[i],priorities[i]);
    }
    printf("The queue elements are %d, should be same as before\n\n",myqueue->getElems());

    // Remove the elements one by one, should come out sorted
    printf("Removing elements in a sorted manner\n");
    while(myqueue->getElems() > 0){
        double pri = myqueue->peekPriority();
        int a = *((int*)myqueue->remove());
        printf("%.2lf ", pri, a);
    }
    printf("\n");

    // Delete the elements and the queue
    for(int i = 0; i < 30; i++){
        delete objects[i];
    }
    delete myqueue;

}