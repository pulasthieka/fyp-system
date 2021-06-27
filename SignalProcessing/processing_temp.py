from scipy.signal.signaltools import detrend
from common import *

def ind_temp(data, flip, time_stamps):
    # Indices: TMP_rise, TMP_fall, TMP_rise/fall, TMP_baseline, TMP_AUC, TMP_env, TMP_max, TMP_min
    if flip:
        TData = data.TCData
        TCData = data.TData
    else:
        TData = data.TData
        TCData = data.TCData

    Ttime = data.TCDatatime
    EData = data.EData
    env_temp = np.mean(EData/100)
    time_points = get_timePoints(Ttime, time_stamps)

    tmp_data = (TData - TCData)/100
    last_ind = tmp_data.last_valid_index()
    tmp_data = remove_nan(tmp_data[:last_ind])
    plot_raw(tmp_data, np.array(time_points)/T_fs, 'Temperature', T_fs)

    tmp_data = detrend(tmp_data)
    filtered_temp = uniform_filter1d(tmp_data, size=80)

    baseline = np.mean(tmp_data[time_points[0]:time_points[1]])
    max_tmp = max(TData/100)
    min_tmp = min(TData/100)

    tmp_ind = long_term_indices(filtered_temp, 60, 120)
    AUC = get_AUC(filtered_temp,time_points[0],len(filtered_temp))

    tmp_ind.append(baseline)
    tmp_ind.append(AUC)
    tmp_ind.append(env_temp)
    tmp_ind.append(max_tmp)
    tmp_ind.append(min_tmp)
    return tmp_ind

data = dataN1
time_stamps = time_stampsN1
TMP_indicesN1 = ind_temp(data, True, time_stamps)