import pandas as pd
import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
from sklearn.preprocessing import PolynomialFeatures
from sklearn import linear_model

def func(X, A0, A1, A2, B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, Be1, Be2):
    (x,z) = X
    #  y = 1 / (1 + np.exp(-Beta_1*(x-Beta_2)))
    # y = np.poly(Beta_1*(x-Beta_2))
    # y = B0 + B1*(x) + B2*(x)**2 + B3*(x)**3 + B4*(x)**4 + B5*(x)**5 + B6*(x)**6 + B7*(x)**7 + B8*(x)**8 + B9*(x)**9 + B10*(x)**10 + B11*(x)**11 + B12*(x)**12 + B13*(x)**13 + B14*(x)**14
    y = np.exp(-Be1*(x)) + np.exp(-Be2*(z)) + A0 + A1*(x) + A2*(x)**2 + B1*(z) +  + B2*(z)**2 + np.sin(B3*x/np.pi) + np.sin(B4*z/np.pi)
    # y = np.sin(B1*x/np.pi) + np.sin(B2*x/np.pi) + np.sin(B3*x/np.pi) + np.sin(B4*x/np.pi) + np.sin(B5*x/np.pi) + np.sin(B6*x/np.pi) + np.sin(B7*x/np.pi) + np.sin(B8*x/np.pi)
    return y

sat_table = pd.read_csv("../steam_tables/sat_table.csv")
table_3 = pd.read_csv("../steam_tables/table_3.csv")

T = np.array(table_3[" Temperature (deg C)"].values.tolist())
P = np.array(table_3["Pressure (MPa)"].values.tolist())
V = np.array(table_3[" Specific Volume (m^3/kg)"].values.tolist())

# xdata = np.c_[T.flatten(), V.flatten()]
# T = table_3[" Temperature (deg C)"]
# V = table_3[" Specific Volume (m^3/kg)"].reshape(-1, 1)
# P = table_3["Pressure (MPa)"]

# Building the model
# polynomial_converter = PolynomialFeatures(degree=10,include_bias=False)
# model = linear_model.LinearRegression(fit_intercept=True)
# model.fit(T,P)
popt, pcov = curve_fit(func, (T,V), P)

#logistic function
y_hat = func((T,V), *popt)
# y_hat = model.predict(T)

# evaluation
print("Mean absolute error: %.2f" % np.mean(np.absolute(y_hat - P)))
print("Residual sum of squares (MSE): %.2f" % np.mean((y_hat - P) ** 2))

#plot initial prediction against datapoints
plt.figure(1,figsize=(8,5))
plt.xlabel("Temperature (deg C)")
plt.ylabel("Pressure (Mpa)")
plt.grid()
plt.plot(V, y_hat)
plt.plot(V, P,'ro')
plt.show()

print(func((2000,0.062181), *popt))