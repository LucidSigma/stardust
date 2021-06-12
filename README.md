# Stardust.
A 2D game engine/framework built with C++20, SDL2, and OpenGL.

This project is currently for personal use while I keep tweaking the API and features (although you're more than welcome to use it).  
Once I have created a few demo games and solidified the API enough, I will begin work on proper documentation and examples.  

## Current Functionality.
<table>
    <tr>
        <td>Window creation with SDL2.</td>
        <td>Batch rendering with OpenGL.</td>
    </tr>
    <tr>
        <td>2D camera.</td>
        <td>ECS with EnTT.</td>
    </tr>
    <tr>
        <td>Full keyboard, mouse, and controller input support.</td>
        <td>Virtual filesystem with PhysFS.</td>
    </tr>
    <tr>
        <td>Data serialisation with cereal.</td>
        <td>Positional audio with SoLoud.</td>
    </tr>
    <tr>
        <td>Config loading.</td>
        <td>Particle system.</td>
    </tr>
    <tr>
        <td>Physics (powered by Box2D).</td>
        <td>Lua scripting (using Sol2).</td>
    </tr>
    <tr>
        <td>Support for multiple locales.</td>
        <td>Timer and timesteps.</td>
    </tr>
    <tr>
        <td>Tilemaps.</td>
        <td>Scene queue.</td>
    </tr>
    <tr>
        <td>Audio recording.</td>
        <td>Querying system information.</td>
    </tr>
    <tr>
        <td>Animation (sprites and basic tweens).</td>
        <td>Logging, assertions, and system modals.</td>
    </tr>
    <tr>
        <td>Shared object/DLL loading. </td>
        <td>Async tasks and parallel task scheduling.</td>
    </tr>
    <tr>
        <td>Basic game AI (graphs, minimax, state machine, boids).</td>
        <td>Memory buffer handling.</td>
    </tr>
</table>

### Planned Functionality.
* Use C++20 modules when most IDEs and CMake properly/fully support them.
* Custom UI with [RmlUi](https://github.com/mikke89/RmlUi).
* Socket networking with [ENet](http://enet.bespin.org/).
* HTTP requests with [libcurl](https://curl.se/).

## Documentation.
Coming soon.

### Building.
See [here](./docs/BUILDING.md).

### Tutorial.
Coming soon.

### Examples.
Coming soon.

## Scene Template.
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
    virtual void ProcessInput(const sd::InputManager& inputManager) override;
    virtual void Update(const sd::f32 deltaTime) override;
    virtual void LateUpdate(const sd::f32 deltaTime) override;

    virtual void Render(sd::Renderer& renderer) override;

    virtual void PollEvent(const sd::Event& event) override;
    virtual void OnGameControllerAdded(sd::GameController& gameController) override;
    virtual void OnGameControllerRemoved(const sd::GameController& gameController) override;
};

#endif
```

## Dependencies.
### Window/Input.
* [SDL2](https://www.libsdl.org/)
    * [SDL_GameControllerDB](https://github.com/gabomdq/SDL_GameControllerDB)

### Graphics.
* [Glad](https://glad.dav1d.de/)
* [OpenGL](https://www.opengl.org/)
* [RmlUi](https://github.com/mikke89/RmlUi)
    * [FreeType](https://www.freetype.org/)

### Audio.
* [SoLoud](https://sol.gfxile.net/soloud/)
    * [dr_libs](https://github.com/mackron/dr_libs)
    * [stb_vorbis](https://github.com/nothings/stb/blob/master/stb_vorbis.c)

### Data.
* [cpu_features](https://github.com/google/cpu_features)
* [concurrentqueue](https://github.com/cameron314/concurrentqueue)
* [EnTT](https://github.com/skypjack/entt)
* [fifo_map](https://github.com/nlohmann/fifo_map)
* [magic_enum](https://github.com/Neargye/magic_enum)
* [psched](https://github.com/p-ranav/psched)
* [stbrumme's hash library](https://github.com/stbrumme/hash-library)

### Serialisation/Parsing.
* [cereal](https://github.com/USCiLab/cereal)
* [nlohmann JSON](https://github.com/nlohmann/json)
* [tinyxml2](https://github.com/leethomason/tinyxml2)
* [toml++](https://marzer.github.io/tomlplusplus/)

### Assets.
* [PhysFS](https://icculus.org/physfs/)
* [SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/)
    * [FreeType](https://www.freetype.org/)
    * [zlib](https://zlib.net/)
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)

### Mathematics.
* [GLM](https://github.com/g-truc/glm)

### Physics.
* [Box2D](https://box2d.org/)

### Scripting.
* [Lua](http://www.lua.org/)
* [Sol2](https://github.com/ThePhD/sol2)

### Debugging/Testing.
* [Catch2](https://github.com/catchorg/Catch2)
* [Google Benchmark](https://github.com/google/benchmark)
* [spdlog](https://github.com/gabime/spdlog)
