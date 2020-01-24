#pragma once

#include "Cappuccino/Rendering/Shader.h"
#include "Cappuccino/Rendering/Texture.h"

#include <unordered_map>

namespace Capp {

	class Material {
	public:

		Material(Shader* shader);
		~Material();

		Shader* getShader() const;
		void setShader(Shader* shader);

		void apply() const;

		void setValue(const std::string& name, bool value);
		void setValue(const std::string& name, int value);
		void setValue(const std::string& name, float value);
		
		void setValue(const std::string& name, const glm::vec2& value);
		void setValue(const std::string& name, const glm::vec3& value);
		void setValue(const std::string& name, const glm::vec4& value);
		
		void setValue(const std::string& name, const glm::mat3& value);
		void setValue(const std::string& name, const glm::mat4& value);
		
		void setValue(const std::string& name, Texture2D* value);

	private:

		Shader* _shader;

		std::unordered_map<std::string, bool> _bools;
		std::unordered_map<std::string, int> _ints;
		std::unordered_map<std::string, float> _floats;
		
		std::unordered_map<std::string, glm::vec2> _vec2s;
		std::unordered_map<std::string, glm::vec3> _vec3s;
		std::unordered_map<std::string, glm::vec4> _vec4s;
		
		std::unordered_map<std::string, glm::mat3> _mat3s;
		std::unordered_map<std::string, glm::mat4> _mat4s;

		std::unordered_map<std::string, Texture2D*> _textures;
		
	};
	
}
