#pragma once
#ifndef STARDUST_H
#define STARDUST_H

#include <entt/entt.hpp>
#include <SDL2/SDL.h>

#include "application/Application.h"
#include "assets/AssetManager.h"
#include "config/Config.h"
#include "camera/Camera2D.h"

#include "data/Containers.h"
#include "data/MathTypes.h"
#include "data/Pointers.h"
#include "data/Types.h"

#include "debug/message_box/MessageBox.h"
#include "debug/logging/Log.h"

#include "filesystem/Filesystem.h"
#include "filesystem/vfs/VFS.h"

#include "graphics/backend/OpenGL.h"
#include "graphics/backend/OpenGLContext.h"
#include "graphics/renderer/objects/BufferUsage.h"
#include "graphics/renderer/objects/IndexBuffer.h"
#include "graphics/renderer/objects/VertexBuffer.h"
#include "graphics/renderer/objects/VertexLayout.h"
#include "graphics/renderer/Renderer.h"
#include "graphics/shaders/Shader.h"
#include "graphics/shaders/ShaderProgram.h"
#include "graphics/surface/PixelSurface.h"
#include "graphics/Colour.h"

#include "locale/Locale.h"

#include "math/Math.h"

#include "scene/components/Components.h"
#include "scene/entity/Entity.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"

#include "text/clipboard/Clipboard.h"

#include "utility/cpu/CPU.h"
#include "utility/enums/Status.h"
#include "utility/interfaces/INoncopyable.h"
#include "utility/interfaces/INonmovable.h"
#include "utility/random/Random.h"

#include "window/display/Display.h"
#include "window/Window.h"

namespace sd = stardust;

#endif