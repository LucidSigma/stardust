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
* Physics (powered by Box2D).
* Lua scripting (using Sol2).
* Support for multiple locales.
* Timer and timesteps.
* Tilemaps (using Tiled JSON files).
* Scene queue.
* Converting JSON to and from MessagePack (for binary storage).
* Audio recording.
* Animation (sprites and basic tweens).
* Logging and system modals.
* Basic parallel task scheduling.

## Scene Template
```cpp
#pragma once
#ifndef EXAMPLE_SCENE_H
#define EXAMPLE_SCENE_H

#include <stardust/Stardust.h>

class ExampleScene final
	: public sd::Scene
{
public:
	ExampleScene(sd::Application& application, const sd::String& name);
	virtual ~ExampleScene() noexcept override = default;

	[[nodiscard]] virtual sd::Status OnLoad() override;
	virtual void OnUnload() noexcept override;

	virtual void FixedUpdate(const sd::f32 fixedDeltaTime) override;
	virtual void ProcessInput() override;
	virtual void Update(const sd::f32 deltaTime) override;
	virtual void LateUpdate(const sd::f32 deltaTime) override;

	virtual void Render(sd::Renderer& renderer) override;

	virtual void PollEvent(const SDL_Event& event) override;
	virtual void OnGameControllerAdded(sd::GameController& gameController) override;
	virtual void OnGameControllerRemoved(const sd::GameController& gameController) override;
};

#endif
```

## Dependencies
### Window/Input
* [SDL2](https://www.libsdl.org/)
	* [SDL_GameControllerDB](https://github.com/gabomdq/SDL_GameControllerDB)

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

### Mathematics
* [GLM](https://github.com/g-truc/glm)

### Physics
* [Box2D](https://box2d.org/)

### Scripting
* [Lua](http://www.lua.org/)
* [Sol2](https://github.com/ThePhD/sol2)

### Debugging/Testing
* [Catch2](https://github.com/catchorg/Catch2)
* [spdlog](https://github.com/gabime/spdlog)
