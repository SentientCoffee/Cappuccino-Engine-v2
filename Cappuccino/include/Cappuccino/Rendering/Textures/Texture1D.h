#pragma once

#include "Cappuccino/Rendering/Textures/TextureFormats.h"
#include "Cappuccino/Rendering/Textures/TextureParams.h"

#include <string>

namespace Capp {

	class Texture1D {
	public:

		Texture1D(const std::string& filepath);
		Texture1D(unsigned size, void* data);
		~Texture1D();

		unsigned getRendererID() const;

		unsigned int getSize() const;

		void bind(unsigned slot = 0) const;
		static void unbind(unsigned slot = 0);

		void setParameters(const TextureParams& params);

	private:

		void createTexture();

		unsigned _id = 0;
		unsigned _size = 0;

		void* _data = nullptr;

		std::string _texturePath;

		TextureFormats _formats;
		TextureParams _parameters;

	};

}