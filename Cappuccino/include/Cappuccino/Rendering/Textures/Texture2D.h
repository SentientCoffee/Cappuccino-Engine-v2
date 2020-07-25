#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Rendering/Textures/TextureFormats.h"
#include "Cappuccino/Rendering/Textures/TextureParams.h"

#include <string>

namespace Capp {
	
	class Texture2D {
	public:
		
		Texture2D() = default;
		Texture2D(uint32_t width, uint32_t height, void* data, InternalFormat format = InternalFormat::RGBA8, Mipmaps enableMipmaps = Mipmaps::On);
		Texture2D(uint32_t width, uint32_t height, InternalFormat format = InternalFormat::RGBA8, Mipmaps enableMipmaps = Mipmaps::On);
		Texture2D(const std::string & filepath, Mipmaps enableMipmaps = Mipmaps::On);
		~Texture2D();

		static Ref<Texture2D> create(uint32_t width, uint32_t height, void* data, InternalFormat format = InternalFormat::RGBA8, Mipmaps enableMipmaps = Mipmaps::On) {
			return Memory::createRef<Texture2D>(width, height, data, format, enableMipmaps);
		}
		
		static Ref<Texture2D> create(uint32_t width, uint32_t height, InternalFormat format = InternalFormat::RGBA8, Mipmaps enableMipmaps = Mipmaps::On) {
			return Memory::createRef<Texture2D>(width, height, format, enableMipmaps);
		}
		
		static Ref<Texture2D> create(const std::string& filepath, Mipmaps enableMipmaps = Mipmaps::On) {
			return Memory::createRef<Texture2D>(filepath, enableMipmaps);
		}
		
		uint32_t getRendererID() const { return _id; }
		uint32_t getWidth() const { return _width; }
		uint32_t getHeight() const { return _height; }
		glm::vec2 getSize() const { return { _width, _height }; }


		void bind(uint32_t slot) const;
		static void unbind(uint32_t slot);

		void setData(void* data, uint32_t size);
		void setParameters(const TextureParams& params);

	private:
		
		uint32_t _id = 0;
		uint32_t _width = 0, _height = 0;
		uint32_t _mipLevels = 0;

		TextureFormats _formats;
		TextureParams _parameters;
	};
	
}
