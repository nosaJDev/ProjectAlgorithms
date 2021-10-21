#include <cstdio>
#include <cstdlib>

int main(){

    char string[20] = "12 32";

    int a;
    sscanf(string,"%d",&a);
    printf("%d\n",a);
    sscanf(string,"%d",&a);
    printf("%d\n",a);

    return 0;

}