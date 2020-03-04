#include "CappPCH.h"
#include "Cappuccino/Rendering/Text/Font.h"
#include "Cappuccino/Resource/FontLibrary.h"

#include <glad/glad.h>

using namespace Capp;

Font::Font(const std::string& name, const std::string& filepath) {
	load(name, filepath);
}

Font::~Font() {	
	_glyphs.clear();
}

void Font::load(const std::string& name, const std::string& filepath) {
	_name = name;
	FT_Face typeface;

	const int ftFontFileLoadStatus = FT_New_Face(FontLibrary::getFontLibrary(), filepath.c_str(), 0, &typeface);
	CAPP_ASSERT(ftFontFileLoadStatus == 0, "Failed to load font file!\nFont file: {0}", filepath);

	FT_Set_Pixel_Sizes(typeface, 0, 48);       // Set size to load glyphs as
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                           // Disable byte-alignment restriction

	// Only loading in the first 128 characters of the font
	for(unsigned char ch = 0; ch < 128; ++ch) {
		const int ftLoadCharStatus = FT_Load_Char(typeface, ch, FT_LOAD_RENDER);
		CAPP_ASSERT(ftLoadCharStatus == 0, "Failed to load glyph!\nGlyph: {0}", ch);
		
		auto texture = new Texture2D(typeface->glyph->bitmap.width, typeface->glyph->bitmap.rows, typeface->glyph->bitmap.buffer, InternalFormat::Red8);
		TextureParams params = { WrapMode::ClampToEdge, MinFilter::Linear, MagFilter::Linear };
		texture->setParameters(params);

		const Glyph glyph = {
			texture,
			{ typeface->glyph->bitmap.width, typeface->glyph->bitmap.rows },
			{ typeface->glyph->bitmap_left, typeface->glyph->bitmap_top },
			typeface->glyph->advance.x
		};

		_glyphs[ch] = glyph;
	}

	FT_Done_Face(typeface);
}

const std::string& Font::getName() const { return _name; }

const Glyph& Font::getCharacter(const unsigned char ch) {
	if(!hasCharacter(ch)) {
		CAPP_PRINT_WARNING("Font {0} does not have glyph {1}", _name, ch);
	}

	return _glyphs[ch];
}
bool Font::hasCharacter(const unsigned char ch) const { return _glyphs.find(ch) != _glyphs.end(); }
