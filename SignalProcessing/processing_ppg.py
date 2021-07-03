from common import *
def get_filt_ppg(data, flip, time_stamps):

    if flip:
        SData = data.SCData
        BData = data.BCData
        SCData = data.SData
        BCData = data.BData
    else:
        SData = data.SData
        BData = data.BData
        SCData = data.SCData
        BCData = data.BCData
    Stime = data.SDatatime 
    time_points = get_timePoints(Stime, time_stamps)

    R = (BData/np.mean(BData))/(SData/np.mean(SData))
    SPO2 = 110 - 25*R

    RC = (BCData/np.mean(BCData))/(SCData/np.mean(SCData))
    SPO2C = 110 - 25*RC

    plot_ppg_raw(SPO2, SPO2C, time_points)

    filtered_ppg_signal = filter_ppg(SPO2, S_fs, True)
    baseline = filtered_ppg_signal[time_points[0]:time_points[1]]
    filtered_ppg = remove_outliers(filtered_ppg_signal, baseline)

    filtered_ppgC_signal = filter_ppg(SPO2C, S_fs, True)
    baselineC = filtered_ppgC_signal[time_points[0]:time_points[1]]
    filtered_ppgC = remove_outliers(filtered_ppgC_signal, baselineC)

    plot_ppg_raw(filtered_ppg, filtered_ppgC, time_points)

    return [time_points, filtered_ppg_signal, filtered_ppgC_signal]

def remove_outliers(sig, base):
    FACTOR = 1.4
    pk, _ = find_peaks(base, distance=60, prominence=(None, 0.6))
    if len(pk):
        thresh = np.mean(base[pk])*FACTOR
    else:
        pk, _ = find_peaks(base, distance=60)
        thresh = np.mean(base[pk])*FACTOR
    
    sig = sig[np.where(~(sig>thresh))]
    sig = sig[np.where(~(sig<-thresh))]

    # clean_sig = remove_nan(sig)
    clean_sig = sig/thresh
    return clean_sig

def plot_ppg_raw(exp, control, time_points):
    ax1 = plt.subplot(2, 1, 1)
    plot_raw(exp, np.array(time_points)/S_fs, 'Experimental Arm', S_fs)

    ax2 = plt.subplot(2, 1, 2, sharex=ax1, sharey=ax1)
    plot_raw(control, np.array(time_points)/S_fs, 'Control Arm', S_fs)
    plt.show()

def get_pulse_amps(data):
    peaks, _ = find_peaks(data, distance=60)

    # plt.plot(data)
    # plt.plot(peaks, data[peaks], "x")
    # plt.show()
    return peaks

def filter_ppg(raw_signal, fs, to_plot):
    # Clean the signal - Mean removal
    raw_signal = remove_nan(np.array(raw_signal))
    raw_signal = raw_signal - np.mean(raw_signal)

    # Lowpass filtering
    A_low = 100
    delt_w_low = 7.21 - 3.8
    M_low = (A_low-8)/(2.285*delt_w_low)
    order_low = 80
    fc_low = 6

    b_low = signal.firwin(order_low, fc_low, window=('kaiser', M_low), fs=fs) 
    filtered_low = filtfilt(b_low, 1, raw_signal)

    # Highpass filtering
    fc_high = 0.5
    order_high = 201

    b_high = signal.firwin(order_high, fc_high, window='hamming', pass_zero=False, fs=fs)       # WHY????
    filtered_high = filtfilt(b_high, 1, filtered_low)

    filtered_signals = [raw_signal, filtered_low, filtered_high]
    if to_plot:
        plot_filtering(filtered_signals, 'PPG', S_fs)

    return filtered_signals[2]

def plot_filtering(signals, label, fs):
    t = np.array(range(len(signals[0])))/fs

    ax1 = plt.subplot(3, 1, 1)
    ax1.plot(t, signals[0])
    plt.title("Raw " + label+" Signal")
    plt.ylabel('Amplitude')
    plt.grid()

    ax2 = plt.subplot(3, 1, 2, sharex=ax1)
    ax2.plot(t, signals[1])
    plt.title("Lowpass Filtered " + label+" Signal")
    plt.ylabel('Amplitude')
    plt.grid()

    ax3 = plt.subplot(3, 1, 3, sharex=ax1)
    ax3.plot(t, signals[2])
    plt.title("Bandpass Filtered " + label+" Signal")
    plt.xlabel('Times(s)')
    plt.ylabel('Amplitude')
    plt.grid()

    plt.show()

def norm(preOcclusion, preOcclusionC, postOcclusion, postOcclusionC):
    pre_amps = get_pulse_amps(preOcclusion)
    post_amps = get_pulse_amps(postOcclusion)
    pre_ampsC = get_pulse_amps(preOcclusionC)
    post_ampsC = get_pulse_amps(postOcclusionC)

    pre_x = np.linspace(max(pre_amps[0],pre_ampsC[0]), min(pre_amps[-1],pre_ampsC[-1]), round(max(pre_amps[-1],pre_ampsC[-1])/10))
    post_x = np.linspace(max(post_amps[1],post_ampsC[1]), min(post_amps[-1],post_ampsC[-1]), round(max(post_amps[-1],post_ampsC[-1])/10))

    pre_f = interpolate.interp1d(pre_amps, preOcclusion[pre_amps])
    preC_f = interpolate.interp1d(pre_ampsC, preOcclusionC[pre_ampsC])
    post_f = interpolate.interp1d(post_amps, postOcclusion[post_amps])
    postC_f = interpolate.interp1d(post_ampsC, postOcclusionC[post_ampsC])

    preSig = pre_f(pre_x)
    preSigC = preC_f(pre_x)
    postSig = post_f(post_x)
    postSigC = postC_f(post_x)

    pre_norm = (preSig/preSigC)
    post_norm = (postSig/postSigC)

    plt.plot(postSig)
    plt.plot(postSigC)
    plt.plot(post_norm)
    plt.legend(['Exp','Control','Normalized'])
    plt.xlabel('Sample points')
    plt.ylabel('Amplitude')
    plt.title('Normalization')
    plt.show()
    return [pre_norm, post_norm]

def get_hyp_indices(post_norm, fs):
    # Indices: hyp_pk, hyp_loc, hyp_grad, hyp_AUC
    filtered_hyp = uniform_filter1d(post_norm, size=50)
    plt.plot(post_norm)
    plt.plot(filtered_hyp)
    plt.show()

    hyp_pk = max(filtered_hyp[:int(len(filtered_hyp)/2)])
    hyp_loc = np.argmax(filtered_hyp[:int(len(filtered_hyp)/2)])/fs
    hyp_grad = hyp_pk/hyp_loc
    filtered_hyp[filtered_hyp<0]=0
    hyp_AUC = np.trapz(filtered_hyp,dx=1/fs)

    return [hyp_pk, hyp_loc, hyp_grad, hyp_AUC]

def get_pulses(data):
    neg_peaks, _ = find_peaks(-data, distance=60)
    avg_pulse_len = np.mean(np.diff(neg_peaks)) 

    pulses = []
    for i in range(len(neg_peaks)-1):
        pulse = np.zeros(int(avg_pulse_len*1.2))
        p_len = neg_peaks[i+1]-neg_peaks[i]
        
        if (p_len<avg_pulse_len*1.2)&(p_len>avg_pulse_len*0.8):
            p = data[neg_peaks[i]:neg_peaks[i+1]]
            pulse[:p_len] = p - min(p)
            pulses.append(pulse)
            # plt.plot(pulse)
    # plt.show()
    return pulses

def get_pk_locs(pulse):
    sys_pk_loc = np.argmax(pulse)  
    sys_wave = np.zeros(len(pulse))
    sys_wave_half = pulse[:sys_pk_loc]
    sys_wave[:sys_pk_loc*2] = np.concatenate([sys_wave_half,np.flip(sys_wave_half)])
    di_wave = pulse - sys_wave

    di_pk_loc = np.argmax(di_wave)  

    # plt.plot(pulse)
    # plt.plot(sys_wave)
    # plt.plot(di_wave)
    # plt.title('Systolic Wave and Diastolic Wave')
    # plt.legend(['Pulse','Systolic Wave','Diastolic Wave'])
    # plt.xlabel('Sample Points')
    # plt.ylabel('Amplitude')
    # plt.show()
    IPA = np.trapz(di_wave)/np.trapz(sys_wave)

    return [di_pk_loc, sys_pk_loc, IPA]

def pulse_indices(pulse):
    # Indices: sys_pk, AUC, delt_T, CT, RI, SI, STT, IPA, pulse_wa_ratio
    [di_pk_loc, CT, IPA] = get_pk_locs(pulse)
    sys_pk = max(pulse)
    di_pk = pulse[di_pk_loc]

    RI = di_pk/sys_pk
    delt_T = di_pk_loc - CT
    SI = HEIGHT/delt_T
    STT = CT/sys_pk
    pulse_wa_ratio = len(pulse)/sys_pk
    AUC = np.trapz(pulse)

    return [sys_pk, AUC, delt_T, CT, RI, SI, STT, IPA, pulse_wa_ratio]

def get_seg_indices(region, WINDOW_LEN = 10):
    # POST_SEG_LEN = len(region)/S_fs
    POST_SEG_LEN = 5*60
    indices = []
    SHIFT_RATIO = WINDOW_LEN
    for i in range(int(POST_SEG_LEN/(WINDOW_LEN/SHIFT_RATIO))-SHIFT_RATIO):
        seg_start = int(S_fs*i*WINDOW_LEN/SHIFT_RATIO)
        post_seg = region[seg_start:seg_start+WINDOW_LEN*S_fs]
        seg_pulses = []
        try:
            seg_pulses = get_pulses(post_seg)
            avg_pulse = np.mean(seg_pulses, axis=0)
            seg_indices = pulse_indices(avg_pulse)
            plt.plot(avg_pulse)
        except Exception as inst:
            print(inst)
            print('Error in segment ',i)
            print(seg_pulses)
            seg_indices = np.zeros(9)
            break
        indices.append(seg_indices)
    
    return indices

def get_pulse_inds(preOcclusion, preOcclusionC, postOcclusion, postOcclusionC):
    index_names = ['sys_pk', 'AUC', 'delt_T', 'CT', 'RI', 'SI', 'STT', 'IPA', 'pulse_wa_ratio']

    # Pre-occlusion
    ax1 = plt.subplot(2,2,1)
    pre_inds = get_seg_indices(preOcclusion, WINDOW_LEN = 10)
    plt.title('Baseline - Exp Pulses')
    # avg_post_inds = [float(sum(col))/sum(map(bool,col)) if sum(map(bool,col))>0 else 0 for col in zip(*post_inds)]
    # print('Hyp Exp')
    # print(avg_post_inds)

    ax2 = plt.subplot(2,2,2)
    pre_indsC = get_seg_indices(preOcclusionC, WINDOW_LEN = 10)
    # avg_post_indsC = [float(sum(col))/sum(map(bool,col)) if sum(map(bool,col))>0 else 0 for col in zip(*post_indsC)]
    # print('Hyp Control')
    # print(avg_post_indsC)
    plt.title('Baseline - Control Pulses')

    # Post-occlusion
    ax3 = plt.subplot(2,2,3)
    post_inds = get_seg_indices(postOcclusion, WINDOW_LEN = 10)
    plt.title('Hyp - Exp Pulses')
    # avg_post_inds = [float(sum(col))/sum(map(bool,col)) if sum(map(bool,col))>0 else 0 for col in zip(*post_inds)]
    # print('Hyp Exp')
    # print(avg_post_inds)

    ax4 = plt.subplot(2,2,4)
    post_indsC = get_seg_indices(postOcclusionC, WINDOW_LEN = 10)
    # avg_post_indsC = [float(sum(col))/sum(map(bool,col)) if sum(map(bool,col))>0 else 0 for col in zip(*post_indsC)]
    # print('Hyp Control')
    # print(avg_post_indsC)
    plt.title('Hyp - Control Pulses')
    plt.show()

    pre_inds = np.array(pre_inds)
    pre_indsC = np.array(pre_indsC)
    post_inds = np.array(post_inds)
    post_indsC = np.array(post_indsC)

    ax1 = plt.subplot(2,2,1)
    for i in range(pre_inds.shape[1]):
        plt.plot(pre_inds[:,i])
    plt.legend(index_names)
    plt.title('Exp Baseline Indices')

    ax2 = plt.subplot(2,2,3, sharey=ax1, sharex=ax1)
    for i in range(pre_indsC.shape[1]):
        plt.plot(pre_indsC[:,i])
    plt.legend(index_names)
    plt.title('Control Baseline Indices')

    ax3 = plt.subplot(2,2,2, sharey=ax1, sharex=ax1)
    for i in range(post_inds.shape[1]):
        plt.plot(post_inds[:,i])
    plt.legend(index_names)
    plt.title('Exp Hyp Indices')

    ax4 = plt.subplot(2,2,4, sharey=ax1, sharex=ax1)
    for i in range(post_indsC.shape[1]):
        plt.plot(post_indsC[:,i])
    plt.legend(index_names)
    plt.title('Control Hyp Indices')
    plt.show()
    return [pre_inds, pre_indsC, post_inds, post_indsC]

def plot_inds_comparison(index, pulse_inds):
    index_names = ['sys_pk', 'AUC', 'delt_T', 'CT', 'RI', 'SI', 'STT', 'IPA', 'pulse_wa_ratio']
    ax1 = plt.subplot(2,2,1)
    plt.plot(pulse_inds[0][:,index])
    plt.axhline(y=np.mean(pulse_inds[0][:,index]), color='r')
    plt.legend([index_names[index]])
    plt.title('Exp Baseline Indices')

    ax2 = plt.subplot(2,2,3, sharey=ax1, sharex=ax1)
    plt.plot(pulse_inds[1][:,index])
    plt.axhline(y=np.mean(pulse_inds[1][:,index]), color='r')
    plt.legend([index_names[index]])
    plt.title('Control Baseline Indices')

    ax3 = plt.subplot(2,2,2, sharey=ax1, sharex=ax1)
    plt.plot(pulse_inds[2][:,index])
    plt.axhline(y=np.mean(pulse_inds[2][:,index]), color='r')
    plt.legend([index_names[index]])
    plt.title('Exp Baseline Indices')

    ax4 = plt.subplot(2,2,4, sharey=ax1, sharex=ax1)
    plt.plot(pulse_inds[3][:,index])
    plt.axhline(y=np.mean(pulse_inds[3][:,index]), color='r')
    plt.legend([index_names[index]])
    plt.title('Control Baseline Indices')
    plt.show()

def get_ppg_indices(sig, flip, time_stamps):

    [time_points, filtered_ppg, filteredC_ppg] = get_filt_ppg(sig, flip, time_stamps)

    PRE_SEG_LEN = 5*60
    POST_SEG_LEN = 5*60

    preOcclusion = filtered_ppg[time_points[1]-PRE_SEG_LEN*S_fs:time_points[1]]
    postOcclusion = filtered_ppg[time_points[2]:time_points[2]+POST_SEG_LEN*S_fs]
    preOcclusionC = filteredC_ppg[time_points[1]-PRE_SEG_LEN*S_fs:time_points[1]]
    postOcclusionC = filteredC_ppg[time_points[2]:time_points[2]+POST_SEG_LEN*S_fs]

    # Hyp indices
    [ _ , post_norm] = norm(preOcclusion, preOcclusionC, postOcclusion, postOcclusionC)
    hyp_indices = get_hyp_indices(post_norm, S_fs)
    indices = hyp_indices

    # Freq indices
    freq_inds = indices_fft(filteredC_ppg, S_fs, 'SPO2')
    indices.append(freq_inds)

    # Pulse Indices
    pulse_inds = get_pulse_inds(preOcclusion, preOcclusionC, postOcclusion, postOcclusionC)
    # for i in range(9):
    #     plot_inds_comparison(i, pulse_inds)
    return indices

data = dataN1
time_stamps = time_stampsN1
PPG_indicesN1 = get_ppg_indices(data, True, time_stamps)