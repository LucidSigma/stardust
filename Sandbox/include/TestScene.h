#pragma once
#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include <stardust/Stardust.h>

class TestScene final
	: public sd::Scene
{
private:
	sd::SortingLayer m_defaultSortingLayer{ "default" };

	sd::ObserverPtr<sd::Texture> m_crateTexture = nullptr;
	sd::ObserverPtr<sd::Texture> m_crumbleTexture = nullptr;

	sd::FontCache m_font;
	sd::TextCache m_textCache;
	sd::ObserverPtr<const sd::Texture> m_glyphTexture = nullptr;
	sd::ObserverPtr<const sd::Texture> m_textTexture = nullptr;

	sd::AssetManager<sd::Texture> m_textures;

	sd::TextureAtlas m_conveyorTextures;
	sd::TextureAtlas m_colourTextures;
	sd::TextureAtlas m_groundTiles;

	sd::GameController* m_controller = nullptr;

	sd::AssetManager<sd::Sound> m_sounds;
	
	sd::ParticleSystem m_particles;
	sd::f32 m_clickParticleDelay = 0.01f;

	sd::Animator m_colourAnimator;
	sd::Animation m_colourAnimation;
	sd::Animation m_flashAnimation;

	sd::Tilemap m_tilemap;

	bool m_startRecording = false;
	sd::RecordingDevice m_device;
	sd::Sound m_chunkSound;
	sd::SoundSource m_source;

public:
	TestScene(sd::Application& application, const sd::String& name)
		: Scene(application, name)
	{
		
	}

	virtual ~TestScene() noexcept = default;

	[[nodiscard]] virtual sd::Status OnLoad() override
	{
		m_application.GetRenderer().SetClearColour(sd::Colour(0.3f, 0.05f, 0.5f, 1.0f));

		const auto textures = sd::vfs::GetAllFilesInDirectory("assets/textures");

		for (const auto& texture : textures)
		{
			const sd::String textureName = sd::fs::GetFileStem(texture);
			m_textures.Add(textureName, texture);

			if (!m_textures[textureName].IsValid())
			{
				return sd::Status::Fail;
			}
		}

		m_crateTexture = &m_textures["crate"];
		m_crumbleTexture = &m_textures["crumble"];

		m_font.SetFont("assets/fonts/TheanoModern.ttf");
		
		if (m_font.Add(128u) != sd::Status::Success)
		{
			return sd::Status::Fail;
		}

		m_textCache.Initialise(m_font[128u]);

		m_glyphTexture = m_textCache['A'];

		m_textTexture = m_textCache.Get(sd::text::UTF16TextInfo{
			.text = u"This is some text. \u0400\u0411\u0414\u042B",
			.outline = sd::text::OutlineInfo{
				.thickness = 4u,
				.innerColour = sd::colours::Red,
				.outerColour = sd::colours::Green,
			},
			.wrapLength = 1024u,
		});

		m_conveyorTextures.Initialise("assets/textures/texture_atlases/conveyors.taj");
		m_colourTextures.Initialise("assets/textures/texture_atlases/colours.taj");
		m_groundTiles.Initialise("assets/textures/texture_atlases/tiles.taj");

		if (!m_conveyorTextures.IsValid() || !m_colourTextures.IsValid() || !m_groundTiles.IsValid())
		{
			return sd::Status::Fail;
		}

		m_sounds.Add("blip", "assets/sounds/blip.wav");

		if (!m_sounds["blip"].IsValid())
		{
			return sd::Status::Fail;
		}

		m_colourAnimation.Initialise("assets/animations/colours.anj", m_colourTextures);
		m_flashAnimation.Initialise("assets/animations/flash.anj", m_colourTextures);

		if (!m_colourAnimation.IsValid() || !m_flashAnimation.IsValid())
		{
			return sd::Status::Fail;
		}

		m_colourAnimator.AddAnimation("dance", m_colourAnimation, true);
		m_colourAnimator.AddAnimation("flash", m_flashAnimation);
		m_colourAnimator.SetSpeed(0.25f);

		m_tilemap.Initialise("assets/tilemaps/ground.json");

		if (!m_tilemap.IsValid())
		{
			return sd::Status::Fail;
		}

		m_tilemap.SetPosition(sd::Vec2{ -10.0f, 5.0f });
		m_tilemap.AddTiles(m_groundTiles);
		m_tilemap.AddTiles(m_conveyorTextures);

		GetInputManager().AddToButton("quit", sd::KeyCode::Escape);
		GetInputManager().AddToButton("outline", sd::KeyCode::Space);
		GetInputManager().AddToButton("speed", sd::KeyCode::LeftShift);
		GetInputManager().AddToButton("coords", sd::MouseButton::Thumb1);

		GetInputManager().AddToButton("play_sound", sd::KeyCode::P);
		GetInputManager().AddToButton("play_sound", sd::GameControllerButton::Y);

		GetInputManager().AddToButton("reset", sd::KeyCode::Tab);
		GetInputManager().AddToButton("reset", sd::GameControllerButton::RightShoulder);

		GetInputManager().AddToButton("particle", sd::MouseButton::Left);
		GetInputManager().AddToButton("particle", sd::GameControllerButton::A);

		GetInputManager().AddToAxis("scroll", sd::AxisType::MouseScroll);

		GetInputManager().AddToAxis("x", sd::AxisType::ADKeys, true);
		GetInputManager().AddToAxis("x", sd::AxisType::LeftRightKeys, true);
		GetInputManager().AddToAxis("x", sd::AxisType::ControllerRightX, true);

		GetInputManager().AddToAxis("y", sd::AxisType::WSKeys, true);
		GetInputManager().AddToAxis("y", sd::AxisType::UpDownKeys, true);
		GetInputManager().AddToAxis("y", sd::AxisType::ControllerRightY, false);

		if (GetScriptEngine().LoadScript("assets/scripts/test.lua") != sd::Status::Success)
		{
			return sd::Status::Fail;
		}

		GetScriptEngine().Set("a", 13);
		sd::Log::Trace("{} {} {}", (sd::i32)GetScriptEngine()["a"], GetScriptEngine().Get<sd::i32>("b"), GetScriptEngine().Get<sd::i32>("c"));

		GetScriptEngine().CallFunction<void, sd::String>("print_stuff", "Script attached.");
		GetScriptEngine().CallFunction<void>("vector_stuff");

		m_device.Initialise(sd::RecordingDevice::GetAllDeviceInfos().back());
		m_device.Open();

		return sd::Status::Success;
	}

	virtual void OnUnload() noexcept
	{ }

	virtual void FixedUpdate(const sd::f32 fixedDeltaTime) override
	{ }

	virtual void ProcessInput() override
	{
		if (m_application.HasWindowFocus())
		{
			if (GetInputManager().IsButtonDown("quit"))
			{
				m_application.FinishCurrentScene();
			}

			if (GetInputManager().IsButtonDown("outline"))
			{
				GetRenderer().SetPolygonMode(sd::Renderer::PolygonMode::Outline);
			}
			else if (GetInputManager().IsButtonUp("outline"))
			{
				GetRenderer().SetPolygonMode(sd::Renderer::PolygonMode::Filled);
			}

			if (GetInputManager().IsButtonDown("speed"))
			{
				m_colourAnimator.SetSpeed(4.0f);
			}
			else if (GetInputManager().IsButtonUp("speed"))
			{
				m_colourAnimator.SetSpeed(0.25f);
			}

			if (GetInputManager().IsButtonUp("coords"))
			{
				const sd::Vec2 mouseClick = GetCamera().ScreenSpaceToWorldSpace(GetMouseState().GetProportionalCoordinates(GetRenderer()));

				sd::Log::Trace("Screen: {} {}; World: {} {}", GetMouseState().GetProportionalCoordinates(GetRenderer()).x, GetMouseState().GetProportionalCoordinates(GetRenderer()).y, mouseClick.x, mouseClick.y);
			}

			GetCamera().SetZoom(GetCamera().GetZoom() + GetInputManager().GetAxis("scroll") * 0.1f);

			if (GetInputManager().IsButtonDown("reset", { m_controller }))
			{
				GetCamera().ResetTransform();
			}

			if (m_controller != nullptr)
			{
				GetCamera().SetRotation(GetCamera().GetRotation() - (m_controller->GetGyroscopeData().z * 0.04f));
			}

			if (GetInputManager().IsButtonDown("play_sound", { m_controller }))
			{
				GetSoundSystem().PlaySound(m_sounds["blip"]);
			}

			if (GetInputManager().IsButtonPressed("particle", { m_controller }) && m_clickParticleDelay < 0.0f)
			{
				m_clickParticleDelay = 0.01f;

				auto finalColour = sd::colours::Yellow;
				finalColour.alpha = 0u;

				m_particles.Emit(sd::ParticleSystem::ParticleData{
					.initialPosition = GetMouseState().GetProportionalCoordinates(GetRenderer()),
					.initialRotation = 0.0f,
					.minVelocity = { -100.0f, -400.0f },
					.maxVelocity = { 100.0f, -10.0f },
					.acceleration = 0.4f,
					.minAngularVelocity = 0.0f,
					.maxAngularVelocity = 180.0f,
					.angularAcceleration = -0.1f,
					.isAffectedByGravity = false,
					.minSize = { 20.0f, 20.0f },
					.maxSize = { 40.0f, 40.0f },
					.sizeUpdateMultipler = -0.2f,
					.keepAsSquare = true,
					.shiftToCentre = true,
					.startColour = sd::colours::Red,
					.endColour = finalColour,
					.texture = nullptr,  
					.textureArea = std::nullopt,
					.minLifetime = 0.5f,
					.maxLifetime = 1.0f,
				});
			}

			if (GetKeyboardState().IsKeyDown(sd::KeyCode::Z))
			{
				m_colourAnimator.SetCurrentAnimation("flash");
			}
			else if (GetKeyboardState().IsKeyUp(sd::KeyCode::Z))
			{
				m_colourAnimator.SetCurrentAnimation("dance");
			}

			if (GetKeyboardState().IsKeyDown(sd::KeyCode::RightShift))
			{
				const sd::f64 elapsedTime = GetScriptEngine().GetFunction<sd::f64()>("get_time")();
				sd::Log::Trace("Elapsed time: {}", elapsedTime);
			}

			if (GetKeyboardState().IsKeyDown(sd::KeyCode::V))
			{
				m_device.StartRecording();
			}

			if (GetKeyboardState().IsKeyUp(sd::KeyCode::V))
			{
				m_device.StopRecording();
				m_device.ClearPCMChunks();
			}

			if (GetKeyboardState().IsKeyDown(sd::KeyCode::X))
			{
				m_colourAnimator.SkipToFrame(16u);
			}
		}
	}

	virtual void Update(const sd::f32 deltaTime) override
	{
		GetCamera().SetPosition(
			GetCamera().GetPosition().x + GetInputManager().GetAxis("x", { m_controller }) * 10.0f * deltaTime,
			GetCamera().GetPosition().y + GetInputManager().GetAxis("y", { m_controller }) * 10.0f * deltaTime,
			GetCamera().GetPosition().z
		);

		m_clickParticleDelay -= deltaTime;
		m_particles.Update(deltaTime);

		m_colourAnimator.Update(deltaTime);

		if (!m_source.HasValidHandle() || m_source.IsStopped())
		{
			if (m_device.HasPCMChunk())
			{
				const auto pcmChunk = m_device.DequeuePCMChunk();
				m_chunkSound.Initialise(pcmChunk, m_device.GetFrequency(), m_device.GetChannelCount());

				m_source = GetSoundSystem().PlaySound(m_chunkSound);
			}
		}
	}

	virtual void LateUpdate(const sd::f32 deltaTime) override { }

	virtual void Render(sd::Renderer& renderer) override
	{
		for (const auto& layer : m_tilemap.GetLayers())
		{
			layer.Render(renderer, GetCamera());
		}

		for (sd::i32 x = -3; x <= 3; ++x)
		{
			for (sd::i32 y = -3; y <= 3; ++y)
			{
				renderer.DrawWorldRect(
					sd::comp::Transform(
						sd::Vec2{ x, y },
						2.5f,
						sd::NullOpt,
						sd::Vec2{ 1.0f, 1.0f }
					),
					sd::comp::ShearTransform(10.0f),
					sd::comp::Sprite((x + y) % 2 == 0 ? *m_crateTexture : *m_crumbleTexture),
					GetCamera()
				);
			}
		}

		renderer.DrawWorldRect(
			sd::comp::Transform(
				sd::Vec2{ 1.0f, 1.0f },
				GetElapsedTime() * 100.0f,
				sd::Vec2{ -1.0f, -1.0f },
				sd::Vec2{ 2.0f, 2.0f }
			),
			sd::Colour(1.0f, 1.0f, 0.0f, 0.5f),
			GetCamera()
		);

		renderer.DrawWorldQuad(
			sd::Quad{
				.lowerLeft = { -0.5f, -0.5f },
				.upperLeft = { -0.225f, 0.4f },
				.upperRight = { 0.5f, 0.5f },
				.lowerRight = { 0.4f, -0.5f }
			},
			sd::comp::Transform(
				sd::Vec2{ -6.0f, 0.0f },
				15.0f,
				sd::NullOpt,
				sd::Vec2{ 2.0f, 2.0f }
			),
			sd::comp::ShearTransform(30.0f),
			sd::colours::Blue,
			GetCamera()
		);

		renderer.DrawWorldQuad(
			sd::Quad{
				.lowerLeft = { -0.4f, -0.5f },
				.upperLeft = { -0.5f, 0.5f },
				.upperRight = { 0.25f, 0.4f },
				.lowerRight = { 0.5f, -0.5f }
			},
			sd::comp::Transform(
				sd::Vec2{ 6.0f, 0.0f },
				-15.0f,
				sd::NullOpt,
				sd::Vec2{ 2.0f, 2.0f }
			),
			sd::comp::ShearTransform(-30.0f),
			sd::comp::Sprite(
				*m_crateTexture, sd::Pair<sd::Vec2, sd::Vec2>{ { 0.25f, 0.25f }, { 1.0f, 1.0f } }, sd::colours::Lime
			),
			GetCamera()
		);

		renderer.DrawScreenRect(
			sd::comp::ScreenTransform(sd::IVec2{ 50, 50 }, sd::UVec2{ 100u, 200u }),
			sd::comp::ShearTransform(0.0f, 5.0f),
			sd::colours::Lime
		);

		renderer.DrawScreenRect(
			sd::comp::ScreenTransform(sd::IVec2{ 150, 50 }, sd::UVec2{ 100u, 200u }),
			sd::comp::ShearTransform(0.0f, -5.0f),
			sd::comp::Sprite(m_glyphTexture)
		);

		renderer.DrawScreenRect(
			sd::comp::ScreenTransform(sd::IVec2{ 200, 800 }, m_textTexture->GetSize() / 3u),
			sd::comp::Sprite(m_textTexture)
		);

		sd::i32 offset = 0;

		for (const auto& word : { "one", "two", "three", "four", "five" })
		{
			const auto texture = m_textCache[word];
			
			renderer.DrawScreenRect(
				sd::comp::ScreenTransform(sd::IVec2{ 1400, 100 + offset }, texture->GetSize() / 2u),
				sd::comp::Sprite(texture)
			);
			
			offset += texture->GetSize().y / 2 + 10;
		}

		renderer.DrawWorldRect(
			sd::comp::Transform(sd::Vec2(-6.5f, 3.0f), 0.0f, sd::NullOpt, sd::Vec2{ 0.9f, 0.9f }),
			sd::comp::Sprite(m_colourTextures.GetTexture(), m_colourTextures["red"]),
			GetCamera()
		);

		renderer.DrawWorldRect(
			sd::comp::Transform(sd::Vec2(-5.0f, 3.0f), 0.0f, sd::NullOpt, sd::Vec2{ 0.9f, 0.9f }),
			sd::comp::Sprite(m_colourTextures.GetTexture(), m_colourTextures["green"]),
			GetCamera()
		);

		renderer.DrawWorldRect(
			sd::comp::Transform(sd::Vec2(-6.5f, 1.5f), 0.0f, sd::NullOpt, sd::Vec2{ 0.9f, 0.9f }),
			sd::comp::Sprite(m_colourTextures.GetTexture(), m_colourTextures["blue"]),
			GetCamera()
		);

		renderer.DrawWorldRect(
			sd::comp::Transform(sd::Vec2(-5.0f, 1.5f), 0.0f, sd::NullOpt, sd::Vec2{ 0.9f, 0.9f }),
			sd::comp::Sprite(m_colourTextures.GetTexture(), m_colourTextures["yellow"]),
			GetCamera()
		);

		renderer.DrawWorldRect(
			sd::comp::Transform(m_colourAnimator.GetPositionOffset(), m_colourAnimator.GetRotation(), sd::NullOpt, m_colourAnimator.GetScale()),
			sd::comp::ShearTransform(m_colourAnimator.GetShear()),
			sd::comp::Sprite(m_colourTextures.GetTexture(), m_colourAnimator.GetSprite(), m_colourAnimator.GetColour()),
			GetCamera()
		);

		m_particles.RenderOnScreen(renderer);
	}

	virtual void PollEvent(const SDL_Event& event) override
	{
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11)
		{
			if (!GetWindow().IsFullscreen())
			{
				GetWindow().SetBorderless(true);
			}

			GetWindow().ToggleFullscreen();

			if (!GetWindow().IsFullscreen())
			{
				GetWindow().SetBorderless(false);
			}
		}
	}

	virtual void OnGameControllerAdded(sd::GameController& gameController) override
	{
		if (m_controller == nullptr)
		{
			m_controller = &gameController;
			m_controller->SetPlayerIndex(0u);
			m_controller->SetLED(sd::colours::Purple);
		}
	}

	virtual void OnGameControllerRemoved(const sd::GameController& gameController) override
	{
		m_controller = nullptr;
	}
};

#endif