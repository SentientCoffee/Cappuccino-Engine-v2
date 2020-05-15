#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Material.h"

#include "Cappuccino/Rendering/Textures/TextureDefaults.h"

using namespace Capp;

Material::Material() {
	setDiffuseMap(Texture2D::create(1, 1, &TextureDefaults::whiteTexture));
	setSpecularMap(Texture2D::create(1, 1, &TextureDefaults::whiteTexture));
	setEmissionMap(Texture2D::create(1, 1, &TextureDefaults::blackTexture));
	// setNormalMap(Texture2D::create(1, 1, &normalTexture));					// TODO: NORMAL MAPPING
	// setParallaxMap(Texture2D::create(1, 1, &normalTexture));					// TODO: PARALLAX MAPPING
}


Material::~Material() {
	_textures.clear();
}

void Material::setDiffuseMap (const Ref<Texture2D>& diffuse)  { _textures["diffuse"]  = diffuse;  }
void Material::setSpecularMap(const Ref<Texture2D>& specular) { _textures["specular"] = specular; }
void Material::setEmissionMap(const Ref<Texture2D>& emission) { _textures["emission"] = emission; }
void Material::setNormalMap  (const Ref<Texture2D>& normal)   { _textures["normal"]   = normal;   }
void Material::setParallaxMap(const Ref<Texture2D>& parallax) { _textures["parallax"] = parallax; }

void Material::setRoughness(const float roughness) { _roughness = glm::clamp(roughness, 0.0f, 1.0f); }

void Material::apply(const Ref<Shader>& shader) const {
	CAPP_ASSERT(shader != nullptr, "No shader bound to use with this material!");
	
	shader->bind();

	int slot = 0;
	for(const auto& t : _textures) {
		t.second->bind(slot);
		shader->setUniform<Int>("uMaterial." + t.first, slot);
		++slot;
	}

	shader->setUniform<Float>("uMaterial.roughness", _roughness);
}
