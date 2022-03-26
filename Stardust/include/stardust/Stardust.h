#pragma once
#ifndef STARDUST_H
#define STARDUST_H

#include <SDL2/SDL.h>

#include "stardust/ai/State.h"
#include "stardust/ai/StateMachine.h"

#include "stardust/animation/easings/Easings.h"
#include "stardust/animation/Animation.h"
#include "stardust/animation/Animator.h"

#include "stardust/application/events/Events.h"
#include "stardust/application/events/GlobalEventHandler.h"
#include "stardust/application/Application.h"

#include "stardust/assets/AssetManager.h"

#include "stardust/audio/listener/Listener.h"
#include "stardust/audio/mixing/MixingBus.h"
#include "stardust/audio/sounds/SoundBase.h"
#include "stardust/audio/sounds/Sounds.h"
#include "stardust/audio/source/PositionalSoundSource.h"
#include "stardust/audio/source/SoundSource.h"
#include "stardust/audio/volume/VolumeManager.h"
#include "stardust/audio/Audio.h"
#include "stardust/audio/SoundCuePlayer.h"
#include "stardust/audio/SoundSystem.h"

#include "stardust/camera/Camera2D.h"

#include "stardust/debug/assert/Assert.h"
#include "stardust/debug/logging/Logging.h"
#include "stardust/debug/Debug.h"

#include "stardust/ecs/bundle/EntityBundle.h"
#include "stardust/ecs/components/Components.h"
#include "stardust/ecs/entity/Entity.h"
#include "stardust/ecs/entity/EntityHandle.h"
#include "stardust/ecs/registry/EntityRegistry.h"

#include "stardust/filesystem/vfs/VirtualFilesystem.h"
#include "stardust/filesystem/Filesystem.h"

#include "stardust/geometry/Shapes.h"

#include "stardust/graphics/backend/OpenGL.h"
#include "stardust/graphics/backend/OpenGLContext.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/colour/Colours.h"
#include "stardust/graphics/display/Display.h"
#include "stardust/graphics/framebuffer/Framebuffer.h"
#include "stardust/graphics/framebuffer/RenderBuffer.h"
#include "stardust/graphics/pipeline/Pipeline.h"
#include "stardust/graphics/pipeline/Shader.h"
#include "stardust/graphics/render_pass/subpasses/BlendSubpass.h"
#include "stardust/graphics/render_pass/subpasses/ClearSubpass.h"
#include "stardust/graphics/render_pass/subpasses/ColourSubpass.h"
#include "stardust/graphics/render_pass/subpasses/DepthSubpass.h"
#include "stardust/graphics/render_pass/subpasses/PipelineSubpass.h"
#include "stardust/graphics/render_pass/subpasses/RenderTargetSubpass.h"
#include "stardust/graphics/render_pass/subpasses/ScissorSubpass.h"
#include "stardust/graphics/render_pass/subpasses/StencilSubpass.h"
#include "stardust/graphics/render_pass/subpasses/ViewportSubpass.h"
#include "stardust/graphics/render_pass/RenderPass.h"
#include "stardust/graphics/render_pass/Subpass.h"
#include "stardust/graphics/renderer/objects/BufferUsage.h"
#include "stardust/graphics/renderer/objects/IndexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexAttribute.h"
#include "stardust/graphics/renderer/objects/VertexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexLayout.h"
#include "stardust/graphics/renderer/objects/VertexLayoutBuilder.h"
#include "stardust/graphics/renderer/objects/Vertices.h"
#include "stardust/graphics/renderer/states/LineBatchState.h"
#include "stardust/graphics/renderer/states/LineDrawState.h"
#include "stardust/graphics/renderer/states/QuadBatchState.h"
#include "stardust/graphics/renderer/states/QuadDrawState.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/sorting_layer/SortingLayer.h"
#include "stardust/graphics/texture/texture_atlas/TextureAtlas.h"
#include "stardust/graphics/texture/Sampler.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/Blending.h"
#include "stardust/graphics/Graphics.h"
#include "stardust/graphics/RenderArea.h"

#include "stardust/input/controller/GameController.h"
#include "stardust/input/controller/GameControllerCodes.h"
#include "stardust/input/controller/GameControllerLobby.h"
#include "stardust/input/joystick/Joystick.h"
#include "stardust/input/joystick/JoystickCodes.h"
#include "stardust/input/joystick/JoystickLobby.h"
#include "stardust/input/keyboard/KeyboardState.h"
#include "stardust/input/keyboard/KeyCodes.h"
#include "stardust/input/keyboard/KeyModState.h"
#include "stardust/input/keyboard/VirtualKeyCodes.h"
#include "stardust/input/mouse/MouseCodes.h"
#include "stardust/input/mouse/MouseState.h"
#include "stardust/input/touch/TouchDevice.h"
#include "stardust/input/InputController.h"
#include "stardust/input/InputManager.h"

#include "stardust/locale/Locale.h"

#include "stardust/math/random/Random.h"
#include "stardust/math/splines/Bezier.h"
#include "stardust/math/Math.h"

#include "stardust/particles/Particle.h"
#include "stardust/particles/ParticleData.h"
#include "stardust/particles/ParticleSystem.h"

#include "stardust/physics/body/Body.h"
#include "stardust/physics/collider/Collider.h"
#include "stardust/physics/geometry/Chain.h"
#include "stardust/physics/geometry/Circle.h"
#include "stardust/physics/geometry/Edge.h"
#include "stardust/physics/geometry/Polygon.h"
#include "stardust/physics/geometry/Rectangle.h"
#include "stardust/physics/world/World.h"
#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"
#include "stardust/physics/RaycastHit.h"

#include "stardust/preferences/ControlPrefs.h"
#include "stardust/preferences/UserPrefs.h"

#include "stardust/scene/layer/Layer.h"
#include "stardust/scene/layer/LayerStack.h"
#include "stardust/scene/resources/GlobalResources.h"
#include "stardust/scene/Scene.h"
#include "stardust/scene/SceneManager.h"

#include "stardust/scripting/ScriptEngine.h"

#include "stardust/task/AsyncTask.h"

#include "stardust/text/clipboard/Clipboard.h"
#include "stardust/text/font/Font.h"
#include "stardust/text/font/FontCache.h"
#include "stardust/text/localisation/Localisation.h"
#include "stardust/text/shaping/ShapingBuffer.h"
#include "stardust/text/text_input/TextInput.h"
#include "stardust/text/Glyph.h"
#include "stardust/text/Markup.h"
#include "stardust/text/TextCache.h"
#include "stardust/text/TextWriter.h"

#include "stardust/time/stopwatch/Stopwatch.h"
#include "stardust/time/timestep/TimestepController.h"
#include "stardust/time/Time.h"
#include "stardust/time/Timeout.h"

#include "stardust/tilemap/Tile.h"
#include "stardust/tilemap/Tilemap.h"
#include "stardust/tilemap/Tileset.h"

#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

#include "stardust/ui/Canvas.h"
#include "stardust/ui/UI.h"
#include "stardust/ui/UIComponent.h"

#include "stardust/utility/error_handling/Status.h"
#include "stardust/utility/error_handling/Result.h"
#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"
#include "stardust/utility/message_box/MessageBox.h"
#include "stardust/utility/string/String.h"
#include "stardust/utility/unicode/Unicode.h"
#include "stardust/utility/Utility.h"

#include "stardust/window/cursor/Cursor.h"
#include "stardust/window/Window.h"

namespace sd = stardust;

#endif
