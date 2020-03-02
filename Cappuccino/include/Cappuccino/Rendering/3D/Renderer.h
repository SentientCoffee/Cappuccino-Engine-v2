#pragma once

#include "Cappuccino/Objects/GameObject.h"
#include "Cappuccino/Objects/Hitbox.h"
#include "Cappuccino/Objects/Rigidbody.h"

#include "Cappuccino/Rendering/Camera.h"
#include "Cappuccino/Rendering/VertexArray.h"

#include "Cappuccino/Rendering/3D/Light.h"
#include "Cappuccino/Rendering/3D/Material.h"
#include "Cappuccino/Rendering/3D/Mesh.h"
#include "Cappuccino/Rendering/3D/Model.h"

#include "Cappuccino/Rendering/Buffers/Framebuffer.h"
#include "Cappuccino/Rendering/Shaders/Shader.h"
#include "Cappuccino/Rendering/Textures/TextureCubemap.h"

namespace Capp {

	struct PostProcessingPass {
		Framebuffer* buffer;
		Shader* shader;
	};

	using PostPasses = std::vector<PostProcessingPass>;

	class Renderer {
	public:

		static void init();
		static void shutdown();

		static void onWindowResized(unsigned width, unsigned height);
		
		static void start();
		static void start(const PerspectiveCamera& camera, const Lights& lights, const std::optional<TextureCubemap*>& skybox = std::nullopt);
		
		static void finish(const PostPasses& postProcessing = {});

		static void addToRenderList(VertexArray* vertexArray, Shader* shader = nullptr);
		static void addToRenderList(VertexArray* vertexArray, Material* material);
		
		static void addToRenderList(Mesh* mesh, Shader* shader);
		static void addToRenderList(Mesh* mesh, Material* material);

		static void addToRenderList(Model* model);

		static void addToRenderList(Hitbox* hitbox);
		static void addToRenderList(const RigidBody& rigidBody);

		static void addToRenderList(GameObject* gameObject);
		
	};
	
}
