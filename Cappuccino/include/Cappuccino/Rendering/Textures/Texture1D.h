#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Rendering/Textures/TextureFormats.h"
#include "Cappuccino/Rendering/Textures/TextureParams.h"

#include <string>

namespace Capp {

	class Texture1D {
	public:

		Texture1D(unsigned size, void* data, InternalFormat format = InternalFormat::RGB8);
		Texture1D(unsigned size, InternalFormat format = InternalFormat::RGB8);
		Texture1D(const std::string& filepath);
		~Texture1D();

		static Ref<Texture1D> create(unsigned size, void* data, InternalFormat format = InternalFormat::RGBA8) {
			return Memory::createRef<Texture1D>(size, data, format);
		}

		static Ref<Texture1D> create(unsigned size, InternalFormat format = InternalFormat::RGBA8) {
			return Memory::createRef<Texture1D>(size, format);
		}

		static Ref<Texture1D> create(const std::string& filepath) {
			return Memory::createRef<Texture1D>(filepath);
		}

		unsigned getRendererID() const { return _id; }
		unsigned getSize() const { return _size; }

		void bind(unsigned slot) const;
		static void unbind(unsigned slot);

		void setData(void* data, unsigned size);
		void setParameters(const TextureParams& params);

	private:
		
		unsigned _id = 0;
		unsigned _size = 0;

		TextureFormats _formats;
		TextureParams _parameters;

	};

}