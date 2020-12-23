#include <stardust/Stardust.h>

#include <glad/glad.h>

class TestScene final
	: public sd::Scene
{
private:
	

public:
	TestScene(sd::Application& application, const sd::String& name)
		: Scene(application, name)
	{

	}

	virtual ~TestScene() noexcept = default;

	[[nodiscard]] virtual sd::Status OnLoad() override
	{
		m_application.GetRenderer().SetClearColour(sd::CreateColour(0.3f, 0.05f, 0.5f, 1.0f));

		return sd::Status::Success;
	}

	virtual void OnUnload() noexcept
	{
		
	}

	virtual void FixedUpdate(const sd::f32 fixedDeltaTime) override { }

	virtual void ProcessInput() override
	{

	}

	virtual void Update(const sd::f32 deltaTime) override
	{
		
	}

	virtual void LateUpdate(const sd::f32 deltaTime) override { }

	virtual void Render(const sd::Renderer& renderer) const override
	{
		m_application.GetRenderer().DrawWorldRect(m_application.GetCamera(), { 0.0f, 0.0f }, { 3.0f, 0.5f }, sd::CreateColour(1.0f, 0.5f, 0.2f, 1.0f), m_application.GetElapsedTime() * 50.0f);
		m_application.GetRenderer().DrawScreenRect({ 50, 50 }, { 100u, 200u }, sd::colours::Lime);
	}

	virtual void PollEvent(const SDL_Event& event) override
	{
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
		{
			m_application.FinishCurrentScene();
		}
	}
};