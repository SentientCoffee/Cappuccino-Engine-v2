#pragma once

#include "Cappuccino/Rendering/TextureFormats.h"
#include "Cappuccino/Rendering/TextureParams.h"

#include <string>

namespace Capp {
	
	class Texture2D {
	public:

		Texture2D(unsigned width, unsigned height, void* data = nullptr, unsigned channels = 4);
		Texture2D(const std::string& filepath);
		~Texture2D();

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		unsigned getRendererID() const;

		void bind(unsigned slot = 0) const;
		static void unbind(unsigned slot = 0);

		void setData(void* data, unsigned size);
		
		void setParameters(const TextureParams& params);

	private:

		void createTexture();

		unsigned _id = 0;
		unsigned _width = 0, _height = 0;
		unsigned _mipLevels = 1;
		unsigned char* _imageData = nullptr;
		
		std::string _texturePath;

		TextureFormats _formats;
		TextureParams _parameters;
		
	};
	
}
