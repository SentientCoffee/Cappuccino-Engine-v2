#pragma once

#include "Cappuccino/Objects/Transform.h"

#include <glm/glm.hpp>

namespace Capp {

	class Camera {
	public:

		virtual ~Camera() = default;
		
		const glm::mat4& getProjectionMatrix() const;
		const glm::mat4& getViewMatrix() const;
		const glm::mat4& getViewProjection() const;

	protected:

		virtual void viewMatrixCalc();
		
		glm::mat4 _projectionMatrix = glm::mat4(1.0f);
		union {
			mutable glm::mat4 _viewMatrix = glm::mat4(1.0f);
			struct {
				float
					_leftX,  _upX,    _backX,  _m03,
					_leftY,  _upY,    _backY,  _m13,
					_leftZ,  _upZ,    _backZ,  _m23,
					_transX, _transY, _transZ, _m33;
			};
		};
		glm::mat4 _viewProjection = glm::mat4(1.0f);

		Transform _transform;

	};

	class OrthographicCamera : public Camera {
	public:
		
		OrthographicCamera() = default;
		OrthographicCamera(float left, float right, float top, float bottom);
		~OrthographicCamera() = default;
		
		void setProjection(float left, float right, float top, float bottom);

		const glm::vec3& getPosition() const;
		OrthographicCamera& setPosition(const glm::vec3& position);
		OrthographicCamera& setPosition(float x, float y, float z);
		
		bool isRotatable() const;
		void setRotatable(bool rotatable);

		float getRotation() const;
		OrthographicCamera& setRotation(float rotation);

	private:

		bool _isRotatable = false;
		
	};

	class PerspectiveCamera : public Camera {
	public:

		PerspectiveCamera() = default;
		PerspectiveCamera(float fovY, unsigned width, unsigned height);
		~PerspectiveCamera() = default;

		void setProjection(float fovY, unsigned width, unsigned height);

		void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
		void lookAt(float targetX, float targetY, float targetZ, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 getForward() const;
		glm::vec3 getUp() const;
		glm::vec3 getRight() const;

		const glm::vec3& getPosition() const;
		PerspectiveCamera& setPosition(const glm::vec3& position);
		PerspectiveCamera& setPosition(float x, float y, float z);

		PerspectiveCamera& setLocalPosition(const glm::vec3& localPosition);

		const glm::vec3& getRotation() const;
		PerspectiveCamera& setRotation(const glm::vec3& rotation);
		PerspectiveCamera& setRotation(float x, float y, float z);
		
	};
}