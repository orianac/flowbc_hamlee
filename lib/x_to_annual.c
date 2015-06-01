/*This program takes a weekly input file  and outputs to the screen an annual time series.
If the uswitch var is set to 0 the units are reported as acre-ft per period.
If the uswitch var is set to 1 the units are reported as cfs averaged over the period. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int
main(int argc, char* argv[]){


FILE *obsqfile, *outfile1;
float qcfs,sum;
int i,j, year, month, tinc, startyear, endyear, days_per_period[52];
int timeincpyr,days, uswitch;
char str[200],junk[100];

  if(argc != 6){
printf("Wrong command line arguments: enter <x time step file> <start year> <end year> <periods per year> <unit switch>,\n");
exit (0);
  }

/*open input file and get years to process*/
obsqfile=fopen(argv[1], "r");

sscanf(argv[2],"%d", &startyear);
sscanf(argv[3],"%d", &endyear);
sscanf(argv[4],"%d", &timeincpyr); 
sscanf(argv[5],"%d", &uswitch);

/* set days per period for July-June split month time series or 12 month water year Oct-Sept*/

if(timeincpyr==14){
days_per_period[0]=31;
days_per_period[1]=15;
days_per_period[2]=16;
days_per_period[3]=30;
days_per_period[4]=31;
days_per_period[5]=30;
days_per_period[6]=31;
days_per_period[7]=31;
days_per_period[8]=28;
days_per_period[9]=31;
days_per_period[10]=15;
days_per_period[11]=15;
days_per_period[12]=31;
days_per_period[13]=30;
  }

  if(timeincpyr==12){
days_per_period[0]=31;
days_per_period[1]=30;
days_per_period[2]=31;
days_per_period[3]=31;
days_per_period[4]=28;
days_per_period[5]=31;
days_per_period[6]=30;
days_per_period[7]=31;
days_per_period[8]=30;
days_per_period[9]=31;
days_per_period[10]=31;
days_per_period[11]=30;
  }
if(timeincpyr==52){

    for(i=0;i<52;i++) days_per_period[i]=7;
    days_per_period[51] = 8;

  }


 sum=0;
 days=0;

for(year=startyear;year<=endyear;year++){


   if ((year %4 == 0) && (((year % 100 == 0) && (year % 400 != 0)) == 0)){
     if(timeincpyr==12) days_per_period[4]=29;
     if(timeincpyr==14) days_per_period[8]=29;
     if(timeincpyr==52) days_per_period[51]=9; 

   }

   else {

     if(timeincpyr==12) days_per_period[4]=28;
     if(timeincpyr==14) days_per_period[8]=28;
     if(timeincpyr==52) days_per_period[51]=8; 

   }


   for(tinc=0;tinc<timeincpyr;tinc++){
    if( fscanf(obsqfile, "%f", &qcfs)==EOF) printf("end of file reached prematurely check beginning and ending dates and number of periods per year are correct\n");;

    if(uswitch == 0){
     sum += qcfs;
     days += days_per_period[tinc];
    }      

    if(uswitch == 1){
     sum += qcfs*days_per_period[tinc];
     days += days_per_period[tinc];
    }      

   if(tinc==(timeincpyr-1)){ 
if(uswitch == 1) printf("%8.2f\n", sum/days);
 if(uswitch == 0) printf("%8.2f\n", sum); 
     sum=0;
     days=0;
    }
   }
 }

 fclose(obsqfile);
 } 
