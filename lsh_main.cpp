#include <cstdio>
#include <cstdlib>
#include "tests.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int main(int argc,char** argv){
    
    if (argc != 15){
        printf("usage: $./lsh –i inputfile –q queryfile –k int -L int -ο outputfile -Ν numberofnearest -R radius\n");
        exit(1);
    }
    char input_file[1000];
    char query_file[1000];
    int k;
    int l;
    char output_file[1000];
    int number_of_nearest;
    double radius;


    //save command line arguements
    for (int i=0; i<argc; i++){
        if (!strcmp(argv[i],"-i")){
            strcpy(input_file,(argv[i+1]));
        }else if (!strcmp(argv[i],"-q")){
            strcpy(query_file,(argv[i+1]));
        }else if (!strcmp(argv[i],"-k")){
            k = atoi(argv[i+1]);
        }else if (!strcmp(argv[i],"-L")){
            l = atoi(argv[i+1]);
        }else if (!strcmp(argv[i],"-o")){
            strcpy(output_file,argv[i+1]);
        }else if (!strcmp(argv[i],"-N")){
            number_of_nearest = atoi(argv[i+1]);
        }else if (!strcmp(argv[i],"-R")){
            radius = atof(argv[i+1]);
        }
    }

    //runs

    char answer[4];
    printf("Would you like to repeat the search with a different datasearch? (y/n)");
    scanf("%s",answer);

    if (answer[0]=='y'){
        printf("Restarting\n");
    }else if (answer[0]=='n'){
        printf("Exiting\n");
    }

    //re-run?






    return 0;

}
