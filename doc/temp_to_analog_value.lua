#! /usr/bin/env lua

-- photon analog pins can handle a max of 3.3v
-- arduino analog pins can handle a max of 5.0v
-- v_in = 3.3
v_in = 5.0
r_25 = 5000

A = -14.141963
B = 4430.783
C = -34078.983
D = -8894192.9

function rt(celcius)
	local kelvin = celcius + 273
	return math.exp(A + B / kelvin + C / math.pow(kelvin, 2) + D / math.pow(kelvin, 3)) * r_25
end

function v_out(celcius, r1)
	local r2 = rt(celcius)
	return r2 / (r2 + r1) * v_in
end

for i=0,50,5 do
	local step = v_out(i, 10000) / 0.0049
	print("temp: " .. i .. "C")
	print("step: " .. step)
end
