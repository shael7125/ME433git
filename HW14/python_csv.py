import csv
import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function
import math

t = [] # column 0
data1 = [] # column 1

# MAF parameters:
# X for sigA is 500, sigB is 200, sigC is 100, sigD is 30

X = math.inf

# IIF parameters:
# for sigA, 0.993 = A; 0.007 = B, for sigB, A = 0.99 and B = 0.01 for sigC 0.9 and 0.1 for sigD, 0.95 and 0.05

A = 0.95
B = 0.05

# reading csv files #1
with open('sigB.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data1.append(float(row[1])) # second column

# for i in range(len(t)):
    # # print the data to verify it was read
    # print(str(t[i]) + ", " + str(data1[i]))
    # print(str(t[i]) + ", " + str(data1[i]) + ", " + str(data2[i]))

dt =  len(t)/t[-1] # samples per second
print(dt)

# plotting code: #4
Ts = 1.0/dt; # sampling interval

y = data1 # the data to make the fft from
n = len(y) # length of the signal
k = np.arange(n)
T = n/dt
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

# fig, (ax1, ax2) = plt.subplots(2, 1)
# print("%d\n", len(t))
# print("%d\n", len(y))
# print("%d\n", len(data1))
# ax1.plot(t,y,'b')
# ax1.set_xlabel('Time')
# ax1.set_ylabel('Amplitude')
# ax2.loglog(frq,abs(Y),'b') # plotting the fft
# ax2.set_xlabel('Freq (Hz)')
# ax2.set_ylabel('|Y(freq)|')

# plt.show()

# part 5
def moving_average_filter(data, X):
    filtered_data = []
    for i in range(len(data)):
        if i < len(data) - X + 1:
            filtered_data.append(np.mean(data[i:i+X]))
        else:
            ignore = 1
            filtered_data.append(np.mean(data[i:i+X - ignore]))
            ignore = ignore + 1
    return np.array(filtered_data)


# def plot_data_with_fft(original_data, filtered_data, X): # 5
# def plot_data_with_fft(original_data, filtered_data, A, B): #6
def plot_data_with_fft(original_data, filtered_data, X, type, cutoff, band): #7
    # Plot original and filtered data
    plt.figure(figsize=(14, 6))

    plt.subplot(2, 1, 1)
    plt.plot(original_data, 'k-', label='Original Data')
    plt.plot(filtered_data, 'r-', label='Filtered Data')
    # plt.title(f'Original and Filtered Data (Averaged over {X} points)')
    # plt.title(f'Original and Filtered Data (A = {A} and B = {B})')
    plt.title(f'Original and Filtered Data ({X},{type},{cutoff},{band})')
    plt.xlabel('Sample Number')
    plt.ylabel('Amplitude')
    plt.legend()

    # Compute FFT
    original_fft = np.fft.fft(original_data)
    original_fft = original_fft[range(int(n/2))]

    filtered_fft = np.fft.fft(filtered_data)
    filtered_fft = filtered_fft[range(int(n/2))]

    freqs = np.fft.fftfreq(len(original_data))
    freqs = freqs[range(int(n/2))]

    plt.subplot(2, 1, 2)
    plt.loglog(freqs, np.abs(original_fft), 'k-', label='Original FFT')
    plt.loglog(freqs, np.abs(filtered_fft), 'r-', label='Filtered FFT')
    plt.title('FFT of Original and Filtered Data')
    plt.xlabel('Frequency')
    plt.ylabel('Magnitude')
    plt.legend()

    plt.tight_layout()
    plt.show()

# filtered_data = moving_average_filter(data1, X)
# plot_data_with_fft(data1, filtered_data, X)

# 6

def iif(data, A, B):
    iif_data = np.zeros(len(data))
    for i in range(len(data)):
        if i == 0:
            iif_data[i] = B * data[i]
        iif_data[i] = A * iif_data[i-1] + B * data[i]
    return np.array(iif_data)

filtered_data = iif(data1, A, B)
# plot_data_with_fft(data1, filtered_data, A, B)

# 7
coeffs = []
with open('bcoeffs.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        coeffs.append(float(row[0])) # leftmost column

def fir(data, coeffs):
    x = len(coeffs)
    fir_data = data[:x]
    for i in range(x,len(data)):
        val = 0
        for j in range(x):
            val += coeffs[j]*data[i-j]
        fir_data.append(val)
    return np.array(fir_data)

# FIR frequency def plot_data_with_fft(original_data, filtered_data, X, type, cutoff, band):
X = len(coeffs)
type = 'rectangular'
cutoff = 25
band = 25

filtered_data = fir(data1,coeffs)
plot_data_with_fft(data1, filtered_data, X,type,cutoff,band)



    


