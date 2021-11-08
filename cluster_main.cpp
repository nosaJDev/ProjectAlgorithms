#include <cstdio>
#include <cstdlib>
#include "tests.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int main(int argc,char** argv){
    
    if (argc != 11){
        
        printf("$./cluster –i <input file> –c <configuration file> -o <output file> -complete<optional> -m <method: (1)Classic OR (2)LSH or (3)Hypercube>\n");
        exit(1);
    }
    char input_file[1000];
    char config_file[1000];
    int complete;
    int method=1;
    char output_file[1000];


    //save command line arguements
    for (int i=0; i<argc; i++){
        if (!strcmp(argv[i],"-i")){
            strcpy(input_file,(argv[i+1]));
        }else if (!strcmp(argv[i],"-c")){
            strcpy(config_file,(argv[i+1]));
        }else if (!strcmp(argv[i],"-o")){
            strcpy(output_file,argv[i+1]);
        }else if (!strcmp(argv[i],"-m")){
            method = atoi(argv[i+1]);
        }else if (!strcmp(argv[i],"-complete")){
            complete = atoi(argv[i+1]);
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
