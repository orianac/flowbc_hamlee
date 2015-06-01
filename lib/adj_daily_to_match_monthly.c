/*This program rescales daily timestep VIC streamflows so that they match 
a monthly time series of observed naturalized values.  Note start year is
water year beginning Oct 1*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char* argv[]){


FILE *rawfile, *rawccfile, *bcyfile, *bcmfile, *outfile;
 float tmp, sum1,sum2,sum3,sum4,data, bcmdata[35];
 float adjfrac;
 int i,j,k,n,day,mnth,year,nday,startwy, endwy, unitsw;
 int dpm[12] = {31,30,31,31,28,31,30,31,30,31,31,30};

  if(argc !=7){
printf("Wrong command line arguments: enter <bcmfile> <bcyfile> <outfile> <startyear> <endyear> <unit sw> ,\n");
exit (0);
  }

/*open files*/
bcmfile=fopen(argv[1], "r");
bcyfile=fopen(argv[2], "r");
outfile=fopen(argv[3], "w");

sscanf(argv[4],"%d", &startwy);
sscanf(argv[5],"%d", &endwy);
sscanf(argv[6],"%d", &unitsw); // 0 means monthly file in cfs 1 means monthly file in acre feet

/*initialize sum variables*/

 nday=0;
 sum1=0;
 sum2=0;
 sum3=0;
 sum4=0;

/* loop over all water years in file */

 for(year=startwy;year<=endwy;year++){

   if ((year %4 == 0) && (((year % 100 == 0) && (year % 400 != 0)) == 0)){
     dpm[4] = 29;
   }
   else {
     dpm[4] = 28;
   }

   for(mnth=0;mnth<12;mnth++){

     fscanf(bcyfile,"%f",&sum1);

     for(day=1;day<=dpm[mnth];day++){
   n++;


   if(fscanf(bcmfile, "%f", &tmp) != 1) {printf("Bad data or EOF encountered prematurely.\n"); exit(0);}
   sum2 += tmp;
   bcmdata[(day-1)] =  tmp;

/* after loading last day of each mnth, write out the data and reinitialize the sum variable */

 if(day==dpm[mnth]) {

     if(sum1==0 || sum2==0) adjfrac=0;

else {

  if(unitsw == 0) adjfrac = sum1/(sum2/dpm[mnth]);
  if(unitsw == 1) adjfrac = (sum1*0.50417)/sum2; 

}
   for(j=0;j<dpm[mnth]; j++) {
fprintf(outfile, "%10.0f\n", adjfrac*bcmdata[j]);

   }

   sum2 = 0;

   }


/*reinitialize sum variables and increment year*/



     }}}


} 
