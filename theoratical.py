import numpy as np 

f = open('output/TIME_MEASUREMENT_dmesg.txt', 'r')
time_per100 = 0
for i in f:
    i = i[:-1].split(' ')
    time_per100 += (float(i[-1]) - float(i[-2]))

time_per100 /= 10 


