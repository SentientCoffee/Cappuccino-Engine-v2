#pragma once

#include "Cappuccino/Objects/Transform.h"
#include "Cappuccino/Rendering/Buffers/Framebuffer.h"

#include <glm/glm.hpp>

#include "../Camera.h"


namespace Capp {

	class Light {
	public:

		virtual ~Light();

		Framebuffer* getShadowBuffer() const;
		void setShadowResolution(const glm::ivec2& resolution) const;
		void setShadowResolution(unsigned x, unsigned y) const;
		void setShadowResolution(unsigned resolution) const;
		
		void setProjection(const glm::mat4& projection);
		const glm::mat4& getProjectionMatrix() const;
		const glm::mat4& getViewMatrix() const;
		const glm::mat4& getViewProjection() const;

		const Transform& getTransform() const;
		
	protected:
		
		Light(const glm::vec3& colour);

		void viewMatrixCalc();

		Framebuffer* _shadowBuffer;
		glm::mat4 _shadowProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 _shadowViewMatrix = glm::mat4(1.0f);
		glm::mat4 _shadowViewProjection = glm::mat4(1.0f);
		
		Transform _transform;
		glm::vec3 _colour;
		
	};
	
	class DirectionalLight : public Light {
	public:

		DirectionalLight(const glm::vec3& direction = glm::vec3(0.0f), const glm::vec3& colour = glm::vec3(1.0f));
		~DirectionalLight() = default;

		const glm::vec3& getDirection() const;
		DirectionalLight& setDirection(const glm::vec3& direction);
		DirectionalLight& setDirection(float x, float y, float z);

		const glm::vec3& getColour() const;
		DirectionalLight& setColour(const glm::vec3& colour);
		DirectionalLight& setColour(float r, float g, float b);
		
	};
	
	class PointLight : public Light {
	public:

		PointLight(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& colour = glm::vec3(1.0f), float attenuation = 0.5f);
		~PointLight() = default;

		const glm::vec3& getPosition() const;
		PointLight& setPosition(const glm::vec3& position);
		PointLight& setPosition(float x, float y, float z);

		const glm::vec3& getColour() const;
		PointLight& setColour(const glm::vec3& colour);
		PointLight& setColour(float r, float g, float b);

		float getAttenuation() const;
		PointLight& setAttenuation(float attenuation);
		
	private:
		
		float _attenuation;
		
		
	};

	class Spotlight : public Light {
	public:
		
		Spotlight(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& direction = glm::vec3(0.0f), const glm::vec3& colour = glm::vec3(1.0f), float attenuation = 0.3f, float innerCutoffAngle = 12.5f, float outerCutoffAngle = 17.5f);
		~Spotlight() = default;
		
		const glm::vec3& getPosition() const;
		Spotlight& setPosition(const glm::vec3& position);
		Spotlight& setPosition(float x, float y, float z);

		const glm::vec3& getDirection() const;
		Spotlight& setDirection(const glm::vec3& direction);
		Spotlight& setDirection(float x, float y, float z);
		
		const glm::vec3& getColour() const;
		Spotlight& setColour(const glm::vec3& colour);
		Spotlight& setColour(float r, float g, float b);

		float getAttenuation() const;
		Spotlight& setAttenuation(float attenuation);

		float getInnerCutoffAngle() const;
		Spotlight& setInnerCutoffAngle(float angle);
		
		float getOuterCutoffAngle() const;
		Spotlight& setOuterCutoffAngle(float angle);

	private:
		
		float _attenuation;
		float _innerCutoffAngle;
		float _outerCutoffAngle;
		
	};

	struct Lights {
		using PointLights = std::vector<PointLight*>;
		using DirectionalLights = std::vector<DirectionalLight*>;
		using Spotlights = std::vector<Spotlight*>;

		DirectionalLights directionalLights;
		PointLights pointLights;
		Spotlights spotlights;
	};
}
