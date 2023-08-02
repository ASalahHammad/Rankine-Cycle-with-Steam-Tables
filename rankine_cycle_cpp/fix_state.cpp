#include <iostream>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <algorithm>
#define type double
using namespace std;
static vector<vector<type>> sat_table;
static vector<vector<type>> table_3;
const int l_sat = 649; // length of saturation table
const int l_3 = 9527;  // length of superheated and compressed table

// functions declaration
void load_tables(int fluid);
vector<type> sat_table_func(int in1_col, type in1, int in2_col, type in2);
vector<type> table_3_func(int in1_col, type in1, int in2_col, type in2);
bool check_vector(vector<type> fixed_state);
bool check_vector(vector<int> fixed_state);
int lower(int a, int b, type x, vector<type> vec);
int lower(vector<int> a, type x, vector<type> vec);
int upper(int a, int b, type x, vector<type> vec);
int upper(vector<int> a, type x, vector<type> vec);
int find_first(int a, int b, type x, vector<type> vec);
int find_first(vector<int> a, type x, vector<type> vec);
vector<int> find_all(int a, int b, type x, vector<type> vec);
vector<int> find_all(vector<int> a, type x, vector<type> vec);

void load_tables(int fluid)
{
    sat_table.resize(10);
    table_3.resize(6);
    for (int i = 0; i < 10; i++)
    {
        sat_table[i].resize(l_sat);
    }
    for (int i = 0; i < 6; i++)
    {
        table_3[i].resize(l_3);
    }
    if (fluid == 1) // steam
    {
        cout << "Loading Steam Tables...\n";
        // load saturation table
        ifstream sat_table_in("steam_tables/sat_table.csv");
        if (!sat_table_in)
        {
            cout << "Error reading file\n";
        }
        for (int i = 0; i < l_sat; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                sat_table_in >> sat_table[j][i];
            }
        }
        sat_table_in.close();

        // load table 3
        ifstream table_3_in("steam_tables/table_3.csv");
        if (!table_3_in)
        {
            cout << "Error reading table_3\n";
        }
        for (int i = 0; i < l_3; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                table_3_in >> table_3[j][i];
            }
        }
        table_3_in.close();
    }
}

vector<type> fix_state(int in1_col, type in1, int in2_col, type in2)
{
    if (in1_col != 0)
    {
        throw invalid_argument("Sorry, the program doesn't yet know how to fix a state without knowing pressure, please enter pressure as input 1 with in1_col = 0");
    }
    vector<type> fixed_state = {-1, -1, -1, -1, -1, -1};
    fixed_state = sat_table_func(in1_col, in1, in2_col, in2);
    if (check_vector(fixed_state))
    {
        return fixed_state;
    }
    else
    {
        fixed_state = table_3_func(in1_col, in1, in2_col, in2);
        if (check_vector(fixed_state))
        {
            return fixed_state;
        }
        else
        {
            throw invalid_argument("Error fixing the state");
        }
    }
}

// ================= Table 3 =================
// int in1_first = in1_down;
// int in1_last = in1_up;
// while (sat_table[in1_col][in1_first] == sat_table[in1_col][in1_down])
// {
//     in1_first--;
// }
// in1_first++;
// while (sat_table[in1_col][in1_last] == sat_table[in1_col][in1_up])
// {
//     in1_last++;
// }
// in1_last--;
// int ind_1 = find_first(in1_first, in1_down, in2, sat_table[in2_col]);
// if (sat_table[in2_col]==in2) // in2 found
// {
//     interpolation_1[in1_col] = sat_table[in1_col][in1_down];
//     interpolation_1[other] = sat_table[other][in1_down];
//     for (int i = 2; i < 10; i++)
//     {
//         interpolation_1[i] = sat_table[i][in1_down];
//     }
// }
// else // in2 not found
// {
//     // then we will have to do interpolation twice
//     int in2_up_1 = upper(in1_first, in1_down, in2, sat_table[in2_col]);
//     int in2_down_1 = in2_up_1 - 1;
//     for (int i = 0; i < 10; i++)
//     {
//         interpolation_1[i] = sat_table[i][in2_down_1] + (in2 - sat_table[in2_col][in2_down_1]) * (sat_table[i][in2_up_1] - sat_table[i][in2_down_1]) / (sat_table[in2_col][in2_up_1] - sat_table[in2_col][in2_down_1]);
//     }
// }

vector<type> sat_table_func(int in1_col, type in1, int in2_col, type in2)
{
    int other = (0 * (in1_col == 1) + 1 * (in1_col == 0));
    vector<type> fixed_state = {-1, -1, -1, -1, -1, -1};
    int in1_ind = find_first(0, sat_table[in1_col].size() - 1, in1, sat_table[in1_col]);
    if (in1_ind != -1) // then in1 is found
    {
        vector<type> saturation_1;
        saturation_1.resize(10);
        for (int i = 0; i < 10; i++)
        {
            saturation_1[i] = sat_table[i][in1_ind];
        }
        // compare in2 with saturation of in1
        if (in2_col == 0 || in2_col == 1)
        {
            if (in2 == saturation_1[in2_col])
            {
                throw std::invalid_argument("Can't fix the state because the two inputs are dependent");
            }
        }
        else if (in2_col == 2) // volume
        {
            int col_1 = 2, col_2 = 3;
            if (in2 >= saturation_1[col_1] && in2 <= saturation_1[col_2])
            {
                type x = (in2 - saturation_1[col_1]) / (saturation_1[col_2] - saturation_1[col_1]);
                fixed_state[in1_col] = in1;
                fixed_state[other] = saturation_1[other];
                for (int i = 2, j = 2; i < 6; i++, j += 2)
                {
                    fixed_state[i] = saturation_1[j] + x * (saturation_1[j + 1] - saturation_1[j]);
                }
                return fixed_state;
            }
        }
        else if (in2_col == 3) // energy
        {
            int col_1 = 4, col_2 = 5;
            if (in2 >= saturation_1[col_1] && in2 <= saturation_1[col_2])
            {
                type x = (in2 - saturation_1[col_1]) / (saturation_1[col_2] - saturation_1[col_1]);
                fixed_state[in1_col] = in1;
                fixed_state[other] = saturation_1[other];
                for (int i = 2, j = 2; i < 6; i++, j += 2)
                {
                    fixed_state[i] = saturation_1[j] + x * (saturation_1[j + 1] - saturation_1[j]);
                }
                return fixed_state;
            }
        }
        else if (in2_col == 4) // enthalpy
        {
            int col_1 = 6, col_2 = 7;
            if (in2 >= saturation_1[col_1] && in2 <= saturation_1[col_2])
            {
                type x = (in2 - saturation_1[col_1]) / (saturation_1[col_2] - saturation_1[col_1]);
                fixed_state[in1_col] = in1;
                fixed_state[other] = saturation_1[other];
                for (int i = 2, j = 2; i < 6; i++, j += 2)
                {
                    fixed_state[i] = saturation_1[j] + x * (saturation_1[j + 1] - saturation_1[j]);
                }
                return fixed_state;
            }
        }
        else if (in2_col == 5) // entropy
        {
            int col_1 = 8, col_2 = 9;
            if (in2 >= saturation_1[col_1] && in2 <= saturation_1[col_2])
            {
                type x = (in2 - saturation_1[col_1]) / (saturation_1[col_2] - saturation_1[col_1]);
                fixed_state[in1_col] = in1;
                fixed_state[other] = saturation_1[other];
                for (int i = 2, j = 2; i < 6; i++, j += 2)
                {
                    fixed_state[i] = saturation_1[j] + x * (saturation_1[j + 1] - saturation_1[j]);
                }
                return fixed_state;
            }
        }
        else if (in2_col == 6)
        {
            fixed_state[in1_col] = in1;
            fixed_state[other] = saturation_1[other];
            for (int i = 2, j = 2; i < 6; i++, j += 2)
            {
                fixed_state[i] = saturation_1[j] + in2 * (saturation_1[j + 1] - saturation_1[j]);
            }
            return fixed_state;
        }
    }
    else // this means in1 is not found
    {
        // Find upper and lower bound and then interpolate
        int in1_up = upper(0, sat_table[in1_col].size() - 1, in1, sat_table[in1_col]);
        int in1_down = in1_up - 1;
        type interpolation_1[10], interpolation_2[10], interpolation_3[10];
        interpolation_1[in1_col] = sat_table[in1_col][in1_down];
        interpolation_1[other] = sat_table[other][in1_down];
        for (int i = 2; i < 10; i++)
        {
            interpolation_1[i] = sat_table[i][in1_down];
        }

        interpolation_2[in1_col] = sat_table[in1_col][in1_up];
        interpolation_2[other] = sat_table[other][in1_up];
        for (int i = 2; i < 10; i++)
        {
            interpolation_2[i] = sat_table[i][in1_up];
        }
        for (int i = 0; i < 10; i++)
        {
            interpolation_3[i] = interpolation_1[i] + (in1 - interpolation_1[in1_col]) * (interpolation_2[i] - interpolation_1[i]) / (interpolation_2[in1_col] - interpolation_1[in1_col]);
        }
        // compare in2 with saturation of interpolation_3
        if (in2_col == 0 || in2_col == 1)
        {
            if (in2 == interpolation_3[in2_col])
            {
                throw invalid_argument("Can't fix the state because the two inputs are dependent");
            }
        }
        else if (in2_col == 2) // volume
        {
            int col_1 = 2, col_2 = 3;
            if (in2 >= interpolation_3[col_1] && in2 <= interpolation_3[col_2])
            {
                type x = (in2 - interpolation_3[col_1]) / (interpolation_3[col_2] - interpolation_3[col_1]);
                fixed_state[in1_col] = in1;
                fixed_state[other] = interpolation_3[other];
                for (int i = 2, j = 2; i < 6; i++, j += 2)
                {
                    fixed_state[i] = interpolation_3[j] + x * (interpolation_3[j + 1] - interpolation_3[j]);
                }
                return fixed_state;
            }
        }
        else if (in2_col == 3) // energy
        {
            int col_1 = 4, col_2 = 5;
            if (in2 >= interpolation_3[col_1] && in2 <= interpolation_3[col_2])
            {
                type x = (in2 - interpolation_3[col_1]) / (interpolation_3[col_2] - interpolation_3[col_1]);
                fixed_state[in1_col] = in1;
                fixed_state[other] = interpolation_3[other];
                for (int i = 2, j = 2; i < 6; i++, j += 2)
                {
                    fixed_state[i] = interpolation_3[j] + x * (interpolation_3[j + 1] - interpolation_3[j]);
                }
                return fixed_state;
            }
        }
        else if (in2_col == 4) // enthalpy
        {
            int col_1 = 6, col_2 = 7;
            if (in2 >= interpolation_3[col_1] && in2 <= interpolation_3[col_2])
            {
                type x = (in2 - interpolation_3[col_1]) / (interpolation_3[col_2] - interpolation_3[col_1]);
                fixed_state[in1_col] = in1;
                fixed_state[other] = interpolation_3[other];
                for (int i = 2, j = 2; i < 6; i++, j += 2)
                {
                    fixed_state[i] = interpolation_3[j] + x * (interpolation_3[j + 1] - interpolation_3[j]);
                }
                return fixed_state;
            }
        }
        else if (in2_col == 5) // entropy
        {
            int col_1 = 8, col_2 = 9;
            if (in2 >= interpolation_3[col_1] && in2 <= interpolation_3[col_2])
            {
                type x = (in2 - interpolation_3[col_1]) / (interpolation_3[col_2] - interpolation_3[col_1]);
                fixed_state[in1_col] = in1;
                fixed_state[other] = interpolation_3[other];
                for (int i = 2, j = 2; i < 6; i++, j += 2)
                {
                    fixed_state[i] = interpolation_3[j] + x * (interpolation_3[j + 1] - interpolation_3[j]);
                }
                return fixed_state;
            }
        }
        else if (in2_col == 6)
        {
            fixed_state[in1_col] = in1;
            fixed_state[other] = interpolation_3[other];
            for (int i = 2, j = 2; i < 6; i++, j += 2)
            {
                fixed_state[i] = interpolation_3[j] + in2 * (interpolation_3[j + 1] - interpolation_3[j]);
            }
            return fixed_state;
        }
    }
    return fixed_state;
}

vector<type> table_3_func(int in1_col, type in1, int in2_col, type in2)
{
    int other = (0 * (in1_col == 1) + 1 * (in1_col == 0));
    vector<type> fixed_state = {-1, -1, -1, -1, -1, -1};
    vector<int> in1_ind = find_all(0, l_3 - 1, in1, table_3[in1_col]);
    bool in1_ind_found = check_vector(in1_ind);
    if (in1_ind_found) // in1 found
    {
        int in2_ind = find_first(in1_ind, in2, table_3[in2_col]);
        if (in2_ind != -1) // in2 found
        {
            for (int i = 0; i < 6; i++)
            {
                fixed_state[i] = table_3[i][in2_ind];
            }
            return fixed_state;
        }
        else // in2 not found, then interpolate
        {
            type interpolation_1[6], interpolation_2[6];
            int in2_ind_1 = lower(in1_ind, in2, table_3[in2_col]);
            int in2_ind_2 = upper(in1_ind, in2, table_3[in2_col]);
            if (in2_ind_1 != -1 && in2_ind_2 != -1)
            {
                for (int i = 0; i < 6; i++)
                {
                    interpolation_1[i] = table_3[i][in2_ind_1];
                    interpolation_2[i] = table_3[i][in2_ind_2];
                }
                for (int i = 0; i < 6; i++)
                {
                    fixed_state[i] = interpolation_1[i] + (in2 - interpolation_1[in2_col]) * (interpolation_2[i] - interpolation_1[i]) / (interpolation_2[in2_col] - interpolation_1[in2_col]);
                }
                return fixed_state;
            }
        }
    }
    else
    {
        vector<type> interpolation_1 = {-1, -1, -1, -1, -1, -1}, interpolation_2 = {-1, -1, -1, -1, -1, -1};
        int lower_int_1 = lower(0, l_3 - 1, in1, table_3[in1_col]);
        int higher_int_1 = upper(0, l_3 - 1, in1, table_3[in1_col]);
        if (lower_int_1 != -1 && higher_int_1 != -1)
        {
            vector<int> lower_vec_1 = find_all(0, lower_int_1, table_3[in1_col][lower_int_1], table_3[in1_col]);
            vector<int> higher_vec_1 = find_all(higher_int_1, l_3 - 1, table_3[in1_col][higher_int_1], table_3[in1_col]);
            if (check_vector(lower_vec_1) && check_vector(higher_vec_1))
            {
                int lower_int_2 = find_first(lower_vec_1, in2, table_3[in2_col]);
                int higher_int_2 = find_first(higher_vec_1, in2, table_3[in2_col]);
                if (lower_int_2 != -1)
                {
                    for (int i = 0; i < 6; i++)
                    {
                        interpolation_1[i] = table_3[i][lower_int_2];
                    }
                }
                else
                {
                    int lower_int_2_1 = lower(lower_vec_1[0], lower_vec_1.back(), in2, table_3[in2_col]);
                    int higher_int_2_1 = upper(lower_vec_1[0], lower_vec_1.back(), in2, table_3[in2_col]);
                    if (lower_int_2_1 != -1 && higher_int_2_1 != -1)
                    {
                        for (int i = 0; i < 6; i++)
                        {
                            interpolation_1[i] = table_3[i][lower_int_2_1] + (in2 - table_3[in2_col][lower_int_2_1]) * (table_3[i][higher_int_2_1] - table_3[i][lower_int_2_1]) / (table_3[in2_col][higher_int_2_1] - table_3[in2_col][lower_int_2_1]);
                        }
                    }
                }
                if (higher_int_2 != -1)
                {
                    for (int i = 0; i < 6; i++)
                    {
                        interpolation_2[i] = table_3[i][higher_int_2];
                    }
                }
                else
                {
                    int lower_int_2_2 = lower(higher_vec_1[0], higher_vec_1.back(), in2, table_3[in2_col]);
                    int higher_int_2_2 = upper(higher_vec_1[0], higher_vec_1.back(), in2, table_3[in2_col]);
                    if (lower_int_2_2 != -1 && higher_int_2_2 != -1)
                    {
                        for (int i = 0; i < 6; i++)
                        {
                            interpolation_2[i] = table_3[i][lower_int_2_2] + (in2 - table_3[in2_col][lower_int_2_2]) * (table_3[i][higher_int_2_2] - table_3[i][lower_int_2_2]) / (table_3[in2_col][higher_int_2_2] - table_3[in2_col][lower_int_2_2]);
                        }
                    }
                }
                if(check_vector(interpolation_1) && check_vector(interpolation_2))
                for (int i = 0; i < 6; i++)
                {
                    fixed_state[i] = interpolation_1[i] + (in1 - interpolation_1[in1_col]) * (interpolation_2[i] - interpolation_1[i]) / (interpolation_2[in1_col] - interpolation_1[in1_col]);
                }
                return fixed_state;
            }
        }
    }
    return fixed_state;
}

int lower(int a, int b, type x, vector<type> vec)
{
    int min = -1;
    for (int i = a; i <= b; i++)
    {
        if (vec[i] < x)
        {
            min = i;
        }
    }
    return min;
}

int lower(vector<int> a, type x, vector<type> vec)
{
    int min = -1;
    for (int i : a)
    {
        if (vec[i] < x)
        {
            min = i;
        }
    }
    return min;
}

int upper(int a, int b, type x, vector<type> vec)
{
    for (int i = a; i <= b; i++)
    {
        if (vec[i] > x)
        {
            return i;
        }
    }
    return -1;
}

int upper(vector<int> a, type x, vector<type> vec)
{
    for (int i : a)
    {
        if (vec[i] > x)
        {
            return i;
        }
    }
    return -1;
}

bool check_vector(vector<type> fixed_state)
{
    for (type f : fixed_state)
    {
        if (f != -1)
        {
            return true;
        }
    }
    return false;
}

bool check_vector(vector<int> fixed_state)
{
    for (type f : fixed_state)
    {
        if (f != -1)
        {
            return true;
        }
    }
    return false;
}

int find_first(int a, int b, type x, vector<type> vec)
{
    for (int i = a; i <= b; i++)
    {
        if (vec[i] == x)
        {
            return i;
        }
    }
    return -1;
}

int find_first(vector<int> a, type x, vector<type> vec)
{
    for (int i : a)
    {
        if (vec[i] == x)
        {
            return i;
        }
    }
    return -1;
}

vector<int> find_all(int a, int b, type x, vector<type> vec)
{
    vector<int> indices = {-1};
    bool first = true;
    for (int i = a; i <= b; i++)
    {
        if (vec[i] == x)
        {
            if (first)
            {
                indices.clear();
                first = false;
            }
            indices.push_back(i);
        }
    }
    return indices;
}

vector<int> find_all(vector<int> a, type x, vector<type> vec)
{
    vector<int> indices = {-1};
    bool first = true;
    for (int i : a)
    {
        if (vec[i] == x)
        {
            if (first)
            {
                indices.clear();
                first = false;
            }
            indices.push_back(i);
        }
    }
    return indices;
}
