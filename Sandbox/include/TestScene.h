#pragma once
#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include <stardust/Stardust.h>

#include <glad/glad.h>

class TestScene final
	: public sd::Scene
{
private:
	sd::Texture m_crateTexture;
	sd::Texture m_crumbleTexture;

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

		return sd::Status::Success;
	}

	virtual void OnUnload() noexcept
	{
		m_crateTexture.Destroy();
	}

	virtual void FixedUpdate(const sd::f32 fixedDeltaTime) override { }

	virtual void ProcessInput() override
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
	}

	virtual void Update(const sd::f32 deltaTime) override
	{
		GetRenderer().BeginFrame();
	}

	virtual void LateUpdate(const sd::f32 deltaTime) override { }

	virtual void Render(sd::Renderer& renderer) override
	{
		//GetRenderer().DrawScreenRect({ 1, 1 }, { 2u, 2u }, sd::colours::Red);
		//
		//GetRenderer().DrawScreenQuad({ sd::IVec2{ 500, 50 }, sd::IVec2{ 600, 250 }, sd::IVec2{ 700, 200 }, sd::IVec2{ 800, 100 } }, sd::colours::Beige);
		//
		//GetRenderer().DrawTexturedScreenRect(m_crateTexture, { 400, 400 }, { 3.0f, 2.0f }, sd::FlipType::None, sd::colours::Green, GetElapsedTime() * 50.0f, sd::IVec2{ 250, 10 });
		//
		//GetRenderer().DrawTexturedScreenQuad(m_crateTexture, { sd::IVec2{ 250, 25 }, sd::IVec2{ 300, 125 }, sd::IVec2{ 350, 100 }, sd::IVec2{ 400, 50 } });

		for (sd::i32 x = -3; x <= 3; ++x)
		{
			for (sd::i32 y = -3; y <= 3; ++y)
			{
				renderer.DrawWorldRect(
					sd::comp::Transform(
						sd::Vec2{ x, y },
						1.0f,
						sd::NullOpt,
						sd::Vec2{ 1.0f, 1.0f }
					),
					sd::comp::ShearTransform(10.0f),
					sd::comp::SpriteRender((x + y) % 2 == 0 ? m_crateTexture : m_crumbleTexture),
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
			{ sd::Vec2{ -0.5f, -0.5f }, sd::Vec2{ -0.25f, 0.4f }, sd::Vec2{ 0.5f, 0.5f }, sd::Vec2{ 0.4f, -0.5f } },
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
			{ sd::Vec2{ -0.4f, -0.5f }, sd::Vec2{ -0.5f, 0.5f }, sd::Vec2{ 0.25f, 0.4f }, sd::Vec2{ 0.5f, -0.5f } },
			sd::comp::Transform(
				sd::Vec2{ 6.0f, 0.0f },
				-15.0f,
				sd::NullOpt,
				sd::Vec2{ 2.0f, 2.0f }
			),
			sd::comp::ShearTransform(-30.0f),
			sd::comp::SpriteRender(
				m_crateTexture, sd::Pair<sd::Vec2, sd::Vec2>{ { 0.25f, 0.25f }, { 1.0f, 1.0f } }, 0.0f, sd::colours::Lime
			),
			GetCamera()
		);

		GetRenderer().DrawScreenRect(
			sd::comp::ScreenTransform(sd::IVec2{ 50, 50 }, sd::UVec2{ 100u, 200u }),
			sd::colours::Lime
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
};

#endif