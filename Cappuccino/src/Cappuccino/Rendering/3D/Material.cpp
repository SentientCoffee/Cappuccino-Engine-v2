#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Material.h"

#include "Cappuccino/Rendering/Textures/TextureDefaults.h"

using namespace Capp;

Material::Material() {
	setValue("diffuseMap", new Texture2D(1, 1, &whiteTexture));
	setValue("specularMap", new Texture2D(1, 1, &whiteTexture));
	setValue("emissionMap", new Texture2D(1, 1, &blackTexture));
	//setValue("normalMap", new Texture2D(1, 1, &normalTexture));					// TODO: NORMAL MAPPING
	//setValue("bumpMap", new Texture2D(1, 1, &normalTexture));						// TODO: PARALLAX MAPPING
}

Material::Material(Shader* shader) :
	_shader(shader) {}

Material::~Material() {
	for(const auto& t : _textures) {
		delete t.second;
	}
}

Shader* Material::getShader() const { return _shader; }
void Material::setShader(Shader* shader) { _shader = shader; }

void Material::apply() const {
	CAPP_ASSERT(_shader != nullptr, "No shader bound to this material!");
	
	_shader->bind();
	
	for(const auto& b : _bools) {
		_shader->setUniform<Bool>("uMaterial." + b.first, b.second);
	}
	for(const auto& i : _ints) {
		_shader->setUniform<Int>("uMaterial." + i.first, i.second);
	}
	for(const auto& f : _floats) {
		_shader->setUniform<Float>("uMaterial." + f.first, f.second);
	}
	for(const auto& v2 : _vec2s) {
		_shader->setUniform<Vec2>("uMaterial." + v2.first, v2.second);
	}
	for(const auto& v3 : _vec3s) {
		_shader->setUniform<Vec3>("uMaterial." + v3.first, v3.second);
	}
	for(const auto& v4 : _vec4s) {
		_shader->setUniform<Vec4>("uMaterial." + v4.first, v4.second);
	}
	for(const auto& m3 : _mat3s) {
		_shader->setUniform<Mat3>("uMaterial." + m3.first, m3.second);
	}
	for(const auto& m4 : _mat4s) {
		_shader->setUniform<Mat4>("uMaterial." + m4.first, m4.second);
	}

	int slot = 0;
	for(const auto& t : _textures) {
		t.second->bind(slot);
		_shader->setUniform<Int>("uMaterial." + t.first, slot);
		++slot;
	}
}

void Material::setValue(const std::string& name, const bool value) { _bools[name] = value; }
void Material::setValue(const std::string& name, const int value) { _ints[name] = value; }
void Material::setValue(const std::string& name, const float value) { _floats[name] = value; }

void Material::setValue(const std::string& name, const glm::vec2& value) { _vec2s[name] = value; }
void Material::setValue(const std::string& name, const glm::vec3& value) { _vec3s[name] = value; }
void Material::setValue(const std::string& name, const glm::vec4& value) { _vec4s[name] = value; }

void Material::setValue(const std::string& name, const glm::mat3& value) { _mat3s[name] = value; }
void Material::setValue(const std::string& name, const glm::mat4& value) { _mat4s[name] = value; }

void Material::setValue(const std::string& name, Texture2D* value) { _textures[name] = value; }
