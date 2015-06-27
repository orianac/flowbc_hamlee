/******************************************************
This program computes the mean, stdev, and fits parameters for several theoretical probability
distributions.
******************************************************/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]) {

  int i,j,k,m;
  int clength, numsamp, dist_option, dist_param, neg_flag;
  FILE *infile;

  float zvalue[4]={1.64, 2.05, 2.32, 2.57};
  float a,ad,data[100], sort[100], sum[4], out[6], max, med, min, getdata;

  void shell(int n, float a[]);

  if(argc !=3) {printf("Wrong number of command line arguments.  Check code.\n"); exit(0);}



 /* load command line arguments to parameter values */

 infile= fopen(argv[1],"r"); 

 sscanf(argv[2],"%d", &dist_option);

 i=0;
 clength=0;


/* find data file length and check for negative numbers*/

 while ((fscanf(infile,"%f", &getdata)==1))  {
  clength++;
  if( getdata < 0) neg_flag = 1;

 }
 

/* report inconsistency between fitting options and data*/

	if(neg_flag==1 && dist_option == 1) printf("Warning:  data contains negative values that are incompatible with fitting of log normal distribution.\n Use option 0 (EV1) or 1 (ln3) setting instead.\n");


rewind(infile);

 sum[0]=0;
 sum[1]=0;
 sum[2]=0;
 sum[3]=0;
 
 max=0;
 min=1e9;

  for(j=0;j<clength;j++){

    fscanf(infile,"%f", &getdata); 
    data[j]=getdata;
    sort[j+1]=getdata;            /*note sort array loaded from 1 to n */
    if(getdata<=min) min=getdata;
    if(getdata>=max) max=getdata;
    
   }

  /* sort values in data array from lowest to highest using shell sort method*/

  shell(clength, sort); 

  /* check sorting */

  /*  for(m=1;m<=clength;m++) printf("%f\n", sort[m]);*/

  /* store median value */
  

if(clength%2==0)  med=(sort[clength/2] + sort[clength/2+1])/2;
 else med=sort[clength/2 +1];

/* note reverse order of sorting routine and array numbering from 1 to n in the context of Stedinger's selection point*/

  /*  printf("med = %f max= %f min= %f\n", med,max,min);*/

  /* calculate offset value  "a"  from sample using Stedinger's scheme */

  ad=(min + max - 2*med);

  if(ad==0.0) a=0;

	else if(dist_option == 2) a= (min*max - med*med)/(min + max - 2*med);

	else a=0;

  

/* if a exceeds min data point, ignore this section of data */
 

  if(a > min) a = 0;


/* calculate mean and variance of transformed data */

for(k=0;k<clength;k++){


       sum[0] += log(data[k]-a);
     sum[1] += log(data[k]-a)*log(data[k]-a);
       sum[2] += data[k];
     sum[3] += data[k]*data[k];

}


   out[0]=sum[0]/clength;
   out[1]=sqrt( (sum[1] - sum[0]*sum[0]/clength)/(clength-1));
	out[2]=sum[2]/clength;
	out[3]=sqrt( (sum[3] - sum[2]*sum[2]/clength)/(clength-1));


	out[4]= exp(out[0] + out[1]*zvalue[0]) + a;
	out[5]= exp(out[0] + out[1]*zvalue[1]) + a;
	out[6]= exp(out[0] + out[1]*zvalue[2]) + a;
	out[7]= exp(out[0] + out[1]*zvalue[3]) + a; 


   
/*   printf("%8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f\n", out[0],out[1],a,out[2],out[3],out[4],out[5]);  */

if(dist_option==0)	printf("%8.5f\n%8.7f\n%8.7f\n%8.7f\n", out[2],out[3],out[2],out[3]);

if(dist_option==1 || dist_option==2) printf("%8.5f\n%8.7f\n%8.7f\n%8.7f\n", out[2],out[3],out[0],out[1]); 


  
} /** end of main **/



/* sorting routine from Numerical Receipes pg 332 */

void shell (int n, float a[]) {

int i,j, inc;
float v;

inc = 1;

do{

inc *= 3;
inc++;
} while(inc <= n);

do{

inc /= 3;

for(i=inc+1;i<=n;i++) {
v=a[i];
j=i;

while(a[j-inc] > v) {

a[j]=a[j-inc];
j -= inc;
if (j <= inc) break;

}
a[j] = v;
}
} while(inc >1);
}

