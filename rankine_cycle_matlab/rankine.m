% Copyright (C) 2022 Ahmed
% Author: Ahmed <ahmed@Ahmed-PC>
% Created: 2022-10-18

clear;
clc;
close all;

%% Define the tables
fluid = input('Enter the working fluid, 1 for water, 2 for R134, 3 for ideal gas:  ');
%fluid = 1;
tic;
fix_state([], [], [], [], fluid); % Identify the type of the fluid to load its tables

%% Inputs
P_high = input("Enter the pressure of the fluid entering the turbine in MPa:  ");
T_high = input("Enter the temperature of the fluid entering the turbine in (deg C):  ");
P_low = input("Enter the pressure of the fluid entering the condenser in MPa:  ");
e_turbine = input("Enter the efficiency of the turbine: ");
e_pump = input("Enter the efficiency of the pump: ");
 m_dot = input("Enter the rate of mass flow (if you want values specific enter 1) : ");

%P_high = 5;
%T_high = 400;
%P_low = 10.1e-3;
%e_turbine = 0.8;
%e_pump = 0.85;
%m_dot = 1;

tic;

state_1 = fix_state(1,P_high,2,T_high)

s2 = state_1(6);
state_2 = fix_state(1,P_low,6,s2)

if(e_turbine~=1)
  h2_actual = state_1(5) + (state_2(5) - state_1(5)) * e_turbine;
  state_2 = fix_state(1,P_low,5,h2_actual)
end

state_3 = fix_state(1,P_low,7,0)


s4 = state_3(6);
state_4 = fix_state(1,P_high,6,s4)

if(e_pump~=1)
  h4_actual = state_3(5) + (state_4(5) - state_3(5)) / e_pump;
  state_4 = fix_state(1,P_high,5,h4_actual)
end

w = m_dot*((state_1(5)-state_2(5)) - (state_4(5) - state_3(5)));
qin = m_dot*(state_1(5) - state_4(5));
efficiency = w/qin*100.0;

fprintf("Work net out = %f kJ/kg\n",w);
fprintf("Heat energy required = %f kJ/kg\n",qin);
fprintf("Efficiency of the cycle = %f%%\n",efficiency);
toc

