import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import pylab

import scipy.fftpack
from scipy.signal import butter, filtfilt, medfilt
from scipy.ndimage import uniform_filter1d
from scipy import signal
from scipy.signal import find_peaks
from scipy import interpolate

S_fs = 100
P_fs = 100
T_fs = 10
I_fs = 20
HEIGHT = 165

def get_timePoints(data, time_stamps):
    b = time_stamps[0]
    o = time_stamps[1]
    r = time_stamps[2]

    base_point = data[(data>b-100)&(data<b+100)].index[0] 
    occlusion_point = data[(data>o-100)&(data<o+100)].index[0] 
    release_point = data[(data>r-100)&(data<r+100)].index[0] 

    time_points = [base_point, occlusion_point, release_point]
    return time_points

def plot_raw(signal, time_stamps, name, fs):
    t = np.array(range(len(signal)))/fs
    plt.plot(t,signal)
    plt.axvline(x=time_stamps[0], color='r')
    plt.axvline(x=time_stamps[1], color='r')
    plt.axvline(x=time_stamps[2], color='r')
    plt.title(name)
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude')
    plt.grid()
    plt.show()

def remove_nan(data):
    mask = np.isnan(data)
    data[mask] = np.interp(np.flatnonzero(mask), np.flatnonzero(~mask), data[~mask])
    return data
    
def get_lowest(data):
    min = np.where(data == data.min())[0] 
    try:
        min = min[0]
        return min
    except:
        return min

def calculate_slope(data, start, stop):
    selected = data[start:stop]
    time = np.array(range(start, stop))/T_fs
    nCs = len(selected)
    slope = (nCs*np.sum(selected*time)-(np.sum(selected)*sum(time)))/(nCs*np.sum(time*time)-(np.sum(time))**2)
    return slope

def plot_grad(data, t_lowest, r_slope, f_slope):
    plt.plot(np.array(range(len(data)))/T_fs,data)
    plt.axline((int(t_lowest/T_fs),data[t_lowest]), slope=r_slope, linewidth=2, color='r')
    plt.axline((int(t_lowest/T_fs),data[t_lowest]), slope=f_slope, linewidth=2, color='g')
    plt.title('Gradient Calculation')
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude')
    plt.show()

def get_AUC(data, start, end):
    sig = data[start:end]
    sig = sig - min(sig)

    plt.fill_between(np.array(range(start,end))/T_fs, sig, color="skyblue", alpha=0.5)
    plt.title('Area Under the Curve')
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude')
    plt.show()

    AUC = np.trapz(sig, dx=1/T_fs)
    return AUC

def long_term_indices(data, rising_seg_len, falling_seg_len):

    tmp_lowest = get_lowest(data)
    tmp_r_grad = calculate_slope(data,tmp_lowest,tmp_lowest+int(rising_seg_len*T_fs))
    tmp_f_grad = calculate_slope(data,tmp_lowest-int(falling_seg_len*T_fs),tmp_lowest)
    tmp_rise_fall = tmp_r_grad/tmp_f_grad

    plot_grad(data, tmp_lowest, tmp_r_grad, tmp_f_grad)

    return [tmp_r_grad, tmp_f_grad, tmp_rise_fall]

####################
##### Datasets #####
####################

# Reading 1 
dataM1 = pd.read_csv('Data/m1.csv')
time_stampsM1 = [1623568142869, 1623568443999, 1623568623752]

# Reading 2
dataA1 = pd.read_csv('Data/a1.csv')
time_stampsA1 = [1623587249559, 1623587553231, 1623587733163]

# Reading 3 
dataA2 = pd.read_csv('Data/a2.csv')
time_stampsA2 = [1623571666140, 1623571966485, 1623572147235]

# Reading 4
dataA3 = pd.read_csv('Data/age52June242021.csv')
time_stampsA3 = [1624509304574, 1624509618657, 1624509783431]

# Reading 5
dataT1 = pd.read_csv('Data/age55June242021.csv')
time_stampsT1 = [1624510578929, 1624510877105, 1624511059168]

# Reading 6
dataM2 = pd.read_csv('Data/age23June262021.csv')
time_stampsM2 = [1624723469219, 1624723768744, 1624723768923]

# Reading 7
dataA4 = pd.read_csv('Data/age52June272021.csv')
time_stampsA4 = [1624803269155, 1624803569284, 1624803749742]

# Reading 8
dataN1 = pd.read_csv('Data/age20June272021.csv')
time_stampsN1 = [1624810203138, 1624810504150, 1624810697546]

# Reading 9
dataP1 = pd.read_csv('Data/age23June272021.csv')
time_stampsP1 = [1624812157099, 1624812463834, 1624812648001]
