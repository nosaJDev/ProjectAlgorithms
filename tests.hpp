// This file is full of tests of various parts of the code
// It won't be used in the final cuts

#include <cstdlib>
#include <cstdio>
#include "dstructs/list.hpp"
#include "dstructs/hashtable.hpp"
#include "dstructs/pqueue.hpp"
#include "fileparse/vectorfile.hpp"
#include "dstructs/lsh.hpp"
#include "dstructs/hcubemap.hpp"
#include "geometry/metric.hpp"

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

void test_vector_reading(){

    // Create a vector file parser with the approriate filename
    VectorFile * myvectors = new VectorFile("./datasets/input_small_id",0);

    // Check and read out the vectors
    List * vlist = myvectors->getVectorList();
    for(int i = 0; i < vlist->getElems(); i++){
        Vector * vec = (Vector *) vlist->get(i);
        //if(i > 9920) vec->print();
    }

    // Print the number of vectors you have read
    printf("I have successfully read %d vectors\n",vlist->getElems());

    // Finally destroy the vectorfile object
    delete myvectors;

}

void test_hamming_calculation(){

    // Test the hamming result of various numbers with 4 bits and distances from 0 to 2

    for(int i = 0; i < 40; i++){
        
        // Check the number you will find
        int number = rand() % 16;
        int dist = rand() % 3;

        // Run the hamming to obtain the array
        int * res = inclusiveHamming(number,4,dist,nullptr);

        // Print the result
        printf("Result for %d with distance %d:[",number,dist);
        for(int i = 1; i <= res[0]; i++){
            printf(" %d%s",res[i],(i != res[0])?",":"");
        }
        printf("]\n");

        // Delete the array after you viewed it
        delete[] res;

    }

}

void test_hash_methods(int mode){

    // First read the input vectors and the query vectors
    printf("Reading the files\n");
    VectorFile * inputvecs = new VectorFile("./datasets/input_small_id",0);
    VectorFile * queryvecs = new VectorFile("./datasets/query_small_id",0);

    // Then create the lsh structure
    printf("Creating the data structure\n");
    PointStruct * localityhash;
    if(mode == 0){
        // int _L, int _k,int _w, int buckets, int dim
        localityhash = new LSH(20,6,4,27,128);
    }else{
        // int cdims, int _w, int dims
        localityhash = new HypercubeMapping(3,4,128);
    }

    //Insert the vectors into the locality hash
    printf("Inserting the input vectors into the data structure\n");
    localityhash->addVectorList(inputvecs->getVectorList());

    // Intialize the L2 Metric
    Metric * metric = new L2Metric();

    // Test the locality hashing first
    List * inputlist = inputvecs->getVectorList();
    LocalityHashFamily * lhf = new LocalityHashFamily(200,128,4);

    for(int i = 0; i < inputlist->getElems()/10; i++){

        // Get the vector
        Vector * vec = (Vector*)inputlist->get(i);

        // Find three values from three hashfunctions
        int vals[3];
        vals[0] = lhf->hash(5,vec);
        vals[1] = lhf->hash(25,vec);
        vals[2] = lhf->hash(23,vec);

        // Print them out
        //printf("For vector %s, values %d %d %d\n",vec->getLabel(),vals[0],vals[1],vals[2]);


    }

    delete lhf;


    // Then extract the query list and perform queries
    List * qlist = queryvecs->getVectorList();
    for(int i = 0; i < qlist->getElems(); i++){

        // Get the current vector
        Vector * vec = (Vector*) qlist->get(i);

        // Find the closest neighbor
        Vector * nn = localityhash->approximateNN(vec,metric);

        // Print it out
        double ld;
        if(nn != nullptr){
            ld = metric->dist(vec,nn);
            printf("Found closest neighbor of distance %lf\n",ld);
        }

        // Calcuate real closest dist
        double rd = 9999;
        double t;
        int missed = 0;
        for(int j = 0; j < inputlist->getElems(); j++){
            Vector * oth = (Vector *) inputlist->get(j);
            t = metric->dist(oth, vec);
            if(t < rd){
                rd = t;
            }
            if(t < ld)
                missed++;
        }
        printf("Real least distance: %lf, missed %d close vectors\n",rd,missed);

    }

    // Then perform the kNN search for some
    for(int i = 0; i < qlist->getElems()/10; i++){

        // Get the vector of the search
        Vector * vec = (Vector*) qlist->get(i);

        // Perform the search and get the priority queue
        PriorityQueue * result = localityhash->approximatekNN(7,vec, metric);

        // Display some of the results
        printf("Closest neighbors for %s:\n",vec->getLabel());
        for(int j = 0; j < 5; j++){

            // Acquire the next vector
            Vector * nn = (Vector*) result->remove();
            if(nn == nullptr) break;

            // Print the distances
            printf("Found vector with distance %lf\n",metric->dist(nn,vec));

        }

        // Then delete the PriorityQueue
        delete result;

    }

    fflush(stdout);

    // Finally, perform a range search for some
    
    int a = qlist->getElems()/10;
    for(int i = a; i < 2*a; i++){

        // Get the vector of the search
        Vector * vec = (Vector*) qlist->get(i);

        // Perform the search and get the priority queue
        double radius = 430.0;
        PriorityQueue * result = localityhash->approximateRange(radius,vec,metric);

        // Display some of the results
        
        printf("Range search with r = %lf for %s:\n",radius,vec->getLabel());
        
        while(result->getElems()>0){

            // Acquire the next vector
            Vector * nn = (Vector*) result->remove();

            // Print the distances
            printf("Found vector with distance %lf\n",metric->dist(nn,vec));

        }

        // Then delete the PriorityQueue
        delete result;
    }
    

    // Delete any redundant objects
    delete localityhash;
    delete inputvecs;
    delete queryvecs;


}

