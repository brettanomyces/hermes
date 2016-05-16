#!/usr/bin/env lua

-- see resistance_differentiation.jpeg for working
t_max = 30
t_min = 0

r_25 = 5000

A = -14.141963
B = 4430.783
C = -34078.983
D = -8894192.9

function rt(celcius)
  local kelvin = celcius + 273
  return math.exp(A + B / kelvin + C / math.pow(kelvin, 2) + D / math.pow(kelvin, 3)) * r_25
end

print(math.sqrt(((rt(t_min) * math.pow(rt(t_max), 2)) - (rt(t_max) * math.pow(rt(t_min), 2))) / (rt(t_max) - rt(t_min))))
