#include <iostream>
#include <vector>
#include <chrono>
#include "fix_state.h"
using namespace std;
using namespace std::chrono;

void print(vector<type> v)
{
    static int state = 1;
    printf("State %i: ",state);
    for (type x : v)
    {
        cout << x << " ";
    }
    cout << "\n";
    state++;
}

int main()
{
    // clock
    auto start = high_resolution_clock::now();

    // load tables
    int fluid = 1; // 1 for water, 2 for R134 but it's not yet implemented
    load_tables(fluid);
    
    // Define cycle parameters
    type P_high = 5;
    type T_high = 550;
    type P_low = 10e-3;
    type e_turbine = .9;
    type e_pump = .9;
    type m_dot = 1;

    // fix states
    vector<type> state_1 = fix_state(0, P_high, 1, T_high);
    type s2 = state_1[5];
    vector<type> state_2 = fix_state(0, P_low, 5, s2);
    if (e_turbine != 1)
    {
        type h2_actual = state_1[4] + (state_2[4] - state_1[4]) * e_turbine;
        state_2 = fix_state(0,P_low,4,h2_actual);
    }
    vector<type> state_3 = fix_state(0, P_low, 6, 0);
    type s4 = state_3[5];
    vector<type> state_4 = fix_state(0, P_high, 5, s4);
    if (e_pump != 1)
    {
        type h4_actual = state_3[4] + (state_4[4] - state_3[4]) / e_pump;
        state_4 = fix_state(0,P_high,4,h4_actual);
    }

    // Calculate work, heat and efficiency
    type w = m_dot * ((state_1[4] - state_2[4]) - (state_4[4] - state_3[4]));
    type qin = m_dot * (state_1[4] - state_4[4]);
    type efficiency = w / qin * 100.0;

    // print everything
    print(state_1);
    print(state_2);
    print(state_3);
    print(state_4);
    printf("Specific work net out = %f kJ/kg\n", w);
    printf("Specific heat energy required = %f kJ/kg\n", qin);
    printf("Efficiency of the cycle = %f%%\n", efficiency);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "\nTime taken by program : " << duration.count() << " microseconds\n\n";
    return 0;
}
