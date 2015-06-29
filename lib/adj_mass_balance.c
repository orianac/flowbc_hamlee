/*This program adjusts the annual mass balance moving upstream from the most downstream location in the 
basin and adjusting monthly streamflows*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(int argc, char* argv[]){


FILE *configfile, *rawdownfile, *rawupfile[5], *bcdownfile, *bcupfile[5], *outfile[5];
 float sum[10],tempdata, testdata, data[5][52];
 float adjfrac;
 int i,j,k,numstat;
 char outdirstr[100], indirstr[100];
 char name[10][100], outdir[100], temp[200];
 int dpsm[14] = {31,15,16,30,31,30,31,31,28,31,15,15,31,30};
 int dpm[12] = {31,30,31,31,28,31,30,31,30,31,31,30};
 int dpp[52], ppy, unitsw;
 int year;

  if(argc !=7){
printf("Wrong command line arguments: enter <config_file> <out_dir_path> <start year> <ppy> <unit switch> <file str> ,\n");
exit (0);
  }

  strcpy(outdirstr, argv[2]);
  strcpy(indirstr,"./in/");
  sscanf(argv[3],"%d", &year);
  sscanf(argv[4],"%d", &ppy); 
  sscanf(argv[5],"%d", &unitsw);
  
/*open input file*/

  configfile = fopen(argv[1], "r");

/*initialize  dpp variables*/ 
 
  if(ppy == 52){ 
    for(i=0;i<52;i++) dpp[i]=7; 
    dpp[51]=8; 
  } 

  if(ppy==14){ 
  for(i=0;i<14;i++) dpp[i]=dpsm[i]; 
  }

  if(ppy==12){ 
  for(i=0;i<12;i++) dpp[i]=dpm[i]; 
  }

/*main loop*/

 while( fscanf(configfile,"%d",&numstat) != EOF){ 

    for(i=0;i<numstat; i++){
      fscanf(configfile,"%s", name[i]);
       printf("%s\n", name[i]);
    }



/*create bc.ff and bc.f input file name strings and open files*/

    for(i=0;i<numstat;i++){

      strcpy(temp,outdirstr);
      strcat(temp,name[i]);
      strcat(temp,argv[6]);
if(i==0) strcat(temp,"bc.ff");
 if(i > 0) strcat(temp,"bc.f");
      printf("Opening bias-corrected file %s for input. \n", temp);

if(i==0) { 
 bcdownfile = fopen(temp, "r");
  if(bcdownfile==NULL) { printf("ERROR opening file named %s\n", temp); exit(0);}
}
 else {
 bcupfile[i-1] = fopen(temp, "r");
 if(bcupfile[i-1]==NULL) { printf("ERROR opening file named %s\n", temp); exit(0);}
 }
    }

/*create bc.ff output file name strings and open files*/

    for(i=0;i < (numstat-1);i++){

      strcpy(temp,outdirstr);
      strcat(temp,name[i+1]);
      strcat(temp,argv[6]);
      strcat(temp,"bc.ff");
      printf("Opening output file %s for writing. \n", temp);

outfile[i] = fopen(temp, "w");
 if(outfile[i]==NULL) { printf("ERROR opening file named %s\n", temp); exit(0);} 

    }

// read in one year of data at a time and correct the mass balance

//initialize sum variables

 sum[0]=0;
 sum[1]=0;
 sum[2]=0;
 sum[3]=0;

//read in one year of data from each file and then adjust water year for the upstream files

 while(fscanf(bcdownfile,"%f",&sum[0])==1){   //loop over available data

if(unitsw == 1)   sum[0] *= dpp[0];

//finish loading bc.ff and bc.f data

  
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


   for(i=1;i<ppy;i++) {

     fscanf(bcdownfile,"%f",&tempdata);
if(unitsw == 0) sum[0] += tempdata; 
if(unitsw == 1) sum[0] += tempdata*dpp[i];
   }



   for(i=0;i < (numstat-1);i++){ 
     for(j=0;j<ppy;j++){

      if(fscanf(bcupfile[i],"%f",&tempdata) != 1) printf("error reading upfile\n");

data[i][j] = tempdata;
              
if(unitsw == 0) sum[1] += tempdata; 
if(unitsw == 1) sum[1] += tempdata*dpp[j];
/* printf("tempdata is %f   \n",  tempdata);
 printf("days of month is %d   \n",  dpp[j]);
 printf("sum[1] is  %f  \n",  sum[1]);*/
     }
   }


if(sum[0]==0 || sum[1]==0) adjfrac=0;
 else   adjfrac = (sum[0]/sum[1]);
/* printf("adjfrac is  %f  \n", adjfrac);
   printf("sum[0] is %f   \n",  sum[0]); */

  for(i=0;i < (numstat-1);i++){
   for(j=0;j<ppy;j++) {

fprintf(outfile[i], "%10.0f\n", adjfrac*data[i][j]); 
/* printf("data[%d][%d] is %f   \n", i,j, data[i][j]);
   printf("outfile is  %10.0f  \n", adjfrac*data[i][j]);*/
  }   
   }


/*reinitialize sum variables*/
 year++;
 sum[0]=0;
 sum[1]=0;
 sum[2]=0;
 sum[3]=0;


   } //end of secondary while loop

//close all files

 fclose(bcdownfile);

 for(i=0;i < (numstat-1);i++){

    fclose(bcupfile[i]);
    fclose(outfile[i]);
  }

  } //end of main while loop

} //end of main
