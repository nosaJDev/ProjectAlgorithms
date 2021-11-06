#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "tests.hpp"

int main(){

    // Seed the random generator
    srand(time(nullptr));

    // test_list();  Passed!
    // test_hashtable(); Passed thank god
    // test_priority_queue(); Passed with flying colors

    // test_vector_reading(); Successful
    // test_hash_methods(0); (LSH) Works, needs valgrind testing
    // test_hash_methods(1); // (Hypercube) Pending

    test_hamming_calculation();


    return 0;

}