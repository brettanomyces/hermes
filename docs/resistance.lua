#! /usr/bin/env lua

-- photon analog pins can handle a max of 3.3v
v_in = 3.3
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

ohms = 0
range = 0
-- resistance 1-100000Ohm
for j=1,1000000 do
	local v_0 = v_out(0, j)
	local v_30 = v_out(30, j)
	local diff = v_0 - v_30
	if diff > range then
		range = diff
		ohm = j
	end
end

print("range: " .. range)
print("ohms: " .. ohm)


