#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <search.h>
#include<stdbool.h> 

struct TableData{
    char FruitColour[50];
    int diam;
    char FruitName[50];
};

struct Question{
    char q[50];
    float numq;
};



// for counting the 0,1,2 indexes corrospond to Apple , Grape , Lemon [x,x,x]
int* count_labels(struct TableData train_data[],int rownum){
    int* counts_array = malloc(3*sizeof(int));
    for(int i=0;i<3;i++){
        counts_array[i] = 0;
    }

    for(int i=0;i<rownum;i++){
        if(strcmp(train_data[i].FruitName,"Apple")==0){
            counts_array[0]+=1;
        }
        if(strcmp(train_data[i].FruitName,"Grape")==0){
            counts_array[1]+=1;
        }
        if(strcmp(train_data[i].FruitName,"Lemon")==0){
            counts_array[2]+=1;
        }
    }
    return counts_array;
}




int* TRUEFALSEfinder(struct TableData train_data[],char* question,int NumQuestion,int colnum,int rownum,int IsNumeric){
    int* TrueRowsIndexes = malloc(rownum*sizeof(int));
    int index = 0;

    if(IsNumeric==0){
        // FruitColour
        if(colnum==0){
            for(int i=0;i<rownum;i++){
                if(strcmp(train_data[i].FruitColour,question)==0){
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








struct TableData* TrueRowsFinder(struct TableData train_data[],char* question,int NumQuestion,int colnum,int rownum,int IsNumeric){
    int* TrueRowsIndexes = TRUEFALSEfinder(train_data,question,NumQuestion,colnum,rownum,IsNumeric);
    int c = 0;
    for(int i=0;i<rownum;i++){
        if(TrueRowsIndexes[i]==1){
            c++;
        }
    }
    
    struct TableData*  TRUEdata  = malloc(sizeof(struct TableData)*c);
    int index = 0;
    for(int i=0;i<rownum;i++){
        if(TrueRowsIndexes[i]==1){
            strcpy(TRUEdata[index].FruitColour,train_data[i].FruitColour);
            TRUEdata[index].diam = train_data[i].diam;
            strcpy(TRUEdata[index].FruitName, train_data[i].FruitName);
            index++;
        }
    }


    return TRUEdata;
}
struct TableData* FalseRowsFinder(struct TableData train_data[],char* question,int NumQuestion,int colnum,int rownum,int IsNumeric){
    
    int* TrueRowsIndexes = TRUEFALSEfinder(train_data,question,NumQuestion,colnum,rownum,IsNumeric);
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
    int c = 0;
    for(int i=0;i<rownum;i++){
        if(FalseRowsIndexes[i]==1){
            c++;
        }
    }
    struct TableData*  FALSEdata  = malloc(sizeof(struct TableData)*c);
    int index = 0;
    for(int i=0;i<rownum;i++){
        if(FalseRowsIndexes[i]==1){
            strcpy(FALSEdata[index].FruitColour, train_data[i].FruitColour);
            FALSEdata[index].diam = train_data[i].diam;
            strcpy(FALSEdata[index].FruitName , train_data[i].FruitName);
            index++;
        }
    }

    return FALSEdata;
}







int TRUErowsLen(struct TableData train_data[],char* question,int NumQuestion,int colnum,int rownum,int IsNumeric){
    int* TrueRowsIndexes = TRUEFALSEfinder(train_data,question,NumQuestion,colnum,rownum,IsNumeric);
    int c = 0;
    for(int i=0;i<rownum;i++){
        if(TrueRowsIndexes[i]==1){
            c++;
        }
    }
    return c;
}
int FALSErowsLen(struct TableData train_data[],char* question,int NumQuestion,int colnum,int rownum,int IsNumeric){
    int* TrueRowsIndexes = TRUEFALSEfinder(train_data,question,NumQuestion,colnum,rownum,IsNumeric);
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
    int c = 0;
    for(int i=0;i<rownum;i++){
        if(FalseRowsIndexes[i]==1){
            c++;
        }
    }
    return c;
}






float gini(struct TableData train_data[], int dim ){
    float impurity = 1.0;
        
    int* counts_array = count_labels(train_data,dim);
    for(int i=0;i<3;i++){
        if(counts_array[i]!=0){
            float temp = ((float)counts_array[i])/((float)dim);
            impurity-=(powf(temp,2.0));
        }
    }

    return impurity;
}




float info_gain(struct TableData TRUE_rows[],struct TableData FALSE_rows[],float Current_uncertainity,float Tc , float Fc){
    
    float giniTRUE = gini(TRUE_rows,Tc);
    float giniFALSE = gini(FALSE_rows,Fc);

    float p = (float)(Tc/(Tc+Fc));
    float ans = Current_uncertainity - (p*giniTRUE) - ((1-p)*giniFALSE);

    return ans;
}




void best_question(struct TableData train_data[],int dim){
    float best_gain = 0.0;
    
    float Current_uncertainity = gini(train_data,dim);
    char* ques = "#";
    int num = -1;
    
    for(int i=0;i<dim;i++){
        struct TableData* TRUEdata = TrueRowsFinder(train_data,train_data[i].FruitColour,0,0,dim,0);
        struct TableData* FALSEdata = FalseRowsFinder(train_data,train_data[i].FruitColour,0,0,dim,0);

        int Tc = TRUErowsLen(train_data,train_data[i].FruitColour,0,0,dim,0);
        int Fc = FALSErowsLen(train_data,train_data[i].FruitColour,0,0,dim,0);
        
        if(Tc==0 || Fc==0){
            continue;
        }

        float gain = info_gain(TRUEdata,FALSEdata,Current_uncertainity,(float)Tc,(float)Fc);
        // printf("gain is :%f \n",gain);
        if(gain>=best_gain){
            ques = train_data[i].FruitColour;
            num = -1;
            best_gain = gain;
        }
    }
    
    for(int i=0;i<dim;i++){
        struct TableData* TRUEdata = TrueRowsFinder(train_data,train_data[i].FruitColour,train_data[i].diam,1,dim,1);
        struct TableData* FALSEdata = FalseRowsFinder(train_data,train_data[i].FruitColour,train_data[i].diam,1,dim,1);


        int Tc = TRUErowsLen(train_data,train_data[i].FruitColour,train_data[i].diam,1,dim,1);
        int Fc = FALSErowsLen(train_data,train_data[i].FruitColour,train_data[i].diam,1,dim,1);

        if(Tc==0 || Fc==0){
            continue;
        }


        float gain = info_gain(TRUEdata,FALSEdata,Current_uncertainity,(float)Tc,(float)Fc);
        // printf("gain is :%f \n",gain);
        if(gain>=best_gain){
            num = train_data[i].diam;
            ques = "#";
            best_gain = gain;
        }
    }

    
    
    
}





int main(){
    struct TableData train_data[5];

    for(int i=0;i<5;i++){
        // only pass int as reference as arrays decay into pointer
        scanf("%s",train_data[i].FruitColour);
        scanf("%d",&train_data[i].diam);
        scanf("%s",train_data[i].FruitName);
    }

    struct TableData*  TRUEdata = TrueRowsFinder(train_data,"Red",3,1,5,1);
    int Tc = TRUErowsLen(train_data,"Red",3,1,5,1);

    // for(int i=0;i<Tc;i++){
    //     // only pass int as reference as arrays decay into pointer
    //     printf("---------------------------------------------- \n");
    //     printf("%s ",TRUEdata[i].FruitColour);
    //     printf("%d ",TRUEdata[i].diam);
    //     printf("%s ",TRUEdata[i].FruitName);
    //     printf("\n");
    // }

    best_question(train_data,5);
    
    
}


