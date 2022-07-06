#include <stdio.h>
#include <stdlib.h>
#include <math.h>


float avgError(float* xdata,float* ydata,int n,float m,float c){
    float temp = 0.0;
    float pred ;
    for(int i=0;i<n;i++){
        pred = (xdata[i]*m + c);
        temp+= (fabs(ydata[i] - pred))/n;
    }
    return temp;
}


int main(){
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

    printf("m = %f and c = %f \n",m,c);

    float ans = avgError(xdata,ydata,n,m,c);
    printf("accuracy : %f ",100.00 - ans*100.00);

    free(ydata);
    free(xdata);
}