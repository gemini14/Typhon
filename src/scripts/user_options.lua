local options = {}
local userSaveFile = 'scripts/user_data.lua'

function GetUserData()
	local handle, errorString = io.open(userSaveFile)
	if handle then
		for str in handle:lines() do
			for k, v in string.gmatch(str, '(%w+)%s*=%s*(%w+)') do
				if k and v then
					options[k] = v
				end
			end
		end
		handle:close()
	else
		io.write(errorString)
	end
end

function GetOption(key)
	return options[key]
end

function SetOption(key, val)
	options[key] = val
end

function SaveUserData()
	local handle = io.open(userSaveFile, 'w')
	if handle then
		for k, v in pairs(options) do
			handle:write(k, ' = ', v, '\n')
		end
		handle:close()
	end
end