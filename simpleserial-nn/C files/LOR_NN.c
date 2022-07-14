#include "hal.h"
#include<stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include<stdbool.h> 
#include <string.h>
#include <search.h>

#include "simpleserial.h"





// sigmoid
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




static float x_train[4096][2];
static float y_train[2];
static int NoOfEx = 2;





//forward propagation 
//tested and working
float* propagate(float* w, float b , int dim ){

    // activated value calculated for each value
    // tested and working 
    float A[NoOfEx];
    for(int i=0;i<NoOfEx;i++){
        float temp = 0.0;
        for(int j=0;j<dim;j++){
            temp+=(w[j]*x_train[j][i]);
        }
        A[i] = sigmoid(temp+b);
    }
    
    
    // cost calculated considering all values
    //tested and working
    float cost = 0;
    for(int i=0;i<NoOfEx ;i++){
        cost += ((y_train[i]*(-log(A[i])) + (1-y_train[i])*(-log(1-A[i])))); 
    }
    cost/=NoOfEx;



    // gradients for back propagation
    //tested and working
    float *dw = malloc(sizeof(float) * (dim+2));
    float temp1[NoOfEx];
    for(int i=0;i<NoOfEx;i++){
        temp1[i] = (A[i]-y_train[i]);
    }

    for(int i=0;i<dim;i++){
        float temp2 = 0;
        for(int j=0;j<NoOfEx;j++){
            temp2 += (temp1[j]*x_train[i][j]);
        }
        temp2/=NoOfEx;
        dw[i] = temp2;
    }
    

    float db = 0;
    for(int i=0;i<NoOfEx;i++){
        db+=temp1[i];
    }
    db/=NoOfEx;
    dw[dim] = db;
    dw[dim+1] = cost;

    return dw;
}








// optimization function
// tested and working
float* optimize(float* w,float b,int num_iterations,float learning_rate, int dim ){

    // new weights vector for updating
    float *wnew = malloc(sizeof(float) * (dim));
    for(int i=0;i<dim;i++){
        wnew[i] = w[i];
    }
    // new bias for updating
    float bnew = b;
    float db;
    float cost;
    float dw[dim];

    float vartemp = 0.0;

    // for loop for num_iterations
    // tested and working
    for(int i = 0;i<num_iterations;i++){

        float* dwinfo = propagate(wnew,bnew,dim);
        for(int i=0;i<dim;i++){
            dw[i] = dwinfo[i];
        }
        db = dwinfo[dim];
        cost = dwinfo[dim+1];

        //updating weights and bias
        for(int i=0;i<dim;i++){
            wnew[i] = wnew[i] - ((dw[i])*(learning_rate));
        }


        bnew = bnew - ((db)*(learning_rate));

        if(i%100==0){
            printf("cost/loss after %d iterations is %f \n",i+1,cost);
            vartemp = cost;
        }
    }
    float tempac = (100.0)*vartemp;
    printf("Accuracy : %f \n",100.0-tempac);

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
float* predict(float* w,float b,int dim){

    // calculating sigmoid values
    float A[NoOfEx];
    for(int i=0;i<NoOfEx;i++){
        float temp = 0.0;
        for(int j=0;j<dim;j++){
            temp+=(w[j]*x_train[j][i]);
        }
        A[i] = sigmoid(temp+b);
    }


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
void printaccuracy(int dimention){

    // initialize weights and biases
    float* w = weightinit(dimention);
    float b = 0.0;
    
    
    // optimize function for optimizing weights and biases
    float* infovec = optimize(w,b,2000,0.005,dimention);

    float wfinal[dimention];
    float bfinal = 0.0;


    for(int i=0;i<dimention;i++){
        wfinal[i] = infovec[i];
    }
    bfinal = infovec[dimention+dimention];


    float* y_preds_train;
    y_preds_train = predict(wfinal,bfinal,dimention);


    // calculating training accuracy
    float accu_train = 0.0;
    float temp = 0.0;
    for(int i=0;i<NoOfEx;i++){
        temp+= (abs(y_train[i] - y_preds_train[i]));
    }
    temp/=(dimention);
    accu_train = (100 - (temp)*100);

    // printf("training accuracy: %f \n", accu_train);

}





uint8_t get_key(uint8_t* k, uint8_t len)
{
	// Load key here
	return 0x00;
}
static void delay_2ms(void)
{
    for(volatile unsigned int i=0;i<0xfff;i++){
        ;
    }
}
float avgError(float* xdata,float* ydata,int n,float m,float c){
    float temp = 0.0;
    float pred ;
    for(int i=0;i<n;i++){
        pred = (xdata[i]*m + c);
        temp+= (fabs(ydata[i] - pred))/n;
    }
    return temp;
}
#if SS_VER == SS_VER_2_0
uint8_t get_pt(uint8_t cmd ,uint8_t scmd,uint8_t len,uint8_t* pt)
#else
uint8_t get_pt(uint8_t* pt, uint8_t len)
#endif
    
{
	/**********************************
	* Start user-specific code here. */
	trigger_high();

	//16 hex bytes held in 'pt' were sent
	//from the computer. Store your response
	//back into 'pt', which will send 16 bytes
	//back to computer. Can ignore of course if
	//not needed
    
    
   
    
    FILE* file = fopen("mydat.txt","r");
    float num;
    for(int j=0;j<2;j++){
        for(int i=0;i<4096;i++){
            fscanf(file,"%f",&num);
            x_train[i][j] = (num/255.00);
            fseek(file,1,SEEK_CUR);
        }
    }


    FILE* filey = fopen("ydata.txt","r");
    for(int i=0;i<2;i++){
        fscanf(filey,"%f",&num);
        y_train[i] = num;
        fseek(file,1,SEEK_CUR);
    }


    printaccuracy(4096);

   
    
    
    
    delay_2ms();
	trigger_low();
	/* End user-specific code here. *
	********************************/
	simpleserial_put('r', 16, pt);
	return 0x00;
}

uint8_t reset(uint8_t* x, uint8_t len)
{
	// Reset key here if needed
	return 0x00;
}



int main(void)
{
    platform_init();
	init_uart();
	trigger_setup();

 	/* Uncomment this to get a HELLO message for debug */
	/*
	putch('h');
	putch('e');
	putch('l');
	putch('l');
	putch('o');
	putch('\n');
	*/

	simpleserial_init();
	simpleserial_addcmd('p', 16, get_pt);
#if SS_VER != SS_VER_2_0
	simpleserial_addcmd('k', 16, get_key);
	simpleserial_addcmd('x', 0, reset);
#endif
	while(1)
		simpleserial_get();
}
