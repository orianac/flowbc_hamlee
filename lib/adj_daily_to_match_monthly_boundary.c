/*This program rescales daily timestep VIC streamflows so that they match 
a monthly time series of observed naturalized values.  Note start year is
water year beginning Oct 1*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[])
{
  FILE *rawfile, *rawccfile,  *bcdfile, *outfile;
  float tmp, sum1,sum2,sum3,sum4,data, bcmdata[35];
  int i,j,k,n,day,mnth,year,nday,startwy, endwy, unitsw, DayYear;
  int dpm[12] = {31,30,31,31,28,31,30,31,30,31,31,30};
  int tdeom[12] = {30,60, 91, 122, 150, 181, 211, 242, 272, 303, 334, 364};
  int tdeoml[12] = {30,60, 91, 122, 151, 182, 212, 243, 273, 304, 335, 365};
  float temp[4] = {0,0,0,0};
  int leap=0;
  if(argc !=5){
    printf("Wrong command line arguments: enter <bcdfile> <outfile> <startyear> <endyear> ,\n");
    exit (0);
  }
  /*open files*/
  bcdfile=fopen(argv[1], "r");
  outfile=fopen(argv[2], "w");
  sscanf(argv[3],"%d", &startwy);
  sscanf(argv[4],"%d", &endwy);

  /*initialize sum variables*/
  n=0;
  i=0;
  year=startwy;
  /* NEW STORAGE*/
  float OneYear[366];
  int EndJuly;
  while(n<DayYear ||year<=endwy)
    {
      // Checking Leap year 
      if ((year %4 == 0) && (((year % 100 == 0) && (year % 400 != 0)) == 0))
	{
	  dpm[4] = 29;	 
	  tdeom[4] = 152;
	  DayYear = 365;
	  leap = 1;
	}
      else 
	{
	  DayYear = 364;
          leap = 0;
	}
   
      /* Reading DATA from 'outfile' (NEED TO CHANGE the part of OPEN FILE)*/ 
      fscanf(bcdfile, "%f", &tmp);
      OneYear[n] = tmp;
   
      /* 305th array (306th day) = August 2*/
      if(leap==1)
	{
	  if(n==tdeoml[i]+1)
	    {
              float temp = OneYear[tdeoml[i]]; 
	      OneYear[tdeoml[i]] = (3*OneYear[tdeoml[i]] + OneYear[tdeoml[i]+1])/4;
	      OneYear[tdeoml[i]+1] = (temp + 3*OneYear[tdeoml[i]+1])/4;
	      i++;
	    }
	}
      else
	{
	  if(n==tdeom[i]+1) 
	    { 
              float temp = OneYear[tdeom[i]]; 
	      OneYear[tdeom[i]] = (3*OneYear[tdeom[i]] + OneYear[tdeom[i]+1])/4;
	      OneYear[tdeom[i]+1] = (temp + 3*OneYear[tdeom[i]+1])/4; 
	      i++; 
	    } 
	}

      if(n==DayYear)
	{
	  year++; 
	  n=-1;
	  i=0;
	  for( j=0; j<=DayYear; j++)
	    {
	      fprintf(outfile, "%10.0f\n", OneYear[j]);
	    }
	}
      n++;
    }

} 

