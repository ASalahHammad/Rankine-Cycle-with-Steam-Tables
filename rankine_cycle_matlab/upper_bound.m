function retval = upper_bound (vec, num)

for i = 1:length(vec)
  if (vec(i) > num)
    retval = i;
    return;
  end
end

retval = 0;
end
