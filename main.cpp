#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "tests.hpp"

int main(){

    // Seed the random generator
    srand(time(nullptr));

    // test_list();  Passed!
    // test_hashtable(); Passed thank god
    // test_priority_queue(); Passed, don't even doubt it

    // test_vector_reading(); Successful
    // test_hash_methods(0); (LSH) Works, needs valgrind testing
    while(true)
        test_hash_methods(1,true,false,false); // (Hypercube) Pending

    // test_hamming_calculation(); Passed, pending valgring check


    return 0;

}