# Streamflow bias correction

## Development history: 
- Alan Hamlet and Se-Yeun Lee
- adapted by Matt Stumbaugh June, 2015

## Overview

At the annual and monthly scale, this code bias corrects modeled daily timestep streamflows to observations.

## Directory Structure

- lib/
- bin/
- scripts/
- cfg_projects/
  - cfg_test2.{obs,hist,rcp45,rcp85}.bsh provide time period and water year variables
  - other configuration files define paths

## Getting Started

- Compile lib/*.c source
  - `mkdir bin/; ./compile_all.bsh`   
- Setup configs for time periods
- Setup configs for paths to data, outputs, scripts, and executables
- Emulate `example_cmds.txt` which links dissimilarly named observed and modeled files  
- Create monthly time step observed and simulated streamflows for training period(s)
- Generate (monthly,yearly) quantiles for observations
- Generate (monthly,yearly) quantiles for simulation training period
- Bias correct simulated time series
 
