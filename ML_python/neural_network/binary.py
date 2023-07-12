import numpy as np
import math

# ============== This script changes floating binary numbers into floating decimals and vice versa ==============

# prepare the output
def find_ans(ans):
    ans = ans.reshape(32)
    print(ans)
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
    
a = 1

b = vectorize(a)
print("b =", b)
ans = find_ans(b)

print(ans)