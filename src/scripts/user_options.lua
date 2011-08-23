local options = {
	playerName = 'Player 1',
	chosenLanguage = 'de'
}

function GetOption(key)
	return options[key]
end

function SetOption(key, val)
	options[key] = val
end
