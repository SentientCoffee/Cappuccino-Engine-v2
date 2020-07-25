#pragma once

#include "Cappuccino/Objects/GameObject.h"
#include "Cappuccino/Objects/Hitbox.h"
#include "Cappuccino/Objects/Rigidbody.h"

#include "Cappuccino/Rendering/Camera.h"
#include "Cappuccino/Rendering/Mesh.h"

#include "Cappuccino/Rendering/3D/Light.h"
#include "Cappuccino/Rendering/3D/Material.h"
#include "Cappuccino/Rendering/3D/Model.h"

#include "Cappuccino/Rendering/Buffers/Framebuffer.h"
#include "Cappuccino/Rendering/Shaders/Shader.h"
#include "Cappuccino/Rendering/Textures/TextureCubemap.h"

#include <optional>

namespace Capp {

	struct PostProcessingPass {
		Ref<Framebuffer> buffer;
		Ref<Shader> shader;
	};

	using PostPasses = std::vector<PostProcessingPass>;
	using Skybox = std::optional<Ref<TextureCubemap>>;

	class Renderer {
	public:

		// Init and shutdown
		static void init();
		static void shutdown();

		// Window resized function (for framebuffers)
		static void onWindowResized(uint32_t width, uint32_t height);

		// Pre and post rendering
		static void start();
		static void start(const PerspectiveCamera& camera, const Lights& lights, const Skybox& skybox = std::nullopt);
		
		static void finish(const PostPasses& postProcessing = {});

		// Rendering commands
		static void addToQueue(const Ref<Mesh>& mesh, const Ref<Material>& material);

		static void addToQueue(const Ref<Model>& model);

		static void addToQueue(const Ref<GameObject>& gameObject);

		static void addToQueue(const Ref<Hitbox>& hitbox);
		static void addToQueue(const RigidBody& rigidBody);

		// Gamma correction
		static void setGamma(float gamma);
		
	};
	
}
