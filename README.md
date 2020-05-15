# Welcome to Cappuccino Engine!
Welcome to Cappuccino Engine, an 3D game engine built on OpenGL!
Cappuccino Engine supports .obj file loading, image loading (using [stb_image](https://github.com/nothings/stb)), and [ImGui](https://github.com/ocornut/imgui) debugging.
It also utilizes the [GLM math library](https://glm.g-truc.net/0.9.9/index.html), and supports logging using [spdlog](https://github.com/gabime/spdlog).

### Current features of the engine:
- Events and input (using [GLFW](https://github.com/glfw/glfw) and [GLAD](https://glad.dav1d.de/))
- Scenes and layers
- Texture loading
- Mesh loading (with index buffers) -- currently only supports `.obj` files
- Basic 2D and 3D Rendering
- Text rendering and font loading (using [Freetype](https://www.freetype.org/))
- Game objects
- Collisions & Rigidbodies
- Material system
- Lighting system
- Framebuffers
- Shadow mapping support
- Custom post-processing effects

More on the way:
- Raycasting
- Sound system (using [FMOD](https://www.fmod.com/))
- Animation system (morph targets and skeletal animation)

# Building Cappuccino Engine
### Note:
Currently, only Windows builds are supported. Other platforms will be supported in the future.

### Building
All you have to do to build Cappuccino Engine is run `CappEngineGeneration.bat`, which is included in the files. A Sandbox application is included in the engine to see its capabilities.

Optionally, you can make a game project using the `CappGameGeneraion.bat` file in the `GameGeneration` folder, which will make a new project that links into the Cappuccino Engine.
Note that you need to have Cappuccino Engine installed and built <b>BEFORE</b> you try to make a game project.

# One Last Thing
[Jewel3D](https://github.com/EmilianC/Jewel3D) was a big part of our learning over the summer of 2019, and we would like to personally thank the creator, Emilian Cioca.
Also, the current version of the engine is very heavily inspired by the [Hazel engine](https://github.com/TheCherno/Hazel) made by [The Cherno](https://www.youtube.com/user/TheChernoProject).
Please check them both out!

If you would like to see the jank that is version 1 of this engine, please follow this [link](https://github.com/Promethaes/CappuccinoEngine) at your own risk.