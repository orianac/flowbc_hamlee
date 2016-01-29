import pandas as pd
import xarray as xray
import os
import sys
from subprocess import call

# Step 1:
# Create the model quantiles for every NRNI site
# Only needed to run this one time to convert the NRNI netcdf file to text monthly mean values.

nrni_file = '/home/oriana/bpa/nrni/NRNI_20150318_clean_fixed_MRS-prepped.nc'
monthly_out_dir_obs = '/raid3/oriana/bpa/nrni/monthly/'

nrni = xray.open_dataset(nrni_file)
nrni.Index.values = nrni.IndexNames.values
for i in nrni['Index'].values:
    flow_df = nrni.sel(Index=i).to_dataframe()
    flow_df['Streamflow'].resample('M', how='mean').to_csv(\
    monthly_out_dir_obs+'monthly_mean_nrni_'+str(i)+'.txt', sep=' ', date_format='%Y-%m')

ncfile = '/raid3/oriana/bpa/future/vic/CNRM-CM5/rcp85/output/route/hist/all_sites_historical.rvic.h0a.2100-01-01.nc'
routed_text_dir = '/raid3/oriana/bpa/future/vic/CNRM-CM5/rcp85/output/route/text/'
working_dir = '/raid3/oriana/bpa/future/vic/CNRM-CM5/rcp85/output/route/bias_correction/'


os.makedirs(routed_text_dir, exist_ok=True)
os.makedirs(working_dir, exist_ok=True)

# Makes each of the different mean monthly streamflow text files for inputs
# Read in the routed ncfile
ds = xray.open_dataset(ncfile)
ds.outlets.values = [outlet.decode() for outlet in ds.outlet_name.values]
df = ds.streamflow.to_dataframe()
streamflow = df.unstack().streamflow.ix[1:]
streamflow.index = pd.date_range(start='1950-01-01', end='2099-12-31')
# Convert to CFS to match the NRNI units
streamflow = streamflow*((100*100*100)/(2.54*2.54*2.54*12*12*12))
# Convert the routed daily streamflow to mean monthly values
monmean = streamflow.resample('M', how='mean')

# Write out the mean monthly values to text file for reading by the bias correction scripts

for site in monmean.keys():
    monmean[site].to_csv(routed_text_dir+'monthly_mean_'+str(site)+'.txt', sep=' ', date_format='%Y-%m')

# Write out the daily values to text file for reading by the bias correction scripts

for site in streamflow.keys():
    streamflow[site].to_csv(routed_text_dir+'historical_daily_'+str(site)+'.txt', sep=' ', date_format='%Y-%m-%d')
    streamflow[site].to_csv(routed_text_dir+'future_daily_'+str(site)+'.txt', sep=' ', date_format='%Y-%m-%d')

# Step 2:
# Create the model historical quantiles

control_input_script = '/home/oriana/scripts/flowbc_hamlee/scripts/control_prep_inputs.bsh'
control_bias_correct_script = '/home/oriana/scripts/flowbc_hamlee/scripts/control_bias_correct.bsh'

site_id = 'YUMW'

monthly_mean_model_file = 'monthly_mean_'+site_id+'.txt'
daily_model_file = 'daily_'+site_id+'.txt'

monthly_mean_obs_file = 'monthly_mean_nrni_'+site_id+'.txt'

## To-do: need to make this file using a template; right now I made it manually
path_file = working_dir+'historical_paths.cfg'

historical_config_file = working_dir+'historical_text_row_numbers.cfg'
future_config_file = working_dir+'future_text_row_numbers.cfg'

historical_dir = routed_text_dir

out_historical_daily_bc_file_stub = 'historical_daily'
out_future_daily_bc_file_stub = 'future_daily'
bc_log_file = working_dir+'bc_log_file.txt'

with open(bc_log_file, 'w') as logfile:
    call([control_input_script, monthly_mean_model_file, path_file, historical_config_file, historical_dir],
     stdout=logfile, shell=False)

# Step 3:
# Bias correct the historical time series

print(str([contl_bias_correct_script, monthly_mean_obs_file, monthly_mean_model_file, out_daily_bc_file_stub,
          site_id, path_file, historical_config_file]))

with open(bc_log_file, 'a') as logfile:
    call([control_bias_correct_script, monthly_mean_obs_file, monthly_mean_model_file, out_daily_bc_file_stub,
          site_id, path_file, historical_config_file],
     stdout=logfile, shell=False)

# Step 4:
# Bias correct the future time series



with open(bc_log_file, 'a') as logfile:
    call([control_bias_correct_script, monthly_mean_obs_file, monthly_mean_model_file, 
          out_future_daily_bc_file_stub, site_id, path_file, future_config_file],
     stdout=logfile, shell=False)
