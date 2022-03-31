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
        <td>Positional audio with SoLoud.</td>
        <td>Particle system.</td>
    </tr>
    <tr>
        <td>Physics (powered by Box2D).</td>
        <td>Lua scripting (using Sol2).</td>
    </tr>
    <tr>
        <td>Support for multiple locales.</td>
        <td>Timesteps.</td>
    </tr>
    <tr>
        <td>Tilemaps.</td>
        <td>Scene queue.</td>
    </tr>
    <tr>
        <td>Animation (sprites and basic tweens).</td>
        <td>Logging, assertions, and system modals.</td>
    </tr>
</table>

## Documentation.
Coming soon.

### Tutorial.
Coming soon.

### Examples.
Coming soon.

## Dependencies.
### Window/Input.
* [SDL2](https://www.libsdl.org/)
    * [SDL_GameControllerDB](https://github.com/gabomdq/SDL_GameControllerDB)

### Graphics.
* [ANGLE](https://github.com/google/angle)
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
* [stb_image_write](https://github.com/nothings/stb/blob/master/stb_image_write.h)

### Text rendering.
* [FreeType](https://www.freetype.org/)
* [FreeType GL](https://github.com/rougier/freetype-gl)
* [HarfBuzz](https://harfbuzz.github.io/)

### Audio.
* [SoLoud](https://sol.gfxile.net/soloud/)
    * [dr_libs](https://github.com/mackron/dr_libs)
    * [stb_vorbis](https://github.com/nothings/stb/blob/master/stb_vorbis.c)

### Data.
* [EnTT](https://github.com/skypjack/entt)
* [ICU](https://icu.unicode.org/)
* [magic_enum](https://github.com/Neargye/magic_enum)
* [TartanLlama's Generator](https://github.com/TartanLlama/generator)
* [thread-pool](https://github.com/bshoshany/thread-pool)
* [utfcpp](https://github.com/nemtrif/utfcpp)

### Serialisation/Parsing.
* [nlohmann JSON](https://github.com/nlohmann/json)
* [pugixml](https://pugixml.org/)
* [toml++](https://marzer.github.io/tomlplusplus/)

### Assets.
* [PhysFS](https://icculus.org/physfs/)

### Mathematics.
* [GLM](https://github.com/g-truc/glm)

### Physics.
* [Box2D](https://box2d.org/)

### Scripting.
* [Lua](http://www.lua.org/)
* [Sol2](https://github.com/ThePhD/sol2)

### Debugging/Testing.
* [Catch2](https://github.com/catchorg/Catch2)
* [spdlog](https://github.com/gabime/spdlog)
* [STX](https://github.com/lamarrr/STX)
