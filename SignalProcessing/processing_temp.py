from common import *

def ind_temp(data, flip, time_stamps):
    # Indices: TMP_rise, TMP_fall, TMP_rise/fall, TMP_base, TMP_AUC, TMP_env, TMP_max, TMP_min
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
    tmp_data = np.array(tmp_data)
    tmp_data[np.where(tmp_data>1)] = np.nan 
    tmp_data = remove_nan(tmp_data[:last_ind])
    plot_raw(tmp_data, np.array(time_points)/T_fs, 'Temperature', T_fs)
    plt.show()

    baseline = tmp_data[time_points[0]:time_points[1]]
    # tmp_data = detrend_sig(baseline, tmp_data)
    filtered_temp = uniform_filter1d(tmp_data, size=80)
    
    plot_raw(filtered_temp, np.array(time_points)/T_fs, 'Temperature', T_fs)
    plt.show()

    base = np.mean(baseline)
    max_tmp = max(TData/100)
    min_tmp = min(TData/100)

    tmp_ind = long_term_indices(filtered_temp, 60, 120)
    AUC = get_AUC(filtered_temp,time_points[0],len(filtered_temp))

    tmp_ind.append(base)
    tmp_ind.append(AUC)
    return tmp_ind

TMP_indicesM1 = ind_temp(dataM1, False, time_stampsM1)
TMP_indicesM2 = ind_temp(dataM2, True, time_stampsM2)
TMP_indicesM3 = ind_temp(dataM3, True, time_stampsM3)
TMP_indicesM4 = ind_temp(dataM4, True, time_stampsM4) 
TMP_indicesN1 = ind_temp(dataN1, True, time_stampsN1)
TMP_indicesN2 = ind_temp(dataN2, True, time_stampsN2)
TMP_indicesN3 = ind_temp(dataN3, True, time_stampsN3)
TMP_indicesA1 = ind_temp(dataA1, True, time_stampsA1)
TMP_indicesA2 = ind_temp(dataA2, False, time_stampsA2) ### Ignore timestamps
TMP_indicesA3 = ind_temp(dataA3, False, time_stampsA3)
TMP_indicesA4 = ind_temp(dataA4, True, time_stampsA4)
# TMP_indicesA5 = ind_temp(dataA5, True, time_stampsA5)
TMP_indicesT1 = ind_temp(dataT1, False, time_stampsT1)
TMP_indicesT2 = ind_temp(dataT2, True, time_stampsT2)


