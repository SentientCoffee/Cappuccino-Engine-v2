#pragma once

#include "Cappuccino/Rendering/Textures/TextureFormats.h"
#include "Cappuccino/Rendering/Textures/TextureParams.h"

#include <string>

namespace Capp {

	class Texture3D {
	public:
		
		Texture3D(const std::string& filepath);
		Texture3D(unsigned width, unsigned height, unsigned depth, void* data);
		~Texture3D();

		unsigned getRendererID() const;
		
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		unsigned int getDepth() const;
		glm::vec3 getSize() const;

		
		void bind(unsigned slot = 0) const;
		static void unbind(unsigned slot = 0);

		void setParameters(const TextureParams& params);

	private:

		void createTexture();

		unsigned _id = 0;
		unsigned _width = 0, _height = 0, _depth = 0;

		void* _data = nullptr;

		std::string _texturePath;

		TextureFormats _formats;
		TextureParams _parameters;

	};
	
}