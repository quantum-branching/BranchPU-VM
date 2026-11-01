local function count(str, substr)
	local _, result = string.gsub(str, substr, substr)
	return result
end

local function intToBinary(n, bits)
	if (not bits) then return "" end
	if (not n) then return "0"*bits end
    
    local binary = ""

	n = n % (2 ^ bits)

    while n > 0 do
        local remainder = n % 2
        binary = math.floor(remainder) .. binary
        n = math.floor(n / 2)
		bits = bits - 1
    end

	while bits > 0 do
		binary = "0" .. binary
		bits = bits - 1
	end

    return binary
end

function binaryToBytes(binary)
	local result = ""
	local str_location = 0
	local str_length = string.len(binary)
	
	while str_location <= str_length - 1 do
		local next_line = (string.find(binary, "\n", str_location) or (str_length + 1))

		print(string.sub(binary, str_location, next_line - 1))

		number = tonumber(string.sub(binary, str_location, next_line - 1), 2)

		if number ~= nil then
			byte1 = math.floor(number / 256)
			byte2 = number % 256
			result = result .. string.char(byte1, byte2)
			end

		str_location = next_line + 1
	end

	return result
end

local function replaceLine(line, definition, alias)
	local result = ""
	local c, a, b = string.match("\t" .. line, alias)

	if c == nil then return line end

	result = result.gsub(definition, "A+", intToBinary(a, count(definition, "A")))
	result = result.gsub(result, "B+", intToBinary(b, count(definition, "B")))
	result = result.gsub(result, "C+", intToBinary(c, count(definition, "C")))
	
	return result
end

function replaceString(str, definition, alias)	
	local result = ""
	local str_location = 0
	local str_length = string.len(str)

	while str_location <= str_length - 1 do
		local next_line = (string.find(str, "\n", str_location) or (str_length + 1))
		result = result .. "\n" .. replaceLine(string.sub(str, str_location, next_line - 1), definition, alias)
		str_location = next_line + 1
	end

	return result
end
