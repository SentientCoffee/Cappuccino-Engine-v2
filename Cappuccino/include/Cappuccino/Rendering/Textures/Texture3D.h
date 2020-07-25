#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Rendering/Textures/TextureFormats.h"
#include "Cappuccino/Rendering/Textures/TextureParams.h"

namespace Capp {

	class Texture3D {
	public:
		
		Texture3D(uint32_t width, uint32_t height, uint32_t depth, void* data, InternalFormat format = InternalFormat::RGB8);
		Texture3D(uint32_t width, uint32_t height, uint32_t depth, InternalFormat format = InternalFormat::RGB8);
		Texture3D(const std::string& filepath);
		~Texture3D();

		static Ref<Texture3D> create(uint32_t width, uint32_t height, uint32_t depth, void* data, InternalFormat format = InternalFormat::RGB8) {
			return Memory::createRef<Texture3D>(width, height, depth, data, format);
		}

		static Ref<Texture3D> create(uint32_t width, uint32_t height, uint32_t depth, InternalFormat format = InternalFormat::RGB8) {
			return Memory::createRef<Texture3D>(width, height, depth, format);
		}

		static Ref<Texture3D> create(const std::string& filepath) {
			return Memory::createRef<Texture3D>(filepath);
		}

		uint32_t getRendererID() const { return _id; }
		
		uint32_t getWidth() const { return _width; }
		uint32_t getHeight() const { return _height; }
		uint32_t getDepth() const { return _depth; }
		glm::vec3 getSize() const { return { _width, _height, _depth }; }

		
		void bind(uint32_t slot) const;
		static void unbind(uint32_t slot);

		void setParameters(const TextureParams& params);

	private:

		uint32_t _id = 0;
		uint32_t _width = 0, _height = 0, _depth = 0;

		TextureFormats _formats;
		TextureParams _parameters;

	};
	
}