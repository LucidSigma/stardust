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

	sd::GameController* m_controller = nullptr;

	sd::AssetManager<sd::Sound> m_sounds;

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

		if (!m_conveyorTextures.IsTextureValid())
		{
			return sd::Status::Fail;
		}

		m_sounds.Add("blip", "assets/sounds/blip.wav");

		if (!m_sounds["blip"].IsValid())
		{
			return sd::Status::Fail;
		}

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
			if (GetKeyboardState().IsKeyDown(sd::KeyCode::Escape))
			{
				m_application.FinishCurrentScene();
			}

			if (GetKeyboardState().IsKeyDown(sd::KeyCode::Space))
			{
				GetRenderer().SetPolygonMode(sd::Renderer::PolygonMode::Outline);
			}
			else if (GetKeyboardState().IsKeyUp(sd::KeyCode::Space))
			{
				GetRenderer().SetPolygonMode(sd::Renderer::PolygonMode::Filled);
			}

			if (GetMouseState().IsButtonDown(sd::MouseButton::Thumb1))
			{
				const sd::Vec2 mouseClick = GetCamera().ScreenSpaceToWorldSpace(GetMouseState().GetProportionalCoordinates(GetRenderer()));

				sd::Log::Trace("Screen: {} {}; World: {} {}", GetMouseState().GetProportionalCoordinates(GetRenderer()).x, GetMouseState().GetProportionalCoordinates(GetRenderer()).y, mouseClick.x, mouseClick.y);
			}

			if (GetMouseState().GetScrollAmount() != 0)
			{
				sd::Log::Trace("{}", GetMouseState().GetScrollAmount());
			}

			if (m_controller != nullptr)
			{
				const auto& touchpadFingers = m_controller->GetTouchpadFingers();

				if (touchpadFingers[0].isTouching && m_controller->IsButtonDown(sd::GameControllerButton::A))
				{
					sd::Log::Trace("{}, {}: {}", touchpadFingers[0].position.x, touchpadFingers[0].position.y, touchpadFingers[0].pressure);
				}

				GetCamera().SetRotation(GetCamera().GetRotation() - (m_controller->GetGyroscopeData().z * 0.04f));
				//GetCamera().SetPosition(
				//	GetCamera().GetPosition().x + m_controller->GetAccelerometerData().x * 0.005f,
				//	GetCamera().GetPosition().y + m_controller->GetAccelerometerData().z * 0.005f,
				//	GetCamera().GetPosition().z
				//);

				if (m_controller->GetAxes().rightTrigger > 0)
				{
					GetCamera().SetRotation(0.0f);
					GetCamera().SetPosition(sd::Vec3{ 0.0f, 0.0f, 0.0f });
				}

				if (m_controller->IsButtonDown(sd::GameControllerButton::LeftShoulder))
				{
					m_controller->Rumble(0.5f, 0.5f, 1'000u);
				}
				else if (m_controller->IsButtonDown(sd::GameControllerButton::RightShoulder))
				{
					m_controller->Rumble(1.0f, 1.0f, 1'000u);
				}
			}

			if (GetKeyboardState().IsKeyDown(sd::KeyCode::P) || (m_controller != nullptr && m_controller->IsButtonDown(sd::GameControllerButton::Y)))
			{
				GetSoundSystem().PlaySound(m_sounds["blip"]);
			}
		}
	}

	virtual void Update(const sd::f32 deltaTime) override
	{
		
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