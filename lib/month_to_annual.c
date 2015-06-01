/*This program takes a split-monthly input file (average cfs per period) and outputs to the screen an annual time series (average cfs per year). 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int
main(int argc, char* argv[]){


FILE *obsqfile, *outfile1;
float qcfs,sum;
int i,j, year, month,startyear, endyear, days_per_period[52];
int timeincpyr,days;
char str[200],junk[100];

  if(argc != 5){
printf("Wrong command line arguments: enter <split-month file> <start year> <end year> <periods per year>,\n");
exit (0);
  }

/*open input file and get years to process*/
obsqfile=fopen(argv[1], "r");

sscanf(argv[2],"%d", &startyear);
sscanf(argv[3],"%d", &endyear);
 sscanf(argv[4],"%d", &timeincpyr);

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



 sum=0;
 days=0;

 for(year=startyear;year<=endyear;year++){

   if(year%4==0 && year%400!=400){

if(timeincpyr==14) days_per_period[8]=29;
 else days_per_period[8]=28;
 
if(timeincpyr==12) days_per_period[4]=29;
 else days_per_period[4]=28;

   }

   for(month=0;month<timeincpyr;month++){
    if( fscanf(obsqfile, "%f", &qcfs)==EOF) printf("end of file reached prematurely check beginning and ending dates and number of periods per year are correct\n");;
     sum += qcfs*days_per_period[month];
     days += days_per_period[month];
      

   if(month==(timeincpyr-1)){ 
     printf("%8.0f\n", sum/days);
     sum=0;
     days=0;
    }
   }
 }

 fclose(obsqfile);
 } 
