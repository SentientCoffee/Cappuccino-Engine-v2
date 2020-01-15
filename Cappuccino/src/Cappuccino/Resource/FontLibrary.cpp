#include "CappPCH.h"
#include "Cappuccino/Resource/FontLibrary.h"

using namespace Capp;

FT_Library FontLibrary::_fontLibrary;
std::unordered_map<std::string, Font*> FontLibrary::_fonts;

bool FontLibrary::_initialized = false;

void FontLibrary::init() {
	if(_initialized) {
		CAPP_ASSERT(!_initialized, "Font library already initialized!");
		return;
	}
	
	const int freetypeStatus = FT_Init_FreeType(&_fontLibrary);
	CAPP_ASSERT(freetypeStatus == 0, "Failed to initialize FreeType!");
	_initialized = true;
}

void FontLibrary::shutdown() {
	for(auto& font : _fonts) {
		delete font.second;
	}
	
	FT_Done_FreeType(_fontLibrary);
}

void FontLibrary::addFont(const std::string& name, Font* font) {
	if(hasFont(name)) {
		CAPP_PRINT_ERROR("Font name: {0}", name);
		CAPP_ASSERT(!hasFont(name), "Font already exists!");
		return;
	}

	_fonts[name] = font;
}

Font* FontLibrary::loadFont(const std::string& name, const std::string& filepath) {
	if(hasFont(name)) {
		CAPP_PRINT_ERROR("Font name: {0}", name);
		CAPP_ASSERT(!hasFont(name), "Font already exists!");
		return _fonts[name];
	}
	
	return _fonts[name] = new Font(name, filepath);
}

Font* FontLibrary::getFont(const std::string& name) {
	if(!hasFont(name)) {
		CAPP_PRINT_ERROR("Font: {0}", name);
		CAPP_ASSERT(hasFont(name), "Font has not been loaded!");
		return nullptr;
	}

	return _fonts[name];
}

bool FontLibrary::hasFont(const std::string& name) { return _fonts.find(name) != _fonts.end(); }

FT_Library FontLibrary::getFontLibrary() { return _fontLibrary; }
