#include "CappPCH.h"
#include "Cappuccino/Rendering/Text/Font.h"
#include "Cappuccino/Resource/FontLibrary.h"

#include <freetype/freetype.h>
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
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);     // Disable byte-alignment restriction

	// Only loading in the first 128 characters of the font
	for(uint8_t ch = 0; ch < 128; ++ch) {
		const int ftLoadCharStatus = FT_Load_Char(typeface, ch, FT_LOAD_RENDER);
		CAPP_ASSERT(ftLoadCharStatus == 0, "Failed to load glyph!\n\tGlyph: {0}", ch);

		Ref<Texture2D> texture = nullptr;
		if(typeface->glyph->bitmap.buffer != nullptr) {
			texture = Texture2D::create(typeface->glyph->bitmap.width, typeface->glyph->bitmap.rows, typeface->glyph->bitmap.buffer, InternalFormat::Red8);
			TextureParams params = { WrapMode::ClampToEdge, MinFilter::Linear, MagFilter::Linear };
			texture->setParameters(params);
		}
		
		const Glyph glyph = {
			texture,
			{ typeface->glyph->bitmap.width, typeface->glyph->bitmap.rows },
			{ typeface->glyph->bitmap_left, typeface->glyph->bitmap_top },
			static_cast<uint64_t>(typeface->glyph->advance.x)
		};

		_glyphs[ch] = glyph;
	}

	FT_Done_Face(typeface);
}

const Glyph& Font::getCharacter(const uint8_t ch) {
	if(!hasCharacter(ch)) {
		CAPP_PRINT_WARNING("Font \"{0}\" does not have glyph '{1}'", _name, ch);
	}

	return _glyphs[ch];
}
bool Font::hasCharacter(const uint8_t ch) const { return _glyphs.find(ch) != _glyphs.end(); }
