#! /usr/bin/env lua

-- photon analog pins can handle a max of 3.3v
v_in = 3.3
r_25 = 5000
t_min = 0
t_max = 30
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
	-- max voltage is at minimum temperature
	local v_max = v_out(t_min, j)
	local v_min = v_out(t_max, j)
	local diff = v_max - v_min
	if diff > range then
		range = diff
		ohm = j
	end
end

print("range: " .. range)
print("ohms: " .. ohm)


