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
    int numq;
    float gain_data;
};


struct node {
    struct TableData* data;
    struct node* left;
    struct node* right;
    bool IsLEFTLeaf;
    bool IsRIGHTLeaf;
    struct Leaf* LEFTleaf;
    struct Leaf* RIGHTleaf;
};

struct Leaf{
    float ApplePercentage;
    float GrapePercentage;
    float LemonPercentage;
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





struct node* newNode(struct TableData train_data[])
{
    struct node* node = (struct node*)malloc(sizeof(struct node));

    node->data = train_data;
 
    node->left = NULL;
    node->right = NULL;
    node->IsLEFTLeaf = false;
    node->IsRIGHTLeaf = false;
    node->LEFTleaf = NULL;
    node->RIGHTleaf = NULL;
    return (node);
}

struct Leaf* newLeaf(struct TableData train_data[],int n){
    struct Leaf* leaf = (struct Leaf*)malloc(sizeof(struct Leaf));

    int* counts_data = count_labels(train_data,n);

    float sum = 0;
    for(int i=0;i<3;i++){
        sum+=(float)counts_data[i];
    }
    
    leaf->ApplePercentage = ((float)counts_data[0])/sum;
    leaf->GrapePercentage = ((float)counts_data[1])/sum;
    leaf->LemonPercentage = ((float)counts_data[2])/sum;

    return (leaf);
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




struct Question* best_question(struct TableData train_data[],int dim){
    float best_gain = 0.0;
    struct Question* quest = malloc(sizeof(struct Question));
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

    strcpy(quest->q,ques);
    quest->numq = num; 
    quest->gain_data = best_gain;

    return quest;
    
}


// deprecated
struct node* build_Tree(struct TableData train_data[],int dim){
    struct Question* qs = best_question(train_data,5);
    printf("%d \n",qs->numq);
    printf("%s \n",qs->q);
    printf("----------------------------- \n");
    struct TableData*  TRUEdata;
    struct TableData*  FALSEdata;
    int Tc;
    int Fc;

    struct node* root = newNode(train_data);

    if(qs->numq !=-1){
        // numeric
        TRUEdata = TrueRowsFinder(train_data,"#",qs->numq,1,dim,1);
        FALSEdata = FalseRowsFinder(train_data,"#",qs->numq,1,dim,1);
        Tc = TRUErowsLen(train_data,"#",qs->numq,1,dim,1);
        Fc = FALSErowsLen(train_data,"#",qs->numq,1,dim,1);

    }else if(strcmp(qs->q,"#")==0){
        // string
        TRUEdata = TrueRowsFinder(train_data,qs->q,0,0,dim,0);
        FALSEdata = FalseRowsFinder(train_data,qs->q,0,0,dim,0);
        Tc = TRUErowsLen(train_data,qs->q,0,0,dim,0);
        Fc = FALSErowsLen(train_data,qs->q,0,0,dim,0);
    }

    
    struct Question* ForLeafCheck_L = best_question(FALSEdata,Fc);
    struct Question* ForLeafCheck_R = best_question(TRUEdata,Tc);
    float curr_gain_L = ForLeafCheck_L->gain_data;
    float curr_gain_R = ForLeafCheck_R->gain_data;
    // printf("curr_gain_L : %f \n",curr_gain_L);
    // printf("curr_gain_R : %f \n",curr_gain_R);

    //Non Leaf case
    if(curr_gain_L > 0.0){
        root->left = build_Tree(TRUEdata,Tc);
    }
    if(curr_gain_R > 0.0){
        root->right = build_Tree(FALSEdata,Fc);
    }
    if(curr_gain_L == 0.0){
        root->IsLEFTLeaf = true;
        root->LEFTleaf = newLeaf(TRUEdata,Tc);
    }
    if(curr_gain_R == 0.0){
        root->IsRIGHTLeaf = true;
        root->RIGHTleaf = newLeaf(FALSEdata,Fc);
    }

    return root;
    
}

void printTree(struct node* root){
    if(root==NULL){
        return;
    }

    if(root->IsLEFTLeaf==true){
        printf("Found Left Leaf \n");
        printf("Apple %f \n",(root->LEFTleaf->ApplePercentage)*100.0);
        printf("Grapes %f \n",(root->LEFTleaf->GrapePercentage)*100.0);
        printf("Lemon %f \n",(root->LEFTleaf->LemonPercentage)*100.0);
    }
    if(root->IsRIGHTLeaf==true){
        printf("Found Right Leaf \n");
        printf("Apple %f \n",(root->RIGHTleaf->ApplePercentage)*100.0);
        printf("Grapes %f \n",(root->RIGHTleaf->GrapePercentage)*100.0);
        printf("Lemon %f \n",(root->RIGHTleaf->LemonPercentage)*100.0);
    }

    if(root->IsLEFTLeaf==false){
        printTree(root->left);
    }
    if(root->IsRIGHTLeaf==false){
        printTree(root->right);
    }
    
}



int main(){
    struct TableData train_data[5];

    // for(int i=0;i<5;i++){
    //     // only pass int as reference as arrays decay into pointer
    //     scanf("%s",train_data[i].FruitColour);
    //     scanf("%d",&train_data[i].diam);
    //     scanf("%s",train_data[i].FruitName);

    // }
    strcpy(train_data[0].FruitColour,"Green");
    train_data[0].diam = 3;
    strcpy(train_data[0].FruitName , "Apple"); 

    strcpy(train_data[1].FruitColour , "Yellow");
    train_data[1].diam = 3;
    strcpy(train_data[1].FruitName ,"Apple");

    strcpy(train_data[2].FruitColour, "Red");
    train_data[2].diam = 1;
    strcpy(train_data[2].FruitName, "Grape");

    strcpy(train_data[3].FruitColour , "Red");
    train_data[3].diam = 1;
    strcpy(train_data[3].FruitName, "Grape");

    strcpy(train_data[4].FruitColour,"Yellow");
    train_data[4].diam = 3;
    strcpy(train_data[4].FruitName, "Lemon");

    
    // struct TableData*  TRUEdata = TrueRowsFinder(train_data,"Red",3,1,5,1);
    // int Tc = TRUErowsLen(train_data,"#",3,1,5,1);
    // for(int i=0;i<Tc;i++){
    //     // only pass int as reference as arrays decay into pointer
    //     printf("---------------------------------------------- \n");
    //     printf("%s ",TRUEdata[i].FruitColour);
    //     printf("%d ",TRUEdata[i].diam);
    //     printf("%s ",TRUEdata[i].FruitName);
    //     printf("\n");
    // }
    // struct Question* qq = best_question(TRUEdata,Tc);
    // printf("---------------------------------------------- \n");
    // printf("%d \n",qq->numq);
    // printf("%s \n",qq->q);
    // printf("%f \n",qq->gain_data);


    // For testing Binary Tree
    struct node* root = newNode(train_data);
    struct node* head = root;
    struct Question* qq = best_question(train_data,5);

    struct TableData*  TRUEdata = TrueRowsFinder(train_data,qq->q,qq->numq,1,5,1);
    struct TableData*  FALSEdata = FalseRowsFinder(train_data,qq->q,qq->numq,1,5,1);

    int Tc = TRUErowsLen(train_data,qq->q,qq->numq,1,5,1);
    int Fc = FALSErowsLen(train_data,qq->q,qq->numq,1,5,1);

    // printf("TRUE data on right: \n");
    // for(int i=0;i<Tc;i++){
    //     printf("---------------------------------------------- \n");
    //     printf("%s ",TRUEdata[i].FruitColour);
    //     printf("%d ",TRUEdata[i].diam);
    //     printf("%s ",TRUEdata[i].FruitName);
    //     printf("\n");
    // }
    // printf("---------------------------------------------- \n");
    // printf("FALSE data on left: \n");
    // for(int i=0;i<Fc;i++){
    //     printf("---------------------------------------------- \n");
    //     printf("%s ",FALSEdata[i].FruitColour);
    //     printf("%d ",FALSEdata[i].diam);
    //     printf("%s ",FALSEdata[i].FruitName);
    //     printf("\n");
    // }




    root->IsLEFTLeaf = true;
    root->LEFTleaf = newLeaf(FALSEdata,Fc);
    root->right = newNode(TRUEdata);
    

    root = root->right;
    struct Question* qq1 = best_question(TRUEdata,Tc);
    

    struct TableData*  TRUEdata1 = TrueRowsFinder(TRUEdata,qq1->q,0,0,Tc,0);
    int Tc1 = TRUErowsLen(TRUEdata,qq1->q,0,0,Tc,0);
    struct TableData*  FALSEdata1 = FalseRowsFinder(TRUEdata,qq1->q,0,0,Tc,0);
    int Fc1 = FALSErowsLen(TRUEdata,qq1->q,0,0,Tc,0);

    root->IsLEFTLeaf = true;
    root->IsRIGHTLeaf = true;
    root->LEFTleaf = newLeaf(FALSEdata1,Fc1);
    root->RIGHTleaf = newLeaf(TRUEdata1,Tc1);


    // printf("TRUE data on right: \n");
    // for(int i=0;i<Tc1;i++){
    //     printf("---------------------------------------------- \n");
    //     printf("%s ",TRUEdata1[i].FruitColour);
    //     printf("%d ",TRUEdata1[i].diam);
    //     printf("%s ",TRUEdata1[i].FruitName);
    //     printf("\n");
    // }
    // printf("---------------------------------------------- \n");
    // printf("FALSE data on left: \n");
    // for(int i=0;i<Fc1;i++){
    //     printf("---------------------------------------------- \n");
    //     printf("%s ",FALSEdata1[i].FruitColour);
    //     printf("%d ",FALSEdata1[i].diam);
    //     printf("%s ",FALSEdata1[i].FruitName);
    //     printf("\n");
    // }

    printTree(head);
    
}


