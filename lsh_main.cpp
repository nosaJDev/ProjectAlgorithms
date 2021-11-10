#include <cstdio>
#include <cstdlib>
#include "tests.hpp"
#include <cstring>
#include <ctime>

using namespace std;

int main(int argc,char** argv){
    
    // Set the initial default values
    char input_file[1000] = "";
    char query_file[1000] = "";
    int k=4;
    int L=5;
    char output_file[1000] = "";
    int N=1;
    float radius=10000;


    // Parse the command line arguments
    for (int i=0; i<argc;){
        if (!strcmp(argv[i],"-i")){
            strcpy(input_file,(argv[i+1]));
            i += 2;
        }else if (!strcmp(argv[i],"-q")){
            strcpy(query_file,(argv[i+1]));
            i += 2;
        }else if (!strcmp(argv[i],"-k")){
            k = atoi(argv[i+1]);
            i += 2;
        }else if (!strcmp(argv[i],"-L")){
            L = atoi(argv[i+1]);
            i += 2;
        }else if (!strcmp(argv[i],"-o")){
            strcpy(output_file,argv[i+1]);
            i += 2;
        }else if (!strcmp(argv[i],"-N")){
            N = atoi(argv[i+1]);
            i += 2;
        }else if (!strcmp(argv[i],"-R")){
            radius = atof(argv[i+1]);
            i += 2;
        }else{
            i += 1;
        }
    }

    // Check it the user has entered the input file correctly
    if(!strcmp(input_file,"")){
        // Ask for a new input file
        printf("Please enter input file: ");
        scanf("%s",input_file);
    }

    // Load up the input file into a VectorFile parser
    VectorFile * inputs = new VectorFile(input_file);
    List * inputlist = inputs->getVectorList();

    // Check that the file was read successfully
    if(inputlist == nullptr){
        printf("Error, input file was not parsed correctly\n");
        delete inputs;
        return -1;
    }

    // Set the parser for the query file
    VectorFile * queries;

    // Setup the hasher to handle the queries
    // and insert the list of vectors to it
    // int _L, int _k,int _w, int buckets, int dim
    PointStruct * hasher = new LSH(L,k,4,inputlist->getElems()/8,inputs->getDimensions());
    hasher->addVectorList(inputlist);

    // Setup a metric for the calculations
    Metric * metric = new L2Metric();

    // Try to run the search
    while(true){

        // Check that the user has provided a query file
        if(!strcmp(query_file,"")){
            printf("Please enter a query file: ");
            scanf("%s",query_file);
        }

        // Parse the query file and the list
        queries = new VectorFile(query_file);
        List * querylist = queries->getVectorList();

        // Check that there were no errors
        if (querylist == nullptr){
            printf("Error, query file was not parsed correctly\n");
            delete inputs;
            delete queries;
            delete hasher;
            delete metric;
            return -1;
        }

        // Create an output file and open it
        strcpy(output_file+strlen(output_file),"_");
        strcpy(output_file+strlen(output_file),input_file);
        for(int i = 0; i < strlen(output_file); i++){
            if(output_file[i] == '/') output_file[i] = '_';
        }
        
        FILE * outfile = fopen(output_file,"w+");
        if(outfile == nullptr){
            printf("Hey");
            return 1;
        }

        // Set variables for measuring time
        clock_t start, end;

        // Perform the searches for every element on the query set
        for(int i = 0; i < querylist->getElems(); i++){
            
            // Find the vector you will be querying
            Vector * q = (Vector*) querylist->get(i);

            // Find the nearest neighbors of the vector
            start = clock();
            PriorityQueue * nearest = hasher->approximatekNN(q,metric);
            end = clock();
            double tnearest = (double) end-start;

            // Find the R-near neighbors of the vector
            
            PriorityQueue * rnear = hasher->approximateRange(radius,q,metric);

            // Find the true nearest neighbors
            start = clock();
            PriorityQueue * truenn = new PriorityQueue(10);
            for(int j = 0; j < inputlist->getElems(); j++){
                
                // Get the vector
                Vector * v = (Vector*) inputlist->get(j);

                // Find the distance and insert it
                float d = metric->dist(q,v);
                truenn->add((void*)v,d);

            }
            end = clock();
            double ttruenn = (double) end-start;

            // Write the results to the file
            fprintf(outfile,"Query: %s\n",q->getLabel());
            
            // Loop-write the nn results
            for(int j = 0; j < N; j++){
                
                // Pop the next nn
                float nnd = nearest->peekPriority();
                Vector * nn = (Vector*) nearest->remove();

                // Pop the next true nn
                float tnnd = truenn->peekPriority();
                truenn->remove();

                // Write the stats
                fprintf(outfile,"Nearest neighbor-1: %s\n",(nn!=nullptr)?nn->getLabel():"none");
                fprintf(outfile,"distanceLSH: %f\n",nnd);
                fprintf(outfile,"distanceTrue: %f\n",tnnd);
            }

            // Write the times
            fprintf(outfile,"tLSH: %lf\n",tnearest);
            fprintf(outfile,"tTrue: %lf\n",ttruenn);

            // Write the r-near results
            while(rnear->getElems() > 0){
                
                // Pop the next r-near and write to file
                Vector * rn = (Vector*) rnear->remove();
                fprintf(outfile,"%s\n",rn->getLabel());
            }
            fprintf(outfile,"\n");

            // Destroy the queues
            delete nearest;
            delete rnear;
            delete truenn;

        }

        // Finally close the file
        fclose(outfile);
        
        // Ask for a new search
        char answer[4];
        printf("Would you like to repeat the search with a different queryset? (y/n)\n");
        scanf("%s",answer);

        // Check what he answered
        if (answer[0]=='y'){
            
            // Make sure to reset the queryset name so it is asked again
            strcpy(query_file,"");
            
            // Go on to the next loop
            continue;

        }else if (answer[0]=='n'){
            
            // If he wishes to finish, break out and exit
            break;

        }

    }

    // Print the exit message
    printf("Exiting\n");

    // Clean up the memory
    delete inputs;
    delete queries;
    delete hasher;
    delete metric;

    return 0;

}
