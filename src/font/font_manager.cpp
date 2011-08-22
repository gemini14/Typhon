// Courtesy Michael Zeilfelder of the Irrlicht community

#include "font/font_manager.h"

#include <sstream>

#include "font/gui_freetype_font.h"

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
	FontMap::iterator itFont = mFontMap.begin();
	for ( ; itFont != mFontMap.end(); ++itFont )
	{
		itFont->second->drop();
	}

	FaceMap::iterator itFace = mFaceMap.begin();
	for ( ; itFace != mFaceMap.end(); ++itFace )
	{
		itFace->second->drop();
	}
}

irr::gui::IGUIFont* FontManager::GetTtFont(irr::video::IVideoDriver* driver, const char* filename_, unsigned int size_, bool antiAlias_, bool transparency_)
{
	if ( !filename_ || !strlen(filename_) )
		return NULL;

	// Make a unique font name for the given settings.
	// We need a new font for each setting, but only a new face when loading a different fontfile
	std::string fontString(MakeFontIdentifier(filename_, size_, antiAlias_, transparency_));
	FontMap::iterator itFont = mFontMap.find(fontString);
	if  (itFont != mFontMap.end() )
		return itFont->second;

	// check if the face is already loaded
	std::string faceName(filename_);
	CGUITTFace * face = NULL;
	FaceMap::iterator itFace = mFaceMap.find(faceName);
	if  (itFace != mFaceMap.end() )
	{
		face = itFace->second;
	}

	// no face loaded
	if ( !face )
	{
		// make a new face
		face = new CGUITTFace;
		if ( !face->load(filename_) )
		{
			face->drop();
			return NULL;
		}
		mFaceMap[faceName] = face;
	}

	// access to the video driver in my application.
	CGUIFreetypeFont * font = new CGUIFreetypeFont(driver);

	font->attach(face, size_);
	font->AntiAlias = antiAlias_;
	font->Transparency = transparency_;
	mFontMap[fontString] = font;

	return font;
}

// make a unique font name for different settings.
std::string FontManager::MakeFontIdentifier(const char* filename_, unsigned int size_, bool antiAlias_, bool transparency_)
{
	std::ostringstream stream;
	stream << filename_ << size_;
	if ( antiAlias_ )
		stream << 'a';
	if ( transparency_ )
		stream << 't';

	//fprintf(stderr, "font: %s", stream.str().c_str());

	return stream.str();
}