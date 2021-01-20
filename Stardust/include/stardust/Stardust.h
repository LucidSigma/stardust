#pragma once
#ifndef STARDUST_H
#define STARDUST_H

#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include <sol/sol.hpp>

#include "animation/Animation.h"
#include "animation/Animator.h"

#include "application/Application.h"
#include "assets/AssetManager.h"

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

#include "debug/logging/Log.h"
#include "debug/message_box/MessageBox.h"

#include "filesystem/Filesystem.h"
#include "filesystem/vfs/VFS.h"

#include "graphics/backend/OpenGL.h"
#include "graphics/backend/OpenGLContext.h"
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
#include "graphics/Colour.h"
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

#include "particles/ParticleSystem.h"

#include "physics/body/Body.h"
#include "physics/world/World.h"
#include "physics/Physics.h"

#include "scene/components/Components.h"
#include "scene/entity/Entity.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"

#include "scripting/ScriptEngine.h"

#include "text/clipboard/Clipboard.h"
#include "text/font/Font.h"
#include "text/text_input/TextInput.h"
#include "text/Text.h"
#include "tilemap/Tilemap.h"

#include "utility/cpu/CPU.h"
#include "utility/interfaces/INoncopyable.h"
#include "utility/interfaces/INonmovable.h"
#include "utility/random/Random.h"
#include "utility/status/Status.h"
#include "utility/tasks/Task.h"
#include "utility/Utility.h"

#include "window/Window.h"

namespace sd = stardust;

#endif