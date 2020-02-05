#pragma once

#include "Cappuccino/Rendering/Textures/Texture2D.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <glm/glm.hpp>

#include <map>

namespace Capp {

	struct Glyph {
		Texture2D* texture;
		glm::ivec2 size;
		glm::ivec2 bearing;
		long advance;
	};
	
	class Font {
	public:

		Font() = default;
		Font(const std::string& name, const std::string& filepath);
		~Font();

		void load(const std::string& name, const std::string& filepath);

		const std::string& getName() const;
		const Glyph& getCharacter(unsigned char ch);

	private:

		bool hasCharacter(unsigned char ch) const;


		std::string _name = "";
		std::map<unsigned char, Glyph> _glyphs;
		
	};
	
}