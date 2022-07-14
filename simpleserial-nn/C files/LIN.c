#include "hal.h"
#include<stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "simpleserial.h"



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
    
        //linr
    float x,y,m,c,d;
    int n = 84;
    float sumx=0,sumxsq=0,sumy=0,sumxy=0;

    FILE* file = fopen("LinData.txt","r");
    float numx;
    float numy;
    float* ydata = malloc(sizeof(float)*n);
    float* xdata = malloc(sizeof(float)*n);

    for(int i=0;i<n;i++){
        fscanf(file,"%f",&numx);
        x = numx;
        fseek(file,1,SEEK_CUR);
        fscanf(file,"%f",&numy);
        y = numy;
        ydata[i] = numy;
        xdata[i] = numx;

        sumx=sumx+x;
        sumxsq=sumxsq+(x*x);
        sumy=sumy+y;
        sumxy=sumxy+(x*y);

        fseek(file,1,SEEK_CUR);
    }


    d=n*sumxsq-sumx*sumx;
    m=(n*sumxy-sumx*sumy)/d;
    c=(sumy*sumxsq-sumx*sumxy)/d;
    
    float ans = avgError(xdata,ydata,n,m,c);
//     printf("accuracy : %f ",100.00 - ans*100.00);

    free(ydata);
    free(xdata);
    
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
