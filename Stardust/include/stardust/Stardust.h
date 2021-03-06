#pragma once
#ifndef STARDUST_H
#define STARDUST_H

#ifndef SDL_ASSERT_LEVEL
    #ifdef NDEBUG
        #define SDL_ASSERT_LEVEL 1
    #else
        #define SDL_ASSERT_LEVEL 2
    #endif
#endif

#include "ai/game_tree/GameState.h"
#include "ai/game_tree/Minimax.h"
#include "ai/graph/Graph.h"
#include "ai/graph/Pathfinding.h"
#include "ai/graph/Traversal.h"
#include "ai/state_machine/State.h"
#include "ai/state_machine/StateMachine.h"
#include "ai/Boids.h"

#include "animation/Animation.h"
#include "animation/Animator.h"
#include "animation/Easings.h"

#include "application/events/Events.h"
#include "application/AppConfig.h"
#include "application/Application.h"
#include "assets/AssetManager.h"

#include "audio/recording/RecordingDevice.h"
#include "audio/sounds/SoundBase.h"
#include "audio/sounds/SoundEnums.h"
#include "audio/sounds/Sounds.h"
#include "audio/volume/VolumeManager.h"
#include "audio/Listener.h"
#include "audio/SoundSource.h"
#include "audio/SoundSystem.h"

#include "config/Config.h"
#include "camera/Camera2D.h"

#include "data/Containers.h"
#include "data/MathTypes.h"
#include "data/Pointers.h"
#include "data/Types.h"

#include "debug/assert/Assert.h"
#include "debug/logging/Log.h"
#include "debug/message_box/MessageBox.h"
#include "debug/Debug.h"

#include "filesystem/serialise/Serialise.h"
#include "filesystem/shared_object/SharedObject.h"
#include "filesystem/vfs/VFS.h"
#include "filesystem/vfs/VirtualFileHandle.h"
#include "filesystem/FileHandle.h"
#include "filesystem/Filesystem.h"

#include "graphics/backend/OpenGL.h"
#include "graphics/backend/OpenGLContext.h"
#include "graphics/colour/Colour.h"
#include "graphics/colour/Colours.h"
#include "graphics/display/Display.h"
#include "graphics/display/VSyncType.h"
#include "graphics/renderer/objects/BufferUsage.h"
#include "graphics/renderer/objects/IndexBuffer.h"
#include "graphics/renderer/objects/VertexBuffer.h"
#include "graphics/renderer/objects/VertexLayout.h"
#include "graphics/renderer/FlipType.h"
#include "graphics/renderer/Renderer.h"
#include "graphics/renderer/Quad.h"
#include "graphics/shaders/Shader.h"
#include "graphics/shaders/ShaderProgram.h"
#include "graphics/surface/PixelSurface.h"
#include "graphics/texture/texture_atlas/TextureAtlas.h"
#include "graphics/texture/Sampler.h"
#include "graphics/texture/Texture.h"
#include "graphics/SortingLayer.h"

#include "input/controller/GameController.h"
#include "input/controller/GameControllerCodes.h"
#include "input/keyboard/Keyboard.h"
#include "input/keyboard/KeyCodes.h"
#include "input/mouse/Mouse.h"
#include "input/mouse/MouseButtonCodes.h"
#include "input/AxisType.h"
#include "input/Input.h"
#include "input/InputManager.h"
#include "locale/Locale.h"

#include "math/Math.h"
#include "memory/MemoryBuffer.h"
#include "memory/ObjectPool.h"

#include "particles/ParticleData.h"
#include "particles/ParticleSystem.h"

#include "physics/body/Body.h"
#include "physics/collider/Collider.h"
#include "physics/geometry/Chain.h"
#include "physics/geometry/Circle.h"
#include "physics/geometry/Edge.h"
#include "physics/geometry/Polygon.h"
#include "physics/geometry/Rectangle.h"
#include "physics/raycast/RaycastHit.h"
#include "physics/world/World.h"
#include "physics/AABB.h"
#include "physics/Physics.h"

#include "rect/Rect.h"

#include "scene/components/Components.h"
#include "scene/entity/Entity.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"

#include "scripting/ScriptEngine.h"

#include "system/cpu/CPU.h"
#include "system/gpu/GPU.h"
#include "system/System.h"

#include "task/scheduler/TaskScheduler.h"
#include "task/AsyncTask.h"
#include "task/TaskStats.h"

#include "text/clipboard/Clipboard.h"
#include "text/font/Font.h"
#include "text/font/FontCache.h"
#include "text/text_input/TextInput.h"
#include "text/Text.h"
#include "text/TextCache.h"

#include "tilemap/Tilemap.h"
#include "time/stopwatch/Stopwatch.h"
#include "time/Time.h"
#include "time/Timers.h"

#include "ui/context/UIContext.h"
#include "ui/UI.h"

#include "utility/hash/Hash.h"
#include "utility/interfaces/INoncopyable.h"
#include "utility/interfaces/INonmovable.h"
#include "utility/interfaces/ISerialisable.h"
#include "utility/random/Random.h"
#include "utility/status/Status.h"
#include "utility/version/Version.h"
#include "utility/Utility.h"

#include "window/cursor/Cursor.h"
#include "window/Window.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>
#include <sol/sol.hpp>
#include <tinyxml2/tinyxml2.h>
#include <toml++/toml.hpp>

namespace sd = stardust;

#endif