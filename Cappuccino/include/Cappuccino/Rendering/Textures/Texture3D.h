#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Rendering/Textures/TextureFormats.h"
#include "Cappuccino/Rendering/Textures/TextureParams.h"

namespace Capp {

	class Texture3D {
	public:
		
		Texture3D(unsigned width, unsigned height, unsigned depth, void* data, InternalFormat format = InternalFormat::RGB8);
		Texture3D(unsigned width, unsigned height, unsigned depth, InternalFormat format = InternalFormat::RGB8);
		Texture3D(const std::string& filepath);
		~Texture3D();

		static Ref<Texture3D> create(unsigned width, unsigned height, unsigned depth, void* data, InternalFormat format = InternalFormat::RGB8) {
			return Memory::createRef<Texture3D>(width, height, depth, data, format);
		}

		static Ref<Texture3D> create(unsigned width, unsigned height, unsigned depth, InternalFormat format = InternalFormat::RGB8) {
			return Memory::createRef<Texture3D>(width, height, depth, format);
		}

		static Ref<Texture3D> create(const std::string& filepath) {
			return Memory::createRef<Texture3D>(filepath);
		}

		unsigned getRendererID() const;
		
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		unsigned int getDepth() const;
		glm::vec3 getSize() const;

		
		void bind(unsigned slot) const;
		static void unbind(unsigned slot);

		void setParameters(const TextureParams& params);

	private:

		unsigned _id = 0;
		unsigned _width = 0, _height = 0, _depth = 0;

		TextureFormats _formats;
		TextureParams _parameters;

	};
	
}