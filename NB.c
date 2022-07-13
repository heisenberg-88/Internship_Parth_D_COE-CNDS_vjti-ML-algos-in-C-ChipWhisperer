#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Classes
// yes = 1 & no = 0
static int Play[14] = {0,0,1,1,1,0,1,0,1,1,1,1,1,0};

// Features
// Rainy = 1 , Overcast = 2 , Sunny = 3
static int Outlook[14] = {1,1,2,3,3,3,2,1,1,1,1,2,2,3};
static float OutlookLikelihood[3][2];

// Hot = 1 , Mild = 2 , Cold = 3
static int Temp[14] = {1,1,1,2,3,3,3,2,3,2,2,2,1,2};
static float TempLikelihood[3][2];

// Humidity High = 1 , Normal = 0
static int Humidity[14] = {1,1,1,1,0,0,0,1,0,0,0,1,0,1};
static float HumidityLikelihood[2][2];

// Windy Yes = 1 & No = 0
static int Windy[14] = {0,1,0,0,0,1,1,0,0,0,1,1,0,1};
static float WindyLikelihood[3][2];



static float YesPriorProb = 0.0;
static float NoPriorProb = 0.0;
static float YesPriorProb_counter = 0.0;
static float NoPriorProb_counter = 0.0;

void calcPriorProb(){
    int c1;
    int c2;
    for(int i=0;i<14;i++){
        if(Play[i]==1){
            c1+=1;
        }else{
            c2+=1;
        }
    }

    YesPriorProb = ((float)c1)/((float)(14));
    NoPriorProb = ((float)c2)/((float)(14));

    YesPriorProb_counter = ((float)c1);
    NoPriorProb_counter = ((float)c2);
    
}


void Likelihood(){
    //init
    int OutlookRainy = 0;
    int OutlookOvercast = 0;
    int OutlookSunny = 0;
    int TempHot = 0;
    int TempMild = 0;
    int TempCold = 0;
    int HumHigh = 0;
    int HumNormal = 0;
    int WindyYes = 0;
    int WindyNo = 0;

    int OutlookRainy_NO = 0;
    int OutlookOvercast_NO = 0;
    int OutlookSunny_NO = 0;
    int TempHot_NO = 0;
    int TempMild_NO = 0;
    int TempCold_NO = 0;
    int HumHigh_NO = 0;
    int HumNormal_NO = 0;
    int WindyYes_NO = 0;
    int WindyNo_NO = 0;

    //counter
    for(int i=0;i<14;i++){
        if(Play[i]==1){
            if(Outlook[i]==1){
                OutlookRainy+=1;
            }
            else if(Outlook[i]==2){
                OutlookOvercast+=1;
            }
            else if(Outlook[i]==3){
                OutlookSunny+=1;
            }
            


            if(Temp[i]==1){
                TempHot+=1;
            }
            else if(Temp[i]==2){
                TempMild+=1;
            }
            else if(Temp[i]==3){
                TempCold+=1;
            }



            if(Humidity[i]==1){
                HumHigh+=1;
            }
            else if(Humidity[i]==0){
                HumNormal+=1;
            }



            if(Windy[i]==1){
                WindyYes+=1;
            }
            else if(Windy[i]==0){
                WindyNo+=1;
            }
        }
        else{
            if(Outlook[i]==1){
                OutlookRainy_NO+=1;
            }
            else if(Outlook[i]==2){
                OutlookOvercast_NO+=1;
            }
            else if(Outlook[i]==3){
                OutlookSunny_NO+=1;
            }
            


            if(Temp[i]==1){
                TempHot_NO+=1;
            }
            else if(Temp[i]==2){
                TempMild_NO+=1;
            }
            else if(Temp[i]==3){
                TempCold_NO+=1;
            }



            if(Humidity[i]==1){
                HumHigh_NO+=1;
            }
            else if(Humidity[i]==0){
                HumNormal_NO+=1;
            }



            if(Windy[i]==1){
                WindyYes_NO+=1;
            }
            else if(Windy[i]==0){
                WindyNo_NO+=1;
            }
        }
    }


    //outlook
    OutlookLikelihood[0][0] = ((float)OutlookRainy)/YesPriorProb_counter;
    OutlookLikelihood[1][0] = ((float)OutlookOvercast)/YesPriorProb_counter;
    OutlookLikelihood[2][0] = ((float)OutlookSunny)/YesPriorProb_counter;

    OutlookLikelihood[0][1] = ((float)OutlookRainy_NO)/NoPriorProb_counter;
    OutlookLikelihood[1][1] = ((float)OutlookOvercast_NO)/NoPriorProb_counter;
    OutlookLikelihood[2][1] = ((float)OutlookSunny_NO)/NoPriorProb_counter;

    //temp
    TempLikelihood[0][0] = ((float)TempHot)/YesPriorProb_counter;
    TempLikelihood[1][0] = ((float)TempMild)/YesPriorProb_counter;
    TempLikelihood[2][0] = ((float)TempCold)/YesPriorProb_counter;

    TempLikelihood[0][1] = ((float)TempHot_NO)/NoPriorProb_counter;
    TempLikelihood[1][1] = ((float)TempMild_NO)/NoPriorProb_counter;
    TempLikelihood[2][1] = ((float)TempCold_NO)/NoPriorProb_counter;

    //Humidity
    HumidityLikelihood[0][0] = ((float)HumHigh)/YesPriorProb_counter;
    HumidityLikelihood[1][0] = ((float)HumNormal)/YesPriorProb_counter;

    HumidityLikelihood[0][1] = ((float)HumHigh_NO)/NoPriorProb_counter;
    HumidityLikelihood[1][1] = ((float)HumNormal_NO)/NoPriorProb_counter;

    //windy 
    WindyLikelihood[0][0] = ((float)WindyYes)/YesPriorProb_counter;
    WindyLikelihood[1][0] = ((float)WindyNo)/YesPriorProb_counter;

    WindyLikelihood[0][1] = ((float)WindyYes_NO)/NoPriorProb_counter;
    WindyLikelihood[1][1] = ((float)WindyNo_NO)/NoPriorProb_counter;
}



int main(){

    // for(int i=0;i<3;i++){
    //     printf("%s \n",arr[i]);
    // }
    calcPriorProb();
    Likelihood();

    // Sunny Cold HighHum WindYes
    float Vyes = (YesPriorProb) * (OutlookLikelihood[2][0]) * (TempLikelihood[2][0]) * (HumidityLikelihood[1][0]) * (WindyLikelihood[1][0]);
    float Vno = (NoPriorProb) * (OutlookLikelihood[2][1]) * (TempLikelihood[2][1]) * (HumidityLikelihood[1][1]) * (WindyLikelihood[1][1]);

    float Y = (Vyes)/(Vyes+Vno);
    float N = (Vno)/(Vno+ Vyes);

    if(Y>=N){
        printf("True \n");
    }else{
        printf("False \n");
    }

}