#include <cstdio>
#include <cstdlib>
#include "tests.hpp"
#include <cstring>
#include <ctime>

using namespace std;

int main(int argc,char** argv){
    
    // Initialize the default values
    char input_file[1000] = "";
    char config_file[1000] = "cluster.conf";
    bool complete = false;
    char method[70];
    char output_file[1000];
    int K = -1;
    int LSH_k = 4;
    int LSH_L = 3;
    int Hy_M = 3;
    int Hy_k = 3;
    int Hy_probes = 2;

    //save command line arguements
    for (int i=0; i<argc;){
        if (!strcmp(argv[i],"-i")){
            strcpy(input_file,(argv[i+1]));
            i+=2;
        }else if (!strcmp(argv[i],"-c")){
            strcpy(config_file,(argv[i+1]));
            i+=2;
        }else if (!strcmp(argv[i],"-o")){
            strcpy(output_file,argv[i+1]);
            i+=2;
        }else if (!strcmp(argv[i],"-m")){
            strcpy(method,argv[i+1]);
            i+=2;
        }else if (!strcmp(argv[i],"-complete")){
            complete = true;
            i++;
        }else{
            i++;
        }
    }

    // Read the configuration file to get the rest of the parameters
    FILE * fileconf = fopen(config_file,"r");
    if(fileconf == nullptr){
        printf("Error while reading configuration file, aborting\n");
        return -1;
    }

    while(true){

        // Scan the next parameter
        char par[150];
        if(fscanf(fileconf,"%s",par) == EOF)
            break;

        // Check which parameter is it and scan it
        if(!strcmp(par,"number_of_clusters:"))
            fscanf(fileconf,"%d",&K);
        if(!strcmp(par,"number_of_vector_hash_tables:"))
            fscanf(fileconf,"%d",&LSH_L);
        if(!strcmp(par,"number_of_vector_hash_functions:"))
            fscanf(fileconf,"%d",&LSH_k);
        if(!strcmp(par,"number_of_M_hypercube:"))
            fscanf(fileconf,"%d",&Hy_M);
        if(!strcmp(par,"number_of_hypercube_dimensions:"))
            fscanf(fileconf,"%d",&Hy_k);
        if(!strcmp(par,"number_of_probes:"))
            fscanf(fileconf,"%d",&Hy_probes);
    }

    // Close the configuration and check that the non-defaults are read
    fclose(fileconf);
    if(K == -1){
        printf("Parameter number_of_clusters is missing from the configuration, aborting..\n");
        return -1;
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


    // Check the method of operation to set the hasher accordingly
    PointStruct * hasher;
    if(!strcmp(method,"LSH")){
        strcpy(method,"Range Search LSH");
        hasher = new LSH(LSH_L,LSH_k,4,inputlist->getElems()/8,inputs->getDimensions());
    }else if(!strcmp(method,"Hypercube")){
        strcpy(method,"Range Search Hypercube");
        hasher = new HypercubeMapping(Hy_k,4,inputs->getDimensions(),Hy_probes,Hy_M);
    }else{
        //No hasher, no range search
        strcpy(method,"Lloyds");
        hasher = nullptr;
    }

    // Initialize the clusterer
    //int d, List * pts, int _k,bool range,PointStruct * q
    LloydClusterer * clusterer = new LloydClusterer(inputs->getDimensions(),inputlist,K,hasher!=nullptr,hasher);

    // Perform the clustering
    Metric * metric = new L2Metric();
    clock_t start,end;
    start = clock();
    clusterer->performClustering(metric,20);
    end = clock();
    double c_time = (double) end-start;

    // Calculate the sillouetes
    clusterer->calculateSillouete(metric);

    // Create the file to write the results
    FILE * outfile = fopen(output_file,"w");
    fprintf(outfile,"Algorithm: %s\n",method);

    // Print the results for every cluster
    for(int i = 0; i < K; i++){

        fprintf(outfile,"CLUSTER-%d { %d,",i+1,clusterer->getCluster(i)->getElems());
        clusterer->getCentroid(i)->printFile(outfile);

        // Check if you need to print all the vector labels aswell
        if(complete){
            // If so, add a coma and loop through the cluster
            fprintf(outfile,",");

            List * cluster = clusterer->getCluster(i);
            for(int j = 0; j < cluster->getElems(); j++){
                
                // Find the next vector
                Vector * v = (Vector*)((HashElement*)cluster->get(j))->data;

                //Print in the label
                fprintf(outfile," %s%s",v->getLabel(),(j == cluster->getElems()-1)?"":",");

            }

        }
        // Close the array
        fprintf(outfile,"}\n");
        

    }

    // Then print the clustering time
    fprintf(outfile,"clustering_time:%lf\n",c_time);

    // Finally , print the sillouetes
    fprintf(outfile,"Silouette: [");
    for(int i = 0; i < K+1; i++){
        if(i == K)
            fprintf(outfile,"%f]\n",clusterer->getGlobalSillouete());
        else
            fprintf(outfile," %f,",clusterer->getSillouete(i));
    }

    // After all that, close the file
    fclose(outfile);

    // Free the allocated memory
    delete metric;
    if(hasher != nullptr) delete hasher;
    delete inputs;
    delete clusterer;

    // Exit like a boss
    return 0;

}
