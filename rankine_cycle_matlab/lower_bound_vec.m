function retval = lower_bound (vec, num)

for i=1:length(vec)
  if(vec(i)>num)
    retval = find(vec == vec(i-1));
    return;
  end
end

retval = 0;
end
