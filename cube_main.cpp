#include <cstdio>
#include <cstdlib>
#include "tests.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int main(int argc,char** argv){
    
    if (argc != 17){
        printf("./cube –i <input file> –q <query file> –k <int> -M <int> -probes <int> -ο <output file> -Ν <number of nearest> -R <radius>\n");
        exit(1);
    }
    char input_file[1000];
    char query_file[1000];
    int k=14;
    int m=10;
    int probes=2;
    char output_file[1000];
    int number_of_nearest=1;
    double radius=  10000;


    //save command line arguements
    for (int i=0; i<argc; i++){
        if (!strcmp(argv[i],"-i")){
            strcpy(input_file,(argv[i+1]));
        }else if (!strcmp(argv[i],"-q")){
            strcpy(query_file,(argv[i+1]));
        }else if (!strcmp(argv[i],"-k")){
            k = atoi(argv[i+1]);
        }else if (!strcmp(argv[i],"-M")){
            m = atoi(argv[i+1]);
        }else if (!strcmp(argv[i],"-probes")){
            probes = atoi(argv[i+1]);
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
