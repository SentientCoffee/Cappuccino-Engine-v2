#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Rendering/Textures/TextureFormats.h"
#include "Cappuccino/Rendering/Textures/TextureParams.h"

#include <string>

namespace Capp {

	class Texture1D {
	public:

		Texture1D(uint32_t size, void* data, InternalFormat format = InternalFormat::RGB8);
		Texture1D(uint32_t size, InternalFormat format = InternalFormat::RGB8);
		Texture1D(const std::string& filepath);
		~Texture1D();

		static Ref<Texture1D> create(uint32_t size, void* data, InternalFormat format = InternalFormat::RGBA8) {
			return Memory::createRef<Texture1D>(size, data, format);
		}

		static Ref<Texture1D> create(uint32_t size, InternalFormat format = InternalFormat::RGBA8) {
			return Memory::createRef<Texture1D>(size, format);
		}

		static Ref<Texture1D> create(const std::string& filepath) {
			return Memory::createRef<Texture1D>(filepath);
		}

		uint32_t getRendererID() const { return _id; }
		uint32_t getSize() const { return _size; }

		void bind(uint32_t slot) const;
		static void unbind(uint32_t slot);

		void setData(void* data, uint32_t size);
		void setParameters(const TextureParams& params);

	private:
		
		uint32_t _id = 0;
		uint32_t _size = 0;

		TextureFormats _formats;
		TextureParams _parameters;

	};

}