#include <stardust/Stardust.h>

class TestScene final
	: public sd::Scene
{
public:
	TestScene(sd::Application& application, const sd::String& name)
		: Scene(application, name)
	{

	}
	virtual ~TestScene() noexcept = default;

	[[nodiscard]] virtual sd::Status OnLoad() override
	{
		return sd::Status::Success;
	}

	virtual void OnUnload() noexcept { }

	virtual void FixedUpdate(const sd::f32 fixedDeltaTime) override { }

	virtual void ProcessInput() override
	{

	}

	virtual void Update(const sd::f32 deltaTime)override { }
	virtual void LateUpdate(const sd::f32 deltaTime) override { }

	// virtual void Render(const Renderer& renderer) const override { };

	virtual void PollEvent(const SDL_Event& event) override
	{
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
		{
			m_application.FinishCurrentScene();
		}
	}
};