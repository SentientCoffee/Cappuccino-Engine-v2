#pragma once

#include "Cappuccino/Objects/Transform.h"
#include "Cappuccino/Rendering/Buffers/Framebuffer.h"

#include <glm/glm.hpp>


namespace Capp {

	class Light {
	public:

		virtual ~Light() = default;

		void setShadowResolution(uint32_t resolution) const;
		
		void setProjection(const glm::mat4& projection) { _shadowProjectionMatrix = projection; }

		Ref<Framebuffer> getShadowBuffer() const { return _shadowBuffer; }

		const glm::mat4& getProjectionMatrix() const { return _shadowProjectionMatrix; }
		const glm::mat4& getViewMatrix() const { return _shadowViewMatrix; }
		const glm::mat4& getViewProjection() const { return _shadowViewProjection; }

	protected:

		Light() = default;
		Light(const glm::vec3& colour);

		void viewMatrixCalc();

		Ref<Framebuffer> _shadowBuffer;
		glm::mat4 _shadowProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 _shadowViewMatrix       = glm::mat4(1.0f);
		glm::mat4 _shadowViewProjection   = glm::mat4(1.0f);
		
		Transform _transform;
		glm::vec3 _colour = glm::vec4(1.0f);
		
	};
	
	class DirectionalLight : public Light {
	public:
		
		DirectionalLight(const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f), const glm::vec3& colour = glm::vec3(1.0f));
		~DirectionalLight() = default;

		static Ref<DirectionalLight> create(const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f), const glm::vec3& colour = glm::vec3(1.0f)) {
			return Memory::createRef<DirectionalLight>(direction, colour);
		}

		const glm::vec3& getDirection() const;
		DirectionalLight& setDirection(const glm::vec3& direction);
		DirectionalLight& setDirection(float x, float y, float z);

		const glm::vec3& getColour() const { return _colour; }
		DirectionalLight& setColour(const glm::vec3& colour);
		DirectionalLight& setColour(float r, float g, float b);
		
	};
	
	class PointLight : public Light {
	public:

		PointLight(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& colour = glm::vec3(1.0f), float attenuation = 0.5f);
		~PointLight() = default;

		static Ref<PointLight> create(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& colour = glm::vec3(1.0f), float attenuation = 0.5f) {
			return Memory::createRef<PointLight>(position, colour, attenuation);
		}

		const glm::vec3& getPosition() const;
		PointLight& setPosition(const glm::vec3& position);
		PointLight& setPosition(float x, float y, float z);

		const glm::vec3& getColour() const { return _colour; }
		PointLight& setColour(const glm::vec3& colour);
		PointLight& setColour(float r, float g, float b);

		float getAttenuation() const { return _attenuation; }
		PointLight& setAttenuation(float attenuation);
		
	private:
		
		float _attenuation;
		
	};

	class Spotlight : public Light {
	public:

		Spotlight(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& direction = glm::vec3(0.0f), const glm::vec3& colour = glm::vec3(1.0f), float attenuation = 0.3f, float innerCutoffAngle = 12.5f, float outerCutoffAngle = 17.5f);
		~Spotlight() = default;

		static Ref<Spotlight> create(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& direction = glm::vec3(0.0f), const glm::vec3& colour = glm::vec3(1.0f), float attenuation = 0.3f, float innerCutoffAngle = 12.5f, float outerCutoffAngle = 17.5f) {
			return Memory::createRef<Spotlight>(position, direction, colour, attenuation, innerCutoffAngle, outerCutoffAngle);
		}
		
		const glm::vec3& getPosition() const;
		Spotlight& setPosition(const glm::vec3& position);
		Spotlight& setPosition(float x, float y, float z);

		const glm::vec3& getDirection() const;
		Spotlight& setDirection(const glm::vec3& direction);
		Spotlight& setDirection(float x, float y, float z);
		
		const glm::vec3& getColour() const { return _colour; }
		Spotlight& setColour(const glm::vec3& colour);
		Spotlight& setColour(float r, float g, float b);

		float getAttenuation() const { return _attenuation; }
		Spotlight& setAttenuation(float attenuation);

		float getInnerCutoffAngle() const { return  _innerCutoffAngle; }
		Spotlight& setInnerCutoffAngle(float angle);
		
		float getOuterCutoffAngle() const { return _outerCutoffAngle; }
		Spotlight& setOuterCutoffAngle(float angle);

	private:
		
		float _attenuation;
		float _innerCutoffAngle;
		float _outerCutoffAngle;
		
	};

	struct Lights {
		using DirectionalLights = std::vector<Ref<DirectionalLight>>;
		using PointLights       = std::vector<Ref<PointLight>>;
		using Spotlights        = std::vector<Ref<Spotlight>>;

		DirectionalLights directionalLights;
		PointLights pointLights;
		Spotlights spotlights;
	};
}
