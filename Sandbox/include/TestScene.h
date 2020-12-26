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
		
	}

	virtual void LateUpdate(const sd::f32 deltaTime) override { }

	virtual void Render(const sd::Renderer& renderer) const override
	{
		GetRenderer().DrawWorldRect(GetCamera(), { 0.0f, 0.0f }, { 3.0f, 0.5f }, sd::CreateColour(1.0f, 0.5f, 0.2f, 1.0f), GetElapsedTime() * 50.0f);
		GetRenderer().DrawScreenRect({ 50, 50 }, { 100u, 200u }, sd::colours::Lime);

		GetRenderer().DrawScreenRect({ 1, 1 }, { 2u, 2u }, sd::colours::Red);

		GetRenderer().DrawWorldQuad(GetCamera(), { sd::Vec2{ 5.0f, 4.0f }, sd::Vec2{ 4.5f, -3.0f }, sd::Vec2{ 6.5f, -2.5f }, sd::Vec2{ 5.5f, 3.5f } }, sd::colours::Blue);
		GetRenderer().DrawScreenQuad({ sd::IVec2{ 500, 50 }, sd::IVec2{ 600, 250 }, sd::IVec2{ 700, 200 }, sd::IVec2{ 800, 100 } }, sd::colours::Beige);

		GetRenderer().DrawTexturedWorldRect(GetCamera(), m_crateTexture, { -1, -1 }, { 4.0f, 4.0f }, sd::colours::Maroon);
		GetRenderer().DrawTexturedScreenRect(m_crateTexture, { 400, 400 }, { 3.0f, 2.0f }, sd::FlipType::None, sd::colours::Green, GetElapsedTime() * 50.0f, sd::IVec2{ 250, 10 });

		GetRenderer().DrawTexturedWorldQuad(GetCamera(), m_crateTexture, { sd::Vec2{ 2.5f, 2.0f }, sd::Vec2{ 2.25f, -1.5f }, sd::Vec2{ 3.25f, -1.25f }, sd::Vec2{ 2.75f, 1.75f } }, sd::Vec2{ 0.0f, 0.0f }, 30.0f);
		GetRenderer().DrawTexturedScreenQuad(m_crateTexture, { sd::IVec2{ 250, 25 }, sd::IVec2{ 300, 125 }, sd::IVec2{ 350, 100 }, sd::IVec2{ 400, 50 } });

		GetRenderer().BatchWorldRect(GetCamera(), m_crumbleTexture, m_crateTexture);
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