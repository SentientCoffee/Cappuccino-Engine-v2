#pragma once

#include "Cappuccino/Resource/FontLibrary.h"
#include "Cappuccino/Resource/MeshLibrary.h"
#include "Cappuccino/Resource/ShaderLibrary.h"

namespace Capp {

	class ResourceManager {
	public:

		static void init() {
			FontLibrary::init();
			MeshLibrary::init();
			ShaderLibrary::init();
		}
		
		static void shutdown() {
			FontLibrary::shutdown();
			MeshLibrary::shutdown();
			ShaderLibrary::shutdown();
		}
	};
	
}