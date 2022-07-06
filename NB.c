#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


struct StringArray{
    char* stringname;
};


// will store the unique strings from labels
static char* temp1[5];


// to calculate number of times the label is repeted
int* class_counts(struct StringArray label_column[], int dim){
    for(int i=0;i<dim;i++){
        temp1[i] = "#";
    }


    int curr_pos = 0;
    int found;


    for(int j=0;j<dim;j++){
        found = 0;
        for(int i=0;i<dim;i++){
            if(strcmp(&label_column[j].stringname,temp1[i])==0){
                found = 1;
                break;
            }
        }
        if(found == 0){
            temp1[curr_pos] = (char*)&label_column[j].stringname;
            curr_pos+=1;
        }
    }
    

    char* tempstring ;
    int* counts_array = malloc(sizeof(int)*dim);
    for(int i=0;i<dim;i++){
        counts_array[i] = 0;
    }

    int i = 0;
    while(temp1[i]!="#" && i<dim){
        tempstring = temp1[i];
        for(int j=0;j<dim;j++){
            if(strcmp(tempstring,&label_column[j].stringname)==0){
                (counts_array[i])+=1;
            }
        }
        i++;
    }

    return counts_array;
}


int main(){
    struct StringArray label_column[5];
    for(int i=0;i<5;i++){
        scanf("%s",&label_column[i].stringname);
    }
    int* counts_array = class_counts(label_column,5);

    for(int i=0;i<5;i++){
        printf("%s -> %d \n",temp1[i],counts_array[i]);
    }
}