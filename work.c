#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <search.h>



int f(char* a[]){
    for(int i=0;i<5;i++){
        printf("%s \n",&a[i]);
    }
}
int main() {
	// char* a[5];
    // for(int i=0;i<5;i++){
    //     &a[i] = "apple";
    // }
    // f(a);
    char* a;
    &a = "apple";
    printf("%s \n",a);
	return 0;
}