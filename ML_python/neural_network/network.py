import neural_network
import numpy as np
import pandas as pd
import math

def binary_dec(dec):
    num = [0, 0, 0]
    n = len(dec)
    if(n):
        num[0] = (dec[0]>='4')
    if(n>1):
        num[1] = (dec[1]>'5')
    if(n>2):
        num[2] = (int(dec[2]) % 2)
    return num

def find_ans(ans):
    ans = ans.reshape(16)
    for i in range(len(ans)):
        ans[i] = (ans[i]>0.5)

    string = [str(a) for a in ans]
    for i in range(len(ans)):
        string[i] = string[i][0]
    answer = "".join(string[:-3])
    answer = "0b" + answer
    answer = int(answer,2)
    return answer

def vectorize(y):
    e = np.zeros(16)
    (dec, integer) = math.modf(y)
    integer = int(integer) # to remove the .0
    dec = str(dec)[2:] # to remove the 0.
    binary = ([int(d) for d in bin(integer)[2:]], binary_dec(dec))
    e[13-len(binary[0]):-3] = binary[0]
    e[13:13+min(len(binary[1]), 3)] = binary[1][0:3]
    e = e.reshape(-1, 1)
    return e

def load_data():
    sat_table = np.array(pd.read_csv("../steam_tables/sat_table.csv").values.tolist())
    table_3 = np.array(pd.read_csv("../steam_tables/table_3.csv").values.tolist())
    sat_table = sat_table.transpose()
    table_3 = table_3.transpose()
    training_data = (table_3[0], table_3[1], table_3[2])
    training_data = np.float32(training_data)
    print(len(training_data[0]))
    z = np.array(len(training_data[0]))

    # for i in range(len(training_data[0])):
    inputs = [[training_data[0][i], training_data[1][i]] for i in range(len(training_data[0]))]
        # [[x,y] for [x,y] in [training_data[0], training_data[1]]]
    # print(inputs)

    inputs = np.reshape(inputs, (-1, 1))
    V = [vectorize(y) for y in training_data[2]]
    train = list(zip(inputs, V))
    return train

train = load_data()
small_p = 0
large_p = 0

# for _ in range(50):
#     net = neural_network.Network([1, 500, 500, 16])
#     net.SGD(training_data=train, epochs=10000, mini_batch_size=10, eta=0.25, test_data=None)
#     small_p += find_ans(net.feedforward(.75))
#     large_p += find_ans(net.feedforward(20.5))
# print("For method 1: small_p = ", small_p/50,", large_p = ", large_p/50)

small_p = 0
large_p = 0
# for _ in range(50):
net = neural_network.Network([2, 20, 20, 16])
net.SGD(training_data=train, epochs=50, mini_batch_size=7, eta=.3, test_data=None)
small_p += find_ans(net.feedforward([7.5, 400]))
large_p += find_ans(net.feedforward([1, 110]))
print(small_p,large_p)
# print("For method 2: small_p = ", small_p/50,", large_p = ", large_p/50)
