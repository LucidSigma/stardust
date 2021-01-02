#pragma once
#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include <stardust/Stardust.h>

#include <glad/glad.h>

class TestScene final
	: public sd::Scene
{
private:
	sd::SortingLayer m_defaultSortingLayer{ "default" };

	sd::Texture m_crateTexture;
	sd::Texture m_crumbleTexture;

	sd::Font m_font;
	sd::Texture m_glyphTexture;
	sd::Texture m_textTexture;
	sd::TextureAtlas m_conveyorTextures;
	sd::TextureAtlas m_colourTextures;

	sd::GameController* m_controller = nullptr;

	sd::AssetManager<sd::Sound> m_sounds;
	
	sd::ParticleSystem m_particles;
	sd::f32 m_clickParticleDelay = 0.01f;

	sd::Animator m_colourAnimator;
	sd::Animation m_colourAnimation;
	sd::Animation m_flashAnimation;

public:
	TestScene(sd::Application& application, const sd::String& name)
		: Scene(application, name)
	{

	}

	virtual ~TestScene() noexcept = default;

	[[nodiscard]] virtual sd::Status OnLoad() override
	{
		m_application.GetRenderer().SetClearColour(sd::CreateColour(0.3f, 0.05f, 0.5f, 1.0f));

		m_crateTexture.Initialise("assets/textures/crate.png");
		m_crumbleTexture.Initialise("assets/textures/crumble.png");

		if (!m_crateTexture.IsValid() || !m_crumbleTexture.IsValid())
		{
			return sd::Status::Fail;
		}

		m_font.Initialise("assets/fonts/TheanoModern.ttf", 128u);

		if (!m_font.IsValid())
		{
			return sd::Status::Fail;
		}

		m_glyphTexture = sd::text::RenderGlyph(m_font, 'A', sd::colours::White);
		m_textTexture = sd::text::RenderWrappedTextWithOutline(m_font, u"This is some text. \u0400\u0411\u0414\u042B", sd::colours::Red, 4u, sd::colours::Green, 1024u);

		m_conveyorTextures.Initialise("assets/textures/texture_atlases/conveyors.taj");
		m_colourTextures.Initialise("assets/textures/texture_atlases/colours.taj");

		if (!m_conveyorTextures.IsTextureValid() || !m_colourTextures.IsTextureValid())
		{
			return sd::Status::Fail;
		}

		m_sounds.Add("blip", "assets/sounds/blip.wav");

		if (!m_sounds["blip"].IsValid())
		{
			return sd::Status::Fail;
		}

		m_particles.Initialise(m_defaultSortingLayer);

		m_colourAnimation.Initialise("assets/animations/colours.anj", m_colourTextures);
		m_flashAnimation.Initialise("assets/animations/flash.anj", m_colourTextures);

		if (!m_colourAnimation.IsValid() || !m_flashAnimation.IsValid())
		{
			return sd::Status::Fail;
		}

		m_colourAnimator.AddAnimation("dance", m_colourAnimation, true);
		m_colourAnimator.AddAnimation("flash", m_flashAnimation);

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

		GetInputManager().AddToPositiveAxis("x", { sd::KeyCode::A, sd::KeyCode::Left });
		GetInputManager().AddToNegativeAxis("x", { sd::KeyCode::D, sd::KeyCode::Right });
		GetInputManager().AddToAxis("x", sd::AxisType::ControllerRightX, true);

		GetInputManager().AddToPositiveAxis("y", { sd::KeyCode::S, sd::KeyCode::Down });
		GetInputManager().AddToNegativeAxis("y", { sd::KeyCode::W, sd::KeyCode::Up });
		GetInputManager().AddToAxis("y", sd::AxisType::ControllerRightY, false);

		return sd::Status::Success;
	}

	virtual void OnUnload() noexcept
	{
		m_crateTexture.Destroy();
		m_crumbleTexture.Destroy();

		m_font.Destroy();
	}

	virtual void FixedUpdate(const sd::f32 fixedDeltaTime) override { }

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
				m_colourAnimator.SetSpeed(1.0f);
			}

			if (GetInputManager().IsButtonUp("coords"))
			{
				const sd::Vec2 mouseClick = GetCamera().ScreenSpaceToWorldSpace(GetMouseState().GetProportionalCoordinates(GetRenderer()));

				sd::Log::Trace("Screen: {} {}; World: {} {}", GetMouseState().GetProportionalCoordinates(GetRenderer()).x, GetMouseState().GetProportionalCoordinates(GetRenderer()).y, mouseClick.x, mouseClick.y);
			}

			GetCamera().SetZoom(GetCamera().GetZoom() + GetInputManager().GetAxis("scroll") * 0.1f);

			if (GetInputManager().IsButtonDown("reset", { m_controller }))
			{
				GetCamera().SetRotation(0.0f);
				GetCamera().SetPosition(sd::Vec3{ 0.0f, 0.0f, 0.0f });
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
				finalColour.a = 0u;

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

			if (GetKeyboardState().IsKeyDown(sd::KeyCode::V))
			{
				m_colourAnimator.SetCurrentAnimation("flash");
			}
			else if (GetKeyboardState().IsKeyUp(sd::KeyCode::V))
			{
				m_colourAnimator.SetCurrentAnimation("dance");
			}
		}
	}

	virtual void Update(const sd::f32 deltaTime) override
	{
		GetCamera().SetPosition(
			GetCamera().GetPosition().x + GetInputManager().GetAxis("x", { m_controller }) * 4.0f * deltaTime,
			GetCamera().GetPosition().y + GetInputManager().GetAxis("y", { m_controller }) * 4.0f * deltaTime,
			GetCamera().GetPosition().z
		);

		m_clickParticleDelay -= deltaTime;
		m_particles.Update(deltaTime);

		m_colourAnimator.Update(deltaTime);
	}

	virtual void LateUpdate(const sd::f32 deltaTime) override { }

	virtual void Render(sd::Renderer& renderer) override
	{
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
					sd::comp::SpriteRender((x + y) % 2 == 0 ? m_crateTexture : m_crumbleTexture, m_defaultSortingLayer),
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
			sd::CreateColour(1.0f, 1.0f, 0.0f, 0.5f),
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
			sd::comp::SpriteRender(
				m_crateTexture, m_defaultSortingLayer, sd::Pair<sd::Vec2, sd::Vec2>{ { 0.25f, 0.25f }, { 1.0f, 1.0f } }, sd::colours::Lime
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
			sd::comp::SpriteRender(m_glyphTexture, m_defaultSortingLayer)
		);

		renderer.DrawScreenRect(
			sd::comp::ScreenTransform(sd::IVec2{ 200, 800 }, m_textTexture.GetSize() / 3u),
			sd::comp::SpriteRender(m_textTexture, m_defaultSortingLayer)
		);

		renderer.DrawWorldRect(
			sd::comp::Transform(sd::Vec2(4.5f, -2.0f)),
			sd::comp::SpriteRender(m_conveyorTextures.GetTexture(), m_defaultSortingLayer, m_conveyorTextures["left"]),
			GetCamera()
		);

		renderer.DrawWorldRect(
			sd::comp::Transform(sd::Vec2(6.5f, -2.0f)),
			sd::comp::SpriteRender(m_conveyorTextures.GetTexture(), m_defaultSortingLayer, m_conveyorTextures["right"]),
			GetCamera()
		);

		renderer.DrawWorldRect(
			sd::comp::Transform(sd::Vec2(-6.5f, 3.0f), 0.0f, sd::NullOpt, sd::Vec2{ 0.9f, 0.9f }),
			sd::comp::SpriteRender(m_colourTextures.GetTexture(), m_defaultSortingLayer, m_colourTextures["red"]),
			GetCamera()
		);

		renderer.DrawWorldRect(
			sd::comp::Transform(sd::Vec2(-5.0f, 3.0f), 0.0f, sd::NullOpt, sd::Vec2{ 0.9f, 0.9f }),
			sd::comp::SpriteRender(m_colourTextures.GetTexture(), m_defaultSortingLayer, m_colourTextures["green"]),
			GetCamera()
		);

		renderer.DrawWorldRect(
			sd::comp::Transform(sd::Vec2(-6.5f, 1.5f), 0.0f, sd::NullOpt, sd::Vec2{ 0.9f, 0.9f }),
			sd::comp::SpriteRender(m_colourTextures.GetTexture(), m_defaultSortingLayer, m_colourTextures["blue"]),
			GetCamera()
		);

		renderer.DrawWorldRect(
			sd::comp::Transform(sd::Vec2(-5.0f, 1.5f), 0.0f, sd::NullOpt, sd::Vec2{ 0.9f, 0.9f }),
			sd::comp::SpriteRender(m_colourTextures.GetTexture(), m_defaultSortingLayer, m_colourTextures["yellow"]),
			GetCamera()
		);

		renderer.DrawWorldRect(
			sd::comp::Transform(m_colourAnimator.GetPositionOffset(), m_colourAnimator.GetRotation(), sd::NullOpt, m_colourAnimator.GetScale()),
			sd::comp::ShearTransform(m_colourAnimator.GetShear()),
			sd::comp::SpriteRender(m_colourTextures.GetTexture(), m_defaultSortingLayer, m_colourAnimator.GetSprite(), m_colourAnimator.GetColour()),
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