#pragma once

#include "Cappuccino/Rendering/Text/Font.h"
#include <freetype/freetype.h>

namespace Capp {
	
	class FontLibrary {

		using FontMap = std::unordered_map<std::string, Ref<Font>>;
		
	public:

		static void init();
		static void shutdown();

		static void addFont(const std::string& name, const Ref<Font>& font);
		static Ref<Font> loadFont(const std::string& name, const std::string& filepath);
		static Ref<Font> getFont(const std::string& name);
		
		static FT_Library getFontLibrary() { return _fontLibrary; }
		
	private:

		static bool hasFont(const std::string& name);

		static FT_Library _fontLibrary;
		
		static FontMap _fonts;
		static bool _initialized;
		
	};

	
}
