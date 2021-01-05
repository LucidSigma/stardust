# Stardust
A 2D game engine/framework built with C++20, SDL2, and OpenGL.

This project is currently for personal use while I keep tweaking the API and features (although you're more than welcome to use it).  
Once I have created a few demo games and solidified the API enough, I will begin work on proper documentation.  

## Current Functionality
* Window creation with SDL2.
* Batch rendering with OpenGL.
* 2D camera.
* ECS with EnTT.
* Full keyboard, mouse, and controller input support.
* Virtual filesystem with PhysFS.
* Positional audio with SoLoud.
* Config loading.
* Particle system.
* Lua scripting (using Sol2).
* Support for multiple locales.
* Timer and timesteps.
* Scene queue.
* Converting JSON to and from MessagePack (for binary storage).
* Animation (sprites and basic tweens).
* Logging and system modals.
* Basic parallel task scheduling.

## Functionality to come
* Creating tilemaps (reading TMX files from Tiled).
* Basic AI (graphs, pathfinding/searching, boids, minimax, et cetera).
* Graphical effects (outlines, kernel effects (blur, edge detection), HDR/bloom).
* Rendering to multiple viewport setups (single, double horizontal, double vertical, quad).
* UI (canvases, basic elements such as buttons/sliders/checkboxes).
* Networking support.
* Coloured keyboard lighting support (Corsair, Razer, Logitech).
* Replace OpenGL renderer with Vulkan (possibly).

## Dependencies
### Window/Input
* [SDL2](https://www.libsdl.org/)

### Graphics
* [Glad](https://glad.dav1d.de/)
* [OpenGL](https://www.opengl.org/)

### Audio
* [SoLoud](https://sol.gfxile.net/soloud/)
	* [dr_libs](https://github.com/mackron/dr_libs)
	* [stb_vorbis](https://github.com/nothings/stb/blob/master/stb_vorbis.c)

### Data
* [concurrentqueue](https://github.com/cameron314/concurrentqueue)
* [EnTT](https://github.com/skypjack/entt)
* [magic_enum](https://github.com/Neargye/magic_enum)
* [nlohmann JSON](https://github.com/nlohmann/json)
* [psched](https://github.com/p-ranav/psched)

### Assets
* [PhysFS](https://icculus.org/physfs/)
* [SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/)
	* [FreeType](https://www.freetype.org/)
	* [zlib](https://zlib.net/)
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
* [stb_image_write](https://github.com/nothings/stb/blob/master/stb_image_write.h)
* [tileson](https://github.com/SSBMTonberry/tileson)

### Mathematics
* [GLM](https://github.com/g-truc/glm)

### Scripting
* [Lua](http://www.lua.org/)
* [Sol2](https://github.com/ThePhD/sol2)

### Debugging/Testing
* [Catch2](https://github.com/catchorg/Catch2)
* [spdlog](https://github.com/gabime/spdlog)
