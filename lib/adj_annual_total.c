/*This program adjusts the annual mean of bias corrected monthly streamflows so that it matches a bias corrected annual mean time series*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char* argv[]){


FILE *rawfile, *rawccfile, *bcyfile, *bcmfile, *outfile;
 float sum1,sum2,sum3,sum4,data, bcmdata[52];
 float adjfrac;
 int i,j,k,year,nday, ppy;
 int dpm[12] = {31,30,31,31,28,31,30,31,30,31,31,30};
 int dpsm[14] = {31,15,16,30,31,30,31,31,28,31,15,15,31,30};
 int  dpp[52], unitsw;

  if(argc !=7){
printf("Wrong command line arguments: enter <bcmfile> <bcyfile> <outfile> <startyear> <ppy> <unit switch>,\n");
exit (0);
  }

/*open files*/
bcmfile=fopen(argv[1], "r");
if(bcmfile == NULL) printf("cannot open input file %s\n", argv[1]);

bcyfile=fopen(argv[2], "r");
if(bcyfile == NULL) printf("cannot open input file %s\n", argv[2]);
outfile=fopen(argv[3], "w");
if(outfile == NULL) printf("cannot open output file %s\n", argv[3]);


sscanf(argv[4],"%d", &year);
sscanf(argv[5],"%d", &ppy);
sscanf(argv[6],"%d", &unitsw);

/*initialize  variables*/

 if(ppy==52){
 for(i=0;i<52;i++) dpp[i]=7;
 dpp[51]=8;
 }

 if(ppy==14){ 
   for(i=0;i<14;i++) dpp[i]=dpsm[i]; 
 }

 if(ppy==12){ 
  for(i=0;i<12;i++) dpp[i]=dpm[i]; 
 }

 nday=0;
 sum1=0;
 sum2=0;
 sum3=0;
 sum4=0;

/*read in one annual bcy value and then adjust one water year from the bcm file*/

 while(fscanf(bcyfile,"%f",&sum1)==1){

   if ((year %4 == 0) && (((year % 100 == 0) && (year % 400 != 0)) == 0)){
 if(ppy==12)    dpp[4] = 29;
 if(ppy==14)    dpp[8] = 29;
 if(ppy==52)    dpp[51] = 9;
   }
   else {
 if(ppy==12)    dpp[4] = 28;
 if(ppy==14)    dpp[8] = 28;
 if(ppy==52)    dpp[51] = 8;
   }


     for(i=0;i<ppy;i++){
       fscanf(bcmfile,"%f",&bcmdata[i]);

       if(unitsw == 0){
       sum2 += bcmdata[i];
       nday += dpp[i];
       }


       if(unitsw == 1){
       sum2 += dpp[i]*bcmdata[i];
       nday += dpp[i];
       }


    }


     if(sum1==0 || sum2==0) adjfrac=0;

     else {   

if(unitsw == 0)  adjfrac = (sum1)/sum2; 
if(unitsw == 1)  adjfrac = (sum1*nday)/sum2;

     }

   for(j=0;j<ppy;j++) {
fprintf(outfile, "%10.0f\n", adjfrac*bcmdata[j]);
   
 }

/*reinitialize sum variables and increment year*/


 nday=0; 
 year++;
 sum1=0;
 sum2=0;
 sum3=0;
 sum4=0;

 }


} 
