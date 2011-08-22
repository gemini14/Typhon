local text = {}

text.en = require('scripts/en_lang')
text.de = require('scripts/de_lang')

local numberOfLanguages = 2

function GetNumberOfLanguages()
	return numberOfLanguages
end

function GetText(lang, entry)
	return text[lang][entry]
end