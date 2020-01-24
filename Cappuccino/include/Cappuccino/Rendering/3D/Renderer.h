#pragma once

#include "Cappuccino/Objects/GameObject.h"
#include "Cappuccino/Objects/Hitbox.h"
#include "Cappuccino/Objects/Rigidbody.h"

#include "Cappuccino/Rendering/Camera.h"
#include "Cappuccino/Rendering/Shader.h"
#include "Cappuccino/Rendering/VertexArray.h"

#include "Cappuccino/Rendering/3D/Light.h"
#include "Cappuccino/Rendering/3D/Material.h"
#include "Cappuccino/Rendering/3D/Mesh.h"
#include "Cappuccino/Rendering/3D/Model.h"
#include "TextureCubemap.h"

namespace Capp {

	struct Lights {
		using PointLights = std::vector<PointLight*>;
		using DirectionalLights = std::vector<DirectionalLight*>;
		using Spotlights = std::vector<Spotlight*>;

		DirectionalLights directionalLights = {};
		PointLights pointLights = {};
		Spotlights spotlights = {};
	};
	
	class Renderer {
	public:

		static void init();
		static void shutdown();

		static void onWindowResized(unsigned width, unsigned height);
		
		static void start();
		static void start(const PerspectiveCamera& camera, const Lights& lights, TextureCubemap* skybox);
		
		static void finish();

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
