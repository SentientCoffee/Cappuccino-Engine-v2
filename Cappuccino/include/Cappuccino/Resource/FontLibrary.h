#pragma once

#include "Cappuccino/Rendering/Text/Font.h"

namespace Capp {
	
	class FontLibrary {
	public:

		static void init();
		static void shutdown();

		static void addFont(const std::string& name, Font* font);
		static Font* loadFont(const std::string& name, const std::string& filepath);
		static Font* getFont(const std::string& name);
		
		static FT_Library getFontLibrary();
		
	private:

		static bool hasFont(const std::string& name);

		static FT_Library _fontLibrary;
		static std::unordered_map<std::string, Font*> _fonts;
		
		static bool _initialized;
		
	};

	
}
