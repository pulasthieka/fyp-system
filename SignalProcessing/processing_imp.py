
from common import *

FILT_ORDER = 100
def ind_imp(data, flip, time_stamps):
    # Indices: IMP_rise, IMP_fall, IMP_rise/fall, IMP_baseline, IMP_AUC
    
    if flip:
        WCData = data.WData
        WData = data.WCData
    else:
        WData = data.WData
        WCData = data.WCData

    Itime = data.YCDatatime
    time_points = get_timePoints(Itime, time_stamps)
    imp_data = np.power((np.power(WData,2)+np.power(WCData,2)), 0.5)
    last_ind = imp_data.last_valid_index()
    imp_data = remove_nan(imp_data[:last_ind])

    baseline = imp_data[time_points[0]:time_points[1]]
    imp_data = detrend_sig(baseline, imp_data)
    plot_raw(imp_data, np.array(time_points)/I_fs, 'Raw Bio-impedance', I_fs)
    plt.show()

    filtered_imp = uniform_filter1d(imp_data, size=FILT_ORDER)
    plot_raw(filtered_imp, np.array(time_points)/I_fs, 'Filtered Bio-impedance', I_fs)
    plt.show()


    # imp_ind = long_term_indices(filtered_imp, 60, 120)
    # AUC = get_AUC(filtered_imp,time_points[0],len(filtered_imp))

    # imp_ind.append(baseline)
    # imp_ind.append(AUC)
    imp_ind = []
    return imp_ind

data = dataN1
time_stamps = time_stampsN1
IMP_indicesN1 = ind_imp(data, False, time_stamps)

