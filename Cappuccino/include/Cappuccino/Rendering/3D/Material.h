#pragma once

#include "Cappuccino/Rendering/Shaders/Shader.h"
#include "Cappuccino/Rendering/Textures/Texture2D.h"

#include <unordered_map>

namespace Capp {

	class Material {

		using TextureMap = std::unordered_map<std::string, Ref<Texture2D>>;
		
	public:

		Material();
		~Material();

		static Ref<Material> create() {
			return Memory::createRef<Material>();
		}

		void setDiffuseMap(const Ref<Texture2D>& diffuse);
		void setSpecularMap(const Ref<Texture2D>& specular);
		void setEmissionMap(const Ref<Texture2D>& emission);
		void setNormalMap(const Ref<Texture2D>& normal);
		void setParallaxMap(const Ref<Texture2D>& parallax);

		void setRoughness(float roughness);

		void apply(const Ref<Shader>& shader) const;
		
	private:

		float _roughness = 1.0f;
		TextureMap _textures;
		
	};
	
}
