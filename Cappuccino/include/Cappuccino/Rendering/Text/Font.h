#pragma once

#include "Cappuccino/Rendering/Textures/Texture2D.h"

#include <freetype/ft2build.h>
#include <glm/glm.hpp>

#include <map>

namespace Capp {

	struct Glyph {
		Ref<Texture2D> texture = nullptr;
		glm::ivec2 size        = glm::vec2(0.0f);
		glm::ivec2 bearing     = glm::vec2(0.0f);
		long advance           = 0;
	};
	
	class Font {
	public:
		
		Font() = default;
		Font(const std::string & name, const std::string & filepath);
		~Font();

		static Ref<Font> create(const std::string& name, const std::string& filepath) {
			return Memory::createRef<Font>(name, filepath);
		}

		const std::string& getName() const { return _name; }
		const Glyph& getCharacter(unsigned char ch);

	private:

		void load(const std::string& name, const std::string& filepath);
		bool hasCharacter(unsigned char ch) const;

		std::string _name;
		std::map<unsigned char, Glyph> _glyphs;
		
	};
	
}