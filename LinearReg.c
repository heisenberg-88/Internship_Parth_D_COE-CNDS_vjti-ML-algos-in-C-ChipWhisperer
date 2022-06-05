#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// sigmoid of whole array
// tested & working
float sigmoid(float n){
    float s = 1/(1 + exp(-n));
    return s;
}



//initialize weights with with 0
// tested and working
float* weightinit(int dim){
    float *w = malloc(sizeof(float) * dim);
    for(int i=0;i<dim;i++){
        w[i] = 0.0;
    }
    return w;
}




//forward propagation 
//tested and working
// put 2nd dimention in input x array
float* propagate(float* w, float b ,float x[][3],float y[], int dim , int NoOfEx){
    
    // activated value calculated for each value
    // tested and working 
    float A[NoOfEx];
    for(int i=0;i<NoOfEx;i++){
        // printf("Example no : %d \n",i);
        float temp = 0;
        for(int j=0;j<dim;j++){
            // printf("multiplying %f with %f \n",w[j],x[j][i]);
            temp+=(w[j]*x[j][i]);
        }
        // printf("current temp value: %f \n",temp+b);
        A[i] = sigmoid(temp+b);
    }
    // for(int i=0;i<NoOfEx;i++){
    //     printf("%f \n",A[i]);
    // }



    // cost calculated considering all values
    //tested and working
    float cost = 0;
    for(int i=0;i<NoOfEx ;i++){
        cost += ((y[i]*(-log(A[i])) + (1-y[i])*(-log(1-A[i])))); 
    }
    cost/=NoOfEx;
    // printf("the cost value : %f \n",cost);



    // gradients for back propagation
    //tested and working
    float *dw = malloc(sizeof(float) * (dim+2));
    float temp1[NoOfEx];
    for(int i=0;i<NoOfEx;i++){
        // printf("sub %f - %f \n",A[i],y[i]);
        temp1[i] = (A[i]-y[i]);
    }
    // printf("values in temp1 are: \n");
    // for(int i=0;i<NoOfEx;i++){
    //     printf("%f \n",temp1[i]);
    // }
    for(int i=0;i<dim;i++){
        float temp2 = 0;
        for(int j=0;j<NoOfEx;j++){
            temp2 += (temp1[j]*x[i][j]);
        }
        temp2/=NoOfEx;
        dw[i] = temp2;
    }
    // printf("the dw values are : \n");
    // for(int i=0;i<dim;i++){
    //     printf("%f \n",dw[i]);
    // }

    
    float db = 0;
    for(int i=0;i<NoOfEx;i++){
        db+=temp1[i];
    }
    db/=NoOfEx;
    dw[dim] = db;
    dw[dim+1] = cost;
    // printf("db value: %f \n",dw[dim]);
    // printf("cost value: %f \n",dw[dim+1]);


    return dw;
}







// optimization function
// tested and working
// put 2nd dimention in input x array
float* optimize(float* w,float b,float x[][3],float y[],int num_iterations,float learning_rate, int dim ,int NoOfEx){

    // new weights vector for updating
    float wnew[dim];
    for(int i=0;i<dim;i++){
        wnew[i] = w[i];
    }
    // new bias for updating
    float bnew = b;
    float db;
    float cost;
    float dw[dim];



    // for loop for num_iterations
    // tested and working
    for(int i = 0;i<num_iterations;i++){

        float* dwinfo = propagate(wnew,bnew,x,y,dim,NoOfEx);
        for(int i=0;i<dim;i++){
            dw[i] = dwinfo[i];
        }
        db = dwinfo[dim];
        cost = dwinfo[dim+1];


        //updating weights and bias
        for(int i=0;i<dim;i++){
            // printf("new wnew is : %f - ((%f)*(%f)) )",wnew[i],dw[i],learning_rate);
            // printf("\n");
            wnew[i] = wnew[i] - ((dw[i])*(learning_rate));
        }
        // for(int i=0;i<dim;i++){
        //     printf("new wnew values are: %f \n",wnew[i]);
        // }
        bnew = bnew - ((db)*(learning_rate));
        // printf("new bnew values are: %f \n",bnew);

        if(i%100==0){
            printf("cost after %d iterations is %f \n",i+1,cost);
        }
    }



    float *infovec = malloc(sizeof(float) * (dim+dim+2));
    
    // storing values in infovec as {wnew values , dw values , bnew values , db values} and then return
    for(int i=0;i<dim;i++){
        infovec[i] = wnew[i];
    }
    int j=0;
    for(int i=dim;i<2*dim;i++){
        infovec[i] = dw[j];
        j++;
    }
    infovec[dim+dim] = bnew;
    infovec[dim+dim+1] = db;


    return infovec;
}







// predict outputs y_preds
// tested and working
// put 2nd dimention in input x array
float* predict(float* w,float b, float x[][3],int dim,int NoOfEx){

    float *y_predictions = malloc(sizeof(float) * (dim));

    // calculating sigmoid values
    float A[NoOfEx];
    for(int i=0;i<NoOfEx;i++){
        float temp = 0;
        for(int j=0;j<dim;j++){
            temp+=(w[j]*x[j][i]);
        }
        A[i] = sigmoid(temp+b);
    }

    // printf("These are A[i] values \n");
    // for(int i=0;i<NoOfEx;i++){
    //     printf("%f \n",A[i]);
    // }
    // printf("------------ \n");

    float* y_preds = malloc(sizeof(float)*dim);

    for(int i=0;i<dim;i++){
        if(A[i]>0.5){
            y_preds[i] = 1;
        }
        else{
            y_preds[i] = 0;
        }
    }

    return y_preds;
}






// printing the accuracy
// testing
// put 2nd dimention in input x array
void printaccuracy(float x_train[][3],float y_train[],float x_test[][3],float y_test[]){
    float* w;
    int dimention ;  //hardcode the value here
    

    // initialize weights and biases
    w = weightinit(dimention);
    float b = 0.0;
    

    // optimize function for optimizing weights and biases
    float* infovec = optimize(w,b,x_train,y_train,2000,0.5,dimention);

    float wfinal[dimention];
    float bfinal = 0.0;

    for(int i=0;i<dimention;i++){
        wfinal[i] = infovec[i];
    }
    bfinal = infovec[dimention+dimention];

    float* y_preds_train;
    float* y_preds_test;

    y_preds_train = predict(wfinal,bfinal,x_train,dimention,NoOfEx);
    y_preds_test = predict(wfinal,bfinal,x_test,dimention,NoOfEx);


    // calculating training accuracy
    float accu_train = 0.0;
    float temp = 0.0;
    for(int i=0;i<dimention;i++){
        temp+= (abs(y_train[i] - y_preds_train[i]));
    }
    temp/=(dimention);
    accu_train = (100 - (temp)*100);


    // calculating testing accuracy
    float accu_test = 0.0;  
    temp = 0.0;
    for(int i=0;i<dimention;i++){
        temp+=(abs(y_test[i] - y_preds_test[i]));
    }
    temp/=(dimention);
    accu_test = temp;

    printf("training accuracy: %d \n", accu_train);
    printf("testing accuracy: %d \n", accu_test);

}




int main() {
    float w[2];
    for(int i=0;i<2;i++){
        scanf("%f",&w[i]);
    }
    float b = 1.5;
    float x[2][3];
    for(int i=0;i<2;i++){
        for(int j=0;j<3;j++){
            scanf("%f",&x[i][j]);
        }
    }
    float y[3];
    for(int i=0;i<3;i++){
        scanf("%f",&y[i]);
    }
    // float* dwinfo = propagate(w,b,x,y,2,3);
    // float* infovec = optimize(w,b,x,y,200,0.9,2,3);
    float* y_preds = predict(w,b,x,2,3);
    for(int i=0;i<3;i++){
        printf("%f \n",y_preds[i]);
    }
    return 0;
}
