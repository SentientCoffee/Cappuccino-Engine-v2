#pragma once

// ------------------------------------------------------------
// ----- CAPPUCCINO INCLUDES ----------------------------------
// ------------------------------------------------------------

// Core engine files
#include "Cappuccino/Core/Application.h"
#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Core/Random.h"
#include "Cappuccino/Core/Time.h"

// Debugging tools
#include "Cappuccino/Debugging/Logging.h"

// Event system
#include "Cappuccino/Events/KeyEvents.h"
#include "Cappuccino/Events/MouseEvents.h"
#include "Cappuccino/Events/WindowEvents.h"

// Input system
#include "Cappuccino/Input/Input.h"
#include "Cappuccino/Input/KeyCodes.h"
#include "Cappuccino/Input/MouseButtonCodes.h"

// Game objects
#include "Cappuccino/Objects/GameObject.h"
#include "Cappuccino/Objects/Hitbox.h"
#include "Cappuccino/Objects/Rigidbody.h"
#include "Cappuccino/Objects/Transform.h"

// Rendering
#include "Cappuccino/Rendering/Camera.h"
#include "Cappuccino/Rendering/Mesh.h"
#include "Cappuccino/Rendering/RenderCommand.h"

#include "Cappuccino/Rendering/2D/2DQuad.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

#include "Cappuccino/Rendering/3D/Light.h"
#include "Cappuccino/Rendering/3D/Material.h"
#include "Cappuccino/Rendering/3D/Model.h"
#include "Cappuccino/Rendering/3D/Renderer.h"

#include "Cappuccino/Rendering/Buffers/Framebuffer.h"

#include "Cappuccino/Rendering/Shaders/Shader.h"

#include "Cappuccino/Rendering/Text/Font.h"
#include "Cappuccino/Rendering/Text/Text.h"

#include "Cappuccino/Rendering/Textures/Texture1D.h"
#include "Cappuccino/Rendering/Textures/Texture2D.h"
#include "Cappuccino/Rendering/Textures/Texture3D.h"
#include "Cappuccino/Rendering/Textures/TextureCubemap.h"

// Resource management
#include "Cappuccino/Resource/AssetLoader.h"
#include "Cappuccino/Resource/FontLibrary.h"
#include "Cappuccino/Resource/MeshLibrary.h"
#include "Cappuccino/Resource/ShaderLibrary.h"

// Scene management
#include "Cappuccino/Scenes/Layer.h"
#include "Cappuccino/Scenes/Scene.h"
#include "Cappuccino/Scenes/SceneManager.h"


// ------------------------------------------------------------
// ----- OTHER INCLUDES ---------------------------------------
// ------------------------------------------------------------

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ImGui
#include <imgui/imgui.h>
