#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <search.h>

struct TableData{
    char* FruitColour;
    int diam;
    char* FruitName;
};

struct StringArray{
    char* stringname;
};



void class_counts(struct StringArray label_column[], int dim){
    char* temp1[dim];
    for(int i=0;i<dim;i++){
        temp1[i] = "#";
    }

    // for(int i=0;i<dim;i++){
    //     printf("%s \n",&label_column[i].stringname);
    //     temp1[i] = (char*)&label_column[i].stringname;
    // }

    int curr_pos = 0;
    int found;

    for(int j=0;j<dim;j++){
        found = 0;
        for(int i=0;i<dim;i++){
            if(strcmp(label_column[j].stringname,temp1[i])==0){
                printf(" %s and %s is same \n",&label_column[j],temp1[i]);
                found = 1;
                break;
            }
        }

        if(found == 0){
            temp1[curr_pos] = (char*)&label_column[j].stringname;
        }
    }
    
    printf("This is the temp1 array: \n");
    for(int i=0;i<dim;i++){
        printf("%s \n",temp1[i]);
    }

    // char* tempstring;
    // int* counts_array = malloc(sizeof(int)*dim);
    // for(int i=0;i<dim;i++){
    //     counts_array[i] = 0;
    // }
    // int i = 0;
    // while(temp1[i]!="#"){
    //     tempstring = temp1[i];
    //     for(int j=0;j<dim;j++){
    //         if(strcmp(tempstring,label_column[j].stringname)){
    //             (counts_array[i])+=1;
    //         }
    //     }
    //     i++;
    // }

    // return counts_array;
}






int main(){
    // struct TableData train_data[5];

    // for(int i=0;i<5;i++){
    //     scanf("%s",&train_data[i].FruitColour);
    //     scanf("%d",&train_data[i].diam);
    //     scanf("%s",&train_data[i].FruitName);
    // }

    // printf("--------------------- \n");
    // for(int i=0;i<5;i++){
    //     printf("%s \n",&train_data[i].FruitColour);
    //     printf("%d \n",train_data[i].diam);
    //     printf("%s \n",&train_data[i].FruitName);
    //     printf("--------------------- \n");
    // }


    struct StringArray strings[5];
    for(int i=0;i<5;i++){
        scanf("%s",&strings[i].stringname);
    }
    
    class_counts(strings,5);

    

}

