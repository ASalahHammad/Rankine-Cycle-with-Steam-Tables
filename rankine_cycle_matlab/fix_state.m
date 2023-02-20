function [fixed_state] = fix_state(in1_col, in1, in2_col, in2, fluid)

persistent sat_table table_3
if nargin==5
  if (fluid == 1) % Water
    fprintf('loading steam tables...\n');
    sat_table = importdata('steam_tables/sat_table.csv'); % saturation table
    sat_table = sat_table.data;
    table_3 = importdata('steam_tables/table_3.csv'); % superheated and compressed table
    table_3 = table_3.data(:,1:6);
  elseif (fluid == 2) % R134
    fprintf('loading R134 tables...');
    sat_table = importdata('R134_tables/sat_table.csv'); % saturation table
    sat_table = sat_table.data;
    error("The superheated R134 tables are not ready...");
  end
  return;
end

if(in1_col ~= 1)
  error("This program doesn't yet know how to fix a state without pressure as the first input to the \"fix_state\" script");
end

in1_ind = find(sat_table(:,in1_col) == in1);
if(in1_ind)
  saturation_1 = sat_table(in1_ind,:);
else
  for i = 1:length(sat_table)
    if(sat_table(i,in1_col)>in1)
      if(i == 1)
        error("Can't interpolate because number is too small");
      end
        saturation_1 = sat_table(i-1,:) + (in1-sat_table(i-1,in1_col))*(sat_table(i,:)-sat_table(i-1,:))/(sat_table(i,in1_col)-sat_table(i-1,in1_col));
      break;
    end
  end
end

if (in2_col == 2) % Temperature
  if (in2 == saturation_1(2))
    error("Can't fix the state because the two inputs are dependent");
  end
elseif (in2_col == 7) % x
  fixed_state = saturation_1(1:2);
  fixed_state(3:6) = saturation_1(3:2:10) + in2*(saturation_1((3:2:10)+1) - saturation_1(3:2:10));
  return;
else
  if (in2_col == 3), col=3; elseif(in2_col == 4), col=5; elseif(in2_col == 5), col=7; elseif(in2_col == 6), col=9; end;
  if(saturation_1(col)<=in2 && saturation_1(col+1)>=in2) % if 2nd input lies in the saturation region corresponding to in1
   x = (in2-saturation_1(col))/(saturation_1(col+1)-saturation_1(col))
   fixed_state = saturation_1(1:2);
   fixed_state(3:6) = saturation_1(3:2:10) + x*(saturation_1((3:2:10)+1)-saturation_1(3:2:10));
   return;
  end
end

%% ===================== Table 3 =====================
clear in1_ind in2_ind
in1_ind = find(table_3(:,in1_col) == in1);
if(in1_ind) % in1 found
  in2_ind = find(table_3(in1_ind,in2_col) == in2);
  if(in2_ind) % in2 found
    fixed_state = table_3(in1_ind+in2_ind-1,:);
  else % in2 not found
    for i = in1_ind'
      if(table_3(i,in2_col)>in2)
        if(i == 1)
          error("Can't interpolate because number is too small, extrapolation hasn't yet been implemented");
        end
          fixed_state = table_3(i-1,:) + (in2-table_3(i-1,in2_col))*(table_3(i,:)-table_3(i-1,:))/(table_3(i,in2_col)-table_3(i-1,in2_col));
        return;
      end
    end
  end
else % in1 not found
  in1_lower = lower_bound_vec(table_3(:,in1_col), in1);
  in1_upper = upper_bound_vec(table_3(:,in1_col), in1);
  if (in1_lower & in1_upper)
    in2_ind_in1_lower = find(table_3(in1_lower,in2_col) == in2);
    if (in2_ind_in1_lower) % in2 found
      interpolation_1 = table_3(in1_lower+in2_ind_in1_lower-1,:);
    else % in2 not found, then interpolate for in2
      in2_lower_in1_lower = lower_bound(table_3(in1_lower,in2_col), in2);
      in2_upper_in1_lower = upper_bound(table_3(in1_lower,in2_col), in2);
      if (in2_lower_in1_lower & in2_upper_in1_lower)
        lower_ind = in1_lower(1) + in2_lower_in1_lower - 1;
        upper_ind = in1_lower(1) + in2_upper_in1_lower - 1;
        interpolation_1 = table_3(lower_ind,:) + (in2-table_3(lower_ind,in2_col)) * (table_3(upper_ind,:) - table_3(lower_ind,:)) / (table_3(upper_ind,in2_col) - table_3(lower_ind,in2_col));
      else % input 2 is out of range of tables
        error("Input is either too small or too large, extrapolation hasn't yet been implemented");
      end
    end

    in2_ind_in1_upper = find(table_3(in1_upper,in2_col) == in2);
    if (in2_ind_in1_upper) % in2 found
      interpolation_2 = table_3(in1_upper+in2_ind_in1_upper-1,:);
    else % in2 not found, then interpolate for in2
      in2_lower_in1_upper = lower_bound(table_3(in1_upper,in2_col), in2);
      in2_upper_in1_upper = upper_bound(table_3(in1_upper,in2_col), in2);
      if (in2_lower_in1_upper & in2_upper_in1_upper)
        lower_ind = in1_upper(1) + in2_lower_in1_upper - 1;
        upper_ind = in1_upper(1) + in2_upper_in1_upper - 1;
        interpolation_2 = table_3(lower_ind,:) + (in2-table_3(lower_ind,in2_col)) * (table_3(upper_ind,:) - table_3(lower_ind,:)) / (table_3(upper_ind,in2_col) - table_3(lower_ind,in2_col));
      else % input 2 is out of range of tables
        error("Input is either too small or too large, extrapolation hasn't yet been implemented");
      end
      fixed_state = interpolation_1 + (in1 - interpolation_1(in1_col)) * (interpolation_2 - interpolation_1) / (interpolation_2(in1_col) - interpolation_1(in1_col));
      return;
    end

  else % input 1 is out of range of tables
    error("Input is either too small or too large, extrapolation hasn't yet been implemented");
  end
end

%% Determine the phase of the fluid
% Write some code that does some stuff here

if(~exist('fixed_state','var'))
  error("Error fixing the state");
end

end % endfunction

