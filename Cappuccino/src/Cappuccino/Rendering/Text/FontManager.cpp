#include "CappPCH.h"
#include "Cappuccino/Rendering/Text/FontManager.h"

using namespace Capp;

FT_Library FontManager::_fontLibrary;
std::unordered_map<std::string, Font*> FontManager::_fonts;

bool FontManager::_initialized = false;

void FontManager::init() {
	if(_initialized) {
		return;
	}
	
	const int freetypeStatus = FT_Init_FreeType(&_fontLibrary);
	CAPP_ASSERT(freetypeStatus == 0, "Failed to initialize FreeType!");
	_initialized = true;
}

void FontManager::shutdown() {
	for(auto& font : _fonts) {
		delete font.second;
	}
	
	FT_Done_FreeType(_fontLibrary);
}

void FontManager::loadFont(const std::string& name, const std::string& filepath) {
	const auto font = new Font(name, filepath);
	_fonts[name] = font;
}

Font* FontManager::getFont(const std::string& name) {
	if(!hasFont(name)) {
		CAPP_PRINT_ERROR("Font: {0}", name);
		CAPP_ASSERT(hasFont(name), "Font has not been loaded!");
	}

	return _fonts[name];
}

bool FontManager::hasFont(const std::string& name) { return _fonts.find(name) != _fonts.end(); }

FT_Library FontManager::getFontLibrary() { return _fontLibrary; }
