#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Capp {

	class Camera {
	public:

		virtual ~Camera() = default;
		
		const glm::mat4& getProjectionMatrix() const;
		const glm::mat4& getViewMatrix() const;
		const glm::mat4& getViewProjection() const;

	protected:

		virtual void viewMatrixCalc() = 0;
		
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

	};

	class OrthographicCamera : public Camera {
	public:
		
		OrthographicCamera(float left, float right, float top, float bottom);
		~OrthographicCamera() = default;
		
		void setProjection(float left, float right, float bottom, float top);

		const glm::vec3& getPosition() const;
		void setPosition(const glm::vec3& position);
		
		bool isRotatable() const;
		void setRotatable(bool rotatable);

		float getRotation() const;
		void setRotation(float rotation);

	private:

		void viewMatrixCalc() override;

		glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
		bool _isRotatable = false;
		float _rotation = 0.0f;
		
	};

	class PerspectiveCamera : public Camera {
	public:

		PerspectiveCamera(float fovY, unsigned width, unsigned height);
		~PerspectiveCamera() = default;

		void setProjection(float fovY, unsigned width, unsigned height);

		void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 getForward() const;
		glm::vec3 getUp() const;
		glm::vec3 getRight() const;

		const glm::vec3& getPosition() const;
		void setPosition(const glm::vec3& position);

		const glm::vec3& getRotation() const;
		void setRotation(const glm::vec3& rotation);
		
	private:

		void viewMatrixCalc() override;

		glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
		
	};
}