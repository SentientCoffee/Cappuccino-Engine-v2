#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Rendering/Textures/TextureFormats.h"
#include "Cappuccino/Rendering/Textures/TextureParams.h"

#include <string>

namespace Capp {
	
	class Texture2D {
	public:
		
		Texture2D() = default;
		Texture2D(unsigned width, unsigned height, void* data, InternalFormat format = InternalFormat::RGBA8, Mipmaps enableMipmaps = Mipmaps::On);
		Texture2D(unsigned width, unsigned height, InternalFormat format = InternalFormat::RGBA8, Mipmaps enableMipmaps = Mipmaps::On);
		Texture2D(const std::string & filepath, Mipmaps enableMipmaps = Mipmaps::On);
		~Texture2D();

		static Ref<Texture2D> create(unsigned width, unsigned height, void* data, InternalFormat format = InternalFormat::RGBA8, Mipmaps enableMipmaps = Mipmaps::On) {
			return Memory::createRef<Texture2D>(width, height, data, format, enableMipmaps);
		}
		
		static Ref<Texture2D> create(unsigned width, unsigned height, InternalFormat format = InternalFormat::RGBA8, Mipmaps enableMipmaps = Mipmaps::On) {
			return Memory::createRef<Texture2D>(width, height, format, enableMipmaps);
		}
		
		static Ref<Texture2D> create(const std::string& filepath, Mipmaps enableMipmaps = Mipmaps::On) {
			return Memory::createRef<Texture2D>(filepath, enableMipmaps);
		}
		
		unsigned getRendererID() const;
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		glm::vec2 getSize() const;


		void bind(unsigned slot) const;
		static void unbind(unsigned slot);

		void setData(void* data, unsigned size);
		void setParameters(const TextureParams& params);

	private:
		
		unsigned _id = 0;
		unsigned _width = 0, _height = 0;
		unsigned _mipLevels = 0;

		TextureFormats _formats;
		TextureParams _parameters;
	};
	
}
