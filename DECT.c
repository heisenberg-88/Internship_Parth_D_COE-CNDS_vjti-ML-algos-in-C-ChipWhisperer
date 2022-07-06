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


// will store the unique strings from labels
static char* uniqueStrings[5];
// will store unique numeric values
static float uniqueuNums[5];



// to calculate number of times the label is repeted
int* class_counts(struct StringArray label_column[],float label_columnNum[], int dim,int IsNumeric){
    int* counts_array = malloc(sizeof(int)*dim);
    if(IsNumeric==0){       /// for non numeric data
        for(int i=0;i<dim;i++){
            uniqueStrings[i] = "#";
        }

        int curr_pos = 0;
        int found;
        // adding unique values
        for(int j=0;j<dim;j++){
            found = 0;
            for(int i=0;i<dim;i++){
                if(strcmp(&label_column[j].stringname,uniqueStrings[i])==0){
                    found = 1;
                    break;
                }
            }
            if(found == 0){
                uniqueStrings[curr_pos] = (char*)&label_column[j].stringname;
                curr_pos+=1;
            }
        }
        

        char* tempstring ;
        
        for(int i=0;i<dim;i++){
            counts_array[i] = 0;
        }

        int i = 0;
        while(uniqueStrings[i]!="#" && i<dim){
            tempstring = uniqueStrings[i];
            for(int j=0;j<dim;j++){
                if(strcmp(tempstring,&label_column[j].stringname)==0){
                    (counts_array[i])+=1;
                }
            }
            i++;
        }
    }
    else{      /// for numeric data
        for(int i=0;i<dim;i++){
            uniqueuNums[i] = -1;
        }

        int curr_pos = 0;
        int found;
        // adding unique values
        for(int j=0;j<dim;j++){
            found = 0;
            for(int i=0;i<dim;i++){
                if(label_columnNum[j]==uniqueuNums[i]){
                    found = 1;
                    break;
                }
            }
            if(found == 0){
                uniqueuNums[curr_pos] = label_columnNum[j];
                curr_pos+=1;
            }
        }
        

        
        for(int i=0;i<dim;i++){
            counts_array[i] = 0;
        }

        int i = 0;
        while(uniqueuNums[i]!=-1 && i<dim){
            for(int j=0;j<dim;j++){
                if(uniqueuNums[i]==label_columnNum[j]){
                    (counts_array[i])+=1;
                }
            }
            i++;
        }
    }
    

    return counts_array;
}




int* TrueRowfinder(struct TableData train_data[],char* question,int NumQuestion,int colnum,int rownum,int IsNumeric){
    int* TrueRowsIndexes = malloc(rownum*sizeof(int));
    int index = 0;

    if(IsNumeric==0){
        // FruitColour
        if(colnum==0){
            for(int i=0;i<rownum;i++){
                if(strcmp(&train_data[i].FruitColour,question)==0){
                    TrueRowsIndexes[index] = 1;
                }
                index++;
            }
        }
        // FruitName
        else if(colnum==2){
            for(int i=0;i<rownum;i++){
                if(train_data[i].FruitName == question){
                    TrueRowsIndexes[index] = 1;
                }
                index++;
            }
        }
    }
    else if(IsNumeric==1){
        // FruitDiam
        if(colnum==1){
            for(int i=0;i<rownum;i++){
                if(train_data[i].diam >= NumQuestion){
                    TrueRowsIndexes[index] = 1;
                }
                index++;
            }
        }
    }

    return TrueRowsIndexes;
    
}


void TrueFalseRowsPrinter(struct TableData train_data[],char* question,int NumQuestion,int colnum,int rownum,int IsNumeric){
    int* TrueRowsIndexes = TrueRowfinder(train_data,question,NumQuestion,colnum,rownum,IsNumeric);
    int* FalseRowsIndexes = malloc(rownum*sizeof(int));
    
    int findex = 0;
    for(int i=0;i<rownum;i++){
        if(TrueRowsIndexes[i] == 1){
            FalseRowsIndexes[findex] = 0;
        }else{
            FalseRowsIndexes[findex] = 1;
        }
        findex++;
    }

    printf("--------------------- \n");
    printf("True row value for ");
    if(IsNumeric==0){
        printf("%s \n",question);
    }
    else{
        printf("%d \n",NumQuestion);
    }
    printf("--------------------- \n");
    for(int i=0;i<rownum;i++){
        if(TrueRowsIndexes[i]==1){
            printf("%s \n",&train_data[i].FruitColour);
            printf("%d \n",train_data[i].diam);
            printf("%s \n",&train_data[i].FruitName);
            printf("--------------------- \n");
        }
    }
    printf("\n");
    printf("\n");
    printf("--------------------- \n");
    printf("False row value for ");
    if(IsNumeric==0){
        printf("%s \n",question);
    }
    else{
        printf("%d \n",NumQuestion);
    }
    printf("--------------------- \n");
    for(int i=0;i<rownum;i++){
        if(FalseRowsIndexes[i]==1){
            printf("%s \n",&train_data[i].FruitColour);
            printf("%d \n",train_data[i].diam);
            printf("%s \n",&train_data[i].FruitName);
            printf("--------------------- \n");
        }
    }


}


float gini(struct StringArray label_column[],float label_columnNum[],int dim , int IsNumeric){
    float impurity = 1.0;
    if(IsNumeric==0){
        int* counts_array = class_counts(label_column,label_columnNum,dim,0);
        int i=0;
        while(uniqueStrings[i]!="#" && i<dim){
            // printf("count value is %d \n",counts_array[i]);
            float temp = (counts_array[i])/((float)dim);
            // printf("temp value is %f \n",temp);
            impurity-=(pow(temp,2.0));
            i++;
        }
    }
    else{
        int* counts_array = class_counts(label_column,label_columnNum,dim,1);
        int i=0;
        while(uniqueuNums[i]!=-1 && i<dim){
            // printf("count value is %d \n",counts_array[i]);
            float temp = ((float)counts_array[i])/((float)dim);
            // printf("temp value is %f \n",temp);
            impurity-=(pow(temp,2.0));
            i++;
        }
    }
    return impurity;
}


float info_gain(struct TableData TrueRows[],struct TableData FalseRows[],float Current_uncertainity,int Tc,int Fc,int colnum, int dim){
    float label_columnNum[5];
    struct StringArray label_column[5];
    
    float giniTRUE = gini(label_column,label_columnNum,dim,IsNumeric);
    float giniFALSE = gini(label_column,label_columnNum,dim,IsNumeric);

    float p = (float)(Tc/(Tc+Fc));
    float ans = Current_uncertainity -  
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


    struct StringArray label_column[5];
    for(int i=0;i<5;i++){
        scanf("%s",&label_column[i].stringname);
    }
    float label_columnNum[5];
    for(int i=0;i<5;i++){
        label_columnNum[i] = i;
    }
    
    int* counts_array = class_counts(label_column,label_columnNum,5,0);

    for(int i=0;i<5;i++){
        printf("%s -> %d \n",uniqueStrings[i],counts_array[i]);
    } 

    printf("Gini: ------------------------\n");
    float ans = gini(label_column,label_columnNum,5,0);
    printf("%f \n",ans);
    
}


