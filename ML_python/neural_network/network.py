import neural_network
import numpy as np
import pandas as pd
import math
import matplotlib.pyplot as plt
from sklearn.metrics import r2_score, mean_absolute_error

# prepare the output
def find_ans(ans):
    ans = ans.reshape(32)
    string = []
    for i in ans:
        if i>0.5:
            string.append('1')
        else:
            string.append('0')
    answer = "".join(string[:-16])
    answer = '0b' + answer
    b = string[-16:]
    carry = 0
    if '1' in b:
        # reverse b to find the last occurence of 1, index() finds the 1st
        b.reverse()
        i = b.index('1')
        i = len(b) - i - 1
        b.reverse()
        for j in range(i, -1, -1): # this means reverse the order of array [i, i-1, ..., 0]
            carry = 0.5 * (int(b[j]) + carry)
    answer = int(answer, 2) + carry
    return answer

# find the decimal part of the no.
def binary_dec(dec):
    counter = 1
    dec_list = []
    decproduct = dec
    while (counter <= 16):
        decproduct *= 2
        (decdec, decwhole) = math.modf(decproduct)
        decwhole = int(decwhole)
        dec_list.append(decwhole)
        decproduct = decdec
        counter += 1
    return dec_list

# prepare the input
def vectorize(y):
    e = np.zeros(32)
    (dec, integer) = math.modf(y)
    integer = int(integer) # to remove the .0
    binary = ([int(d) for d in bin(integer)[2:]], binary_dec(dec))
    e[16-len(binary[0]):-16] = binary[0]
    e[16:] = binary[1]
    e = e.reshape(-1, 1)
    return e

# read the steam tables
def load_data():
    sat_table = np.array(pd.read_csv("../steam_tables/sat_table.csv").values.tolist())
    sat_table = sat_table.transpose()
    # table_3 = np.array(pd.read_csv("../steam_tables/table_3.csv").values.tolist())
    # table_3 = table_3.transpose()
    training_data = (sat_table[0], sat_table[1])
    T = [vectorize(x) for x in training_data[1]] # Temperature
    inputs = [np.reshape(x, (32, 1)) for x in T]
    P = [vectorize(y) for y in training_data[0]] # Pressure
    train = list(zip(inputs, P))
    return train, sat_table


ALL_DATA = load_data()
train = ALL_DATA[0]
sat_table = ALL_DATA[1]

# Initialise the network
net = neural_network.Network([32, 20, 20, 32])

# Train the network
net.SGD(training_data=train, epochs=50, mini_batch_size=1, eta=0.5, test_data=None) # epochs 50-100

# Test and Plot
x_plot = np.arange(1, 380, 0.5)
y_plot = []
for i in x_plot:
    y_plot.append(find_ans(net.feedforward(vectorize(i))))

plt.scatter(x_plot, y_plot, marker='*', color='g')
plt.scatter(sat_table[1], sat_table[0], color='r')
plt.xlabel("T (C)")
plt.ylabel("P (MPa)")
plt.show()
