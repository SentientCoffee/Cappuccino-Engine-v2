#include "CappPCH.h"
#include "Cappuccino/Resource/FontLibrary.h"

using namespace Capp;

FT_Library FontLibrary::_fontLibrary;
FontLibrary::FontMap FontLibrary::_fonts;

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
	_fonts.clear();
	FT_Done_FreeType(_fontLibrary);
}

void FontLibrary::addFont(const std::string& name, const Ref<Font>& font) {
	if(hasFont(name)) {
		CAPP_ASSERT(!hasFont(name), "Font \"{0}\" already exists!", name);
		return;
	}

	_fonts[name] = font;
}

Ref<Font> FontLibrary::loadFont(const std::string& name, const std::string& filepath) {
	if(hasFont(name)) {
		CAPP_PRINT_INFO("Font \"{0}\" already exists, using loaded font...", name);
		return _fonts[name];
	}
	
	return _fonts[name] = Font::create(name, filepath);
}

Ref<Font> FontLibrary::getFont(const std::string& name) {
	if(!hasFont(name)) {
		CAPP_ASSERT(hasFont(name), "Font \"{0}\" has not been loaded!", name);
		return nullptr;
	}

	return _fonts[name];
}

bool FontLibrary::hasFont(const std::string& name) { return _fonts.find(name) != _fonts.end(); }
