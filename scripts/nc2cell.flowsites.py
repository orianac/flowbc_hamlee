#!/usr/bin/env python

import os 
import os.path
import sys
import numpy as np
import pandas as pd
from netCDF4 import Dataset
from netCDF4 import num2date, date2num

def main(args):
    
    ncfilein = args[0]
    dimens   = args[1].split(',') #"time,x"        "Time,Index"
    selvars   = args[2].split(',')#"streamflow"    "Streamflow"
    seldim   = args[3].split(',') #"Index,5,8,20"
    timefmt  = args[4] # "%Y-%m-%d"
    print args
    kwargs={'dimens':dimens,
            'selvars':selvars,
            'seldim':seldim,
            'timefmt':timefmt}
    readnc(ncfilein,**kwargs)

def readnc(ncfilein,**kwargs):
    '''Read nc file'''

    rgrp = Dataset(ncfilein,'r', format='NETCDF4_CLASSIC')

    times=rgrp.variables[kwargs['dimens'][0]]
    ntimes=len(times)

    dates = num2date(rgrp.variables[kwargs['dimens'][0]][:], units=times.units, calendar=times.calendar)   
    
    dummy = np.zeros((ntimes,1))    
    df = pd.DataFrame(dummy,index=[date.strftime(kwargs['timefmt']) for date in dates])
    dfs={}

    if kwargs['seldim']:
        seldimname=kwargs['seldim'][0]
        selinds=kwargs['seldim'][1:]
        if len(selinds) == 0 or not selinds[0]:
            # get all indices along dim
            selinds=range(0,len(rgrp.dimensions[seldimname]))
    for idim in selinds:        
        for var in kwargs['selvars']:
            df.ix[:,0] = rgrp.variables[var][:,idim]

        ## write out csv file
        print idim
        
        df.to_csv(ncfilein+".%s.txt"%idim, sep=" ", float_format="%.3f", header=False) #, index=False)
    
if __name__=='__main__':
    print sys.argv
    if len(sys.argv)==6:
        print sys.argv[1:]
        main(sys.argv[1:])
    else:
        print 'Need args <input netcdf file> <coord-vars> <sel-vars> <sel-inds> <timefmt>'
        print 'Suggested time formats: Daily <"%Y-%m-%d">, Monthly <"%Y-%m">'

        print 'Usage hydra { /raid3/stumbaugh/IS/CONUS/routed }->'
        print 'python devel/scripts/nc2cell.flowsites.py vic_RoutedRunoff_NorESM1-M_r1i1p1_historical_1950_2005_WUSA_daily.nc "time,x" "streamflow" "x,15,10" "%Y-%m-%d"'
        print "qcmd -N nrnidaily2cell2 $DEFQ", 'python devel/scripts/nc2cell.flowsites.py data2-NRNI.station.asc/NRNI_20150318_clean_fixed_MRS-prepped.recTime.nc "Time,Index" "Streamflow" "Index," "%Y-%m-%d"'
        print 'qcmd -N nrnidaily2cell3 $DEFQ', 'python devel/scripts/nc2cell.flowsites.py data2-NRNI.station.asc/NRNI_20150318_clean_fixed_MRS-prepped.recTime.monsum.nc "Time,Index" "Streamflow" "Index," "%Y-%m"'