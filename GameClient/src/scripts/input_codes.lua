local input_codes = {
	forward = 0x57,
	left = 0x41,
	backward = 0x53,
	right = 0x44,
	jump = 0x20,
	throwPrimary = 0x01,
	throwSecondary = 0x02
}

function GetInputCode(action)
	return input_codes[action]
end
