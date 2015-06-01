/*This program bias corrects a simulated data stream according to a
set of quantile lookup tables for an observed time series and a simulated time series. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int
main(int argc, char* argv[]){


FILE *obsqfile, *simqfile, *infile1, *outfile1;
float obs[52][100][2], sim[52][100][2], rawdata, quantile, value;
float overfrac[2], underfrac[2];
float obsa[52],obsmean[52],obsstd[52], sima[52], simmean[52],simstd[52];
float uobsmean[52],uobsstd[52],usimmean[52],usimstd[52];
float obsalpha[52],obszeta[52],simalpha[52],simzeta[52];
float ev1quantile, ln3anom;
float thresh = 3.5;
float ev1thresh =0.995;
int i,j,k,l, numobs, numsim, count, timeinc, outrangeflag[2];
int dist_opt, timeincpyr;
char str[200],junk[100];

  if(argc != 7){
printf("Wrong command line arguments: enter <qfile.obs> <qfile.sim> <inputfile> <outputfile> <dist_opt> <timeperiodsperyr>,\n");
exit (0);
  }


obsqfile=fopen(argv[1], "r");
simqfile=fopen(argv[2], "r");
infile1=fopen(argv[3], "r");
outfile1=fopen(argv[4], "w");



 sscanf(argv[5],"%d", &dist_opt); 
 sscanf(argv[6],"%d", &timeincpyr);


/*get number of values to read for observed and simulated quantile files*/

fscanf(obsqfile,"%d", &numobs);
fscanf(simqfile,"%d", &numsim);


/*read distribution parameters and observed quantile lookup table from file and load to array*/

 for(i=0;i<timeincpyr;i++){

   fscanf(obsqfile,"%f%f%f%f",&uobsmean[i],&uobsstd[i],&obsmean[i],&obsstd[i]);
   obsalpha[i] = 0.7797 * uobsstd[i];
   obszeta[i] = uobsmean[i] - 0.5772*obsalpha[i];

  for(j=0;j<numobs;j++){
    fscanf(obsqfile,"%f%f", &obs[i][j][0], &obs[i][j][1]);
   }
    }

/*read simulated quantile lookup table from file and load to array*/

 for(i=0;i<timeincpyr;i++){
 
   fscanf(simqfile,"%f%f%f%f",&usimmean[i],&usimstd[i],&simmean[i],&simstd[i]);
   simalpha[i]= 0.7797*usimstd[i];
   simzeta[i]= usimmean[i] - 0.5772*simalpha[i];
   for(j=0;j<numsim;j++){
    fscanf(simqfile,"%f%f", &sim[i][j][0], &sim[i][j][1]);
}
    }

 


/*check simulated quantile array
 for(i=0;i<timeincpyr;i++){
   for(j=0;j<numsim;j++){

   printf("%f %f\n", sim[i][j][0], sim[i][j][1]);
   } 
    }

*/



/*Read raw data and bias correct,  writing result to output file*/

count=0;
 

 while(fscanf(infile1,"%f",&rawdata)!=EOF){ 

/**for input loop for testing
for(k=0;k<55;k++){
fscanf(infile1,"%f",&rawdata);**/

outrangeflag[0]=0;

count++;

timeinc=(count-1)%timeincpyr;


/*printf("rawdata=%f\n", rawdata);*/


/*check to see if flow is higher or lower than any quantile value*/
 if(rawdata == sim[timeinc][0][1]){

   quantile=sim[timeinc][0][0];
   ev1quantile = exp(-1*exp(-1*(rawdata-simzeta[timeinc])/simalpha[timeinc]));
   ln3anom = (log(rawdata)-simmean[timeinc])/simstd[timeinc];
     }

else if(rawdata == sim[timeinc][numsim-1][1]){

   quantile=sim[timeinc][numsim-1][0];
   ev1quantile = exp(-1*exp(-1*(rawdata-simzeta[timeinc])/simalpha[timeinc]));
   ln3anom = (log(rawdata)-simmean[timeinc])/simstd[timeinc];
     }

else if(rawdata > sim[timeinc][0][1]  || rawdata < sim[timeinc][numsim-1][1]) {
 
  ev1quantile = exp(-1*exp(-1*(rawdata-simzeta[timeinc])/simalpha[timeinc]));
  ln3anom = (log(rawdata)-simmean[timeinc])/simstd[timeinc];
  outrangeflag[0]=1;
}

 else{
   ev1quantile = exp(-1*exp(-1*(rawdata-simzeta[timeinc])/simalpha[timeinc]));
   ln3anom = (log(rawdata)-simmean[timeinc])/simstd[timeinc];
 

/*search simulated quantile array for bracketing quantiles using appropriate timeinc*/
 for(i=0;i<numsim;i++){

/*printf("sim position=%d quantile=%f value=%f\n", i, sim[timeinc][i][0], sim[timeinc][i][1]);*/

   if(rawdata > sim[timeinc][i][1]){
/*interpolate*/
quantile= sim[timeinc][i][0] + (sim[timeinc][i-1][0]-sim[timeinc][i][0]) * (rawdata-sim[timeinc][i][1]) / (sim[timeinc][i-1][1]-sim[timeinc][i][1]);

/* exit loop*/
i=numsim;
   }
    } /*end for*/
     } /*end else*/

/*printf("quantile= %f\n", quantile);*/


/*get new value from observed distribution*/

/*check to see if quantile value is out of range of observed lookup table*/
 if(quantile == obs[timeinc][numobs-1][0] && outrangeflag[0]==0){

   value=obs[timeinc][numobs-1][1];
     }

else if(quantile == obs[timeinc][0][0] && outrangeflag[0]==0){

   value=obs[timeinc][0][1];
     }

 else if((quantile > obs[timeinc][numobs-1][0]) || (quantile < obs[timeinc][0][0]) || outrangeflag[0]==1) {

if (dist_opt==0) value = obszeta[timeinc] - obsalpha[timeinc]*log(-1*log(ev1quantile));
if(dist_opt==1)   value=exp(obsstd[timeinc]*ln3anom + obsmean[timeinc]) + obsa[timeinc]; 
 }

 else{

/*get value associated with quantile from obs quantile lookup array*/

 for(i=0;i<numobs;i++){

/*printf("obs position=%d quantile=%f value=%f\n", i, obs[timeinc][i][0], obs[timeinc][i][1]);*/

if(quantile < obs[timeinc][i][0]){

/*interpolate*/

value= obs[timeinc][i][1] + (obs[timeinc][i-1][1]-obs[timeinc][i][1]) * (obs[timeinc][i][0]-quantile) / (obs[timeinc][i][0]-obs[timeinc][i-1][0]);

/*exit loop*/
i=numobs;

}
    } /*end for*/
     }/*end else*/
 
 /* if rawdata is  zero then override interpolation and just print out zero to file*/   
 
if(rawdata > -0.001 && rawdata < 0.001) value=0;



/* if rawdata is sufficiently out of range as defined by preset threshold value, use simple scaling technique */


 else if(dist_opt ==0 && (ev1quantile < (1.0-ev1thresh) || ev1quantile > ev1thresh)) value=rawdata/usimmean[timeinc]*uobsmean[timeinc]; 

 else if(dist_opt == 1 && (ln3anom < (-1*thresh) || ln3anom > thresh)) value=rawdata/usimmean[timeinc]*uobsmean[timeinc];



 
/* print diagnostic data to the screen and bias corrected value to output file*/

/*printf("%d %f %f %f %f %f %d \n", timeinc, rawdata, quantile, ln3anom, ev1quantile, value, outrangeflag[0]);*/ 

fprintf(outfile1,"%f\n", value);


 }

fclose(infile1);
fclose(outfile1);

} 
