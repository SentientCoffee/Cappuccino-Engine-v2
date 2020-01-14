# Welcome to Cappuccino Engine!
Welcome to Cappuccino Engine, an 3D game engine built on OpenGL!
Cappuccino Engine supports .obj file loading, image loading (using [stb_image](https://github.com/nothings/stb)), and ImGui debugging (from the [ImGui github](https://github.com/ocornut/imgui))

### Current features of the engine:
- Events and input (using [GLFW](https://github.com/glfw/glfw))
- Scenes and layers
- Texture loading
- Mesh loading (with index buffers)
- Basic 2D and 3D Rendering

More on the way:
- Text rendering and font loading
- Game objects
- Collisions & Rigidbodies
- Material system
- Raycasting
- Lighting system
- Animation system (morph targets and skeletal animation)
- Viewports
- Sound system


# Building Cappuccino Engine
### Note:
Currently, only Windows builds are supported. Other platforms will be supported in the future.

### Building
All you have to do to build Cappuccino Engine is run `CappEngineGeneration.bat`, which is included in the files.
Then you can make a game project using the `CappGameGeneraion.bat` file in the `GameGeneration` folder, which will make a new project that links into the Cappuccino Engine.

### One Last Thing
[Jewel3D](https://github.com/EmilianC/Jewel3D) was a big part of our learning over the summer of 2019, and we would like to personally thank the creator.
Also, the current version of the engine is very heavily inspired by the [Hazel engine](https://github.com/TheCherno/Hazel) made by [The Cherno](https://www.youtube.com/user/TheChernoProject).
Please check them both out!

If you would like to see the jank that is version 1 of this engine, please follow this [link](https://github.com/Promethaes/CappuccinoEngine) at your own risk.