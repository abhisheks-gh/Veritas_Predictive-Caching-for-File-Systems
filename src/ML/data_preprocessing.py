
#----------Modules------------------------------------------------------------------------------------------------------------------
import pandas as pd
import numpy as np 
import time 
import tkinter as tk

#----------Data Preprocessing ---------------------------------------------------------------------------------------------------------------

init_time = time.time()
print(" --- Use input values as specified in [ ... ] ---\n")
columns = ['w/r','file_path','offset','offset2','date_time']
file = pd.read_csv('output.csv', names = columns , skipinitialspace=True) # found initial spaces while reading data
file = file.drop(columns=['offset','offset2'])
file['day'] = [str(x).split(' ')[0] for x in file['date_time']]
file['time'] = [str(x).split(' ')[3].split(':')[0] for x in file['date_time']]
file.drop(columns=['date_time'] , inplace=True)
file.to_csv( './/new_output.csv' , index= False)
