/*

This program finds maximum annual day flow
Based on Wateryear i.e. starting month is October
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 

main( int argc, char* argv[])
{

  int    i,j,k,m,n, numsteps, numdata, numcells, numtoread, skip;
  int    flag, datatype; 
  float  Max, tmp;
  FILE  *filelist, *inputfile, *outputfile, *workfile, *outputnr;
  int    year, nmth,startyear, endyear, yrday;

  if(argc != 7){

printf("Wrong command line arguments: enter <infile> <outfile> <startyear> <endyear> <numcells > .\n");
exit (0);
  }

/*open input and output files*/

  inputfile=fopen(argv[1], "r");
  outputfile=fopen(argv[2], "w");
  outputnr=fopen(argv[6], "w"); 

/*read argument strings to integer values*/

sscanf(argv[3],"%d", &startyear);
sscanf(argv[4],"%d", &endyear);
sscanf(argv[5],"%d", &numcells);

/* Check some values */

 if(startyear > endyear) {printf("Startyear and endyear improperly  defined.\n"); exit(0);}

/* Initialize variables */

 n=0;
 m=0;
 Max =0;
 /* for(i=0;i<numcells;i++) Max[i] = 0;*/

/* loop over all data in file */

 for(year=startyear;year<=endyear;year++){

     for(nmth=1;nmth<=12;nmth++){
      n++;

     for(i=0;i<numcells;i++){        

     fscanf(inputfile, "%f", &tmp);
     if(Max < tmp) Max = tmp;   
     }}
    
/* after loading last day of each month, write out the data and reinitialize the sum variable */
     fprintf(outputfile," %5.4f ",Max); 
     /*     printf("# of year = %d. Max = %f \n ",yrday, Max);*/ 
     fprintf(outputfile,"\n");  
      Max = 0;  
  m++;
 }
       
printf("Done.\nNumber of daily timesteps processed = %d.\n", n);
fprintf(outputnr,"%d.", m);
 


}
