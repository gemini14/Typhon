local text = {}

text.en = require('scripts/en_lang')
text.de = require('scripts/de_lang')

function GetText(lang, entry)
	return text[lang][entry]
end