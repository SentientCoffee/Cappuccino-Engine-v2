#pragma once

// Core engine files
#include "Cappuccino/Core/Application.h"
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

// Rendering
#include "Cappuccino/Rendering/Camera.h"
#include "Cappuccino/Rendering/Shader.h"
#include "Cappuccino/Rendering/Texture.h"
#include "Cappuccino/Rendering/RenderCommand.h"

#include "Cappuccino/Rendering/3D/Material.h"
#include "Cappuccino/Rendering/3D/Mesh.h"
#include "Cappuccino/Rendering/3D/Model.h"
#include "Cappuccino/Rendering/3D/Renderer.h"

#include "Cappuccino/Rendering/2D/2DQuad.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

#include "Cappuccino/Rendering/Text/Text.h"
#include "Cappuccino/Rendering/Text/Font.h"

// Resource management
#include "Cappuccino/Resource/FontLibrary.h"
#include "Cappuccino/Resource/MeshLibrary.h"
#include "Cappuccino/Resource/ShaderLibrary.h"

// Scene management
#include "Cappuccino/Scenes/Scene.h"
#include "Cappuccino/Scenes/Layer.h"

// Game objects
#include "Cappuccino/Objects/Transform.h"
#include "Cappuccino/Objects/Hitbox.h"