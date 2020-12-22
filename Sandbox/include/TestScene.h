#include <stardust/Stardust.h>

#include <glad/glad.h>

class TestScene final
	: public sd::Scene
{
private:
	sd::VertexBuffer m_vertexBuffer;
	sd::IndexBuffer m_indexBuffer;
	sd::VertexLayout m_vertexLayout;

	sd::Shader m_vertexShader;
	sd::Shader m_fragmentShader;
	
	sd::ShaderProgram m_shaderProgram;

	inline static sd::Vector<sd::f32> s_vertices{
		-0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
	};

	inline static sd::Vector<sd::u32> s_indices{
		0u, 2u, 1u,
		1u, 2u, 3u,
	};

public:
	TestScene(sd::Application& application, const sd::String& name)
		: Scene(application, name)
	{

	}

	virtual ~TestScene() noexcept = default;

	[[nodiscard]] virtual sd::Status OnLoad() override
	{
		m_application.GetRenderer().SetClearColour(sd::CreateColour(0.3f, 0.05f, 0.5f, 1.0f));

		m_vertexBuffer.Initialise(s_vertices);
		m_indexBuffer.Initialise(s_indices);

		m_vertexLayout.AddAttribute({
			// Position.
			.elementCount = 2u,
			.dataType = GL_FLOAT,
			.isNormalised = true,
		})
		.AddAttribute({
			// Texture coordinate.
			.elementCount = 2u,
			.dataType = GL_FLOAT,
			.isNormalised = true,
		})
		.AddVertexBuffer(m_vertexBuffer)
		.Initialise();

		m_vertexShader.Initialise(sd::Shader::Type::Vertex, "assets/shaders/quad.vert");
		m_fragmentShader.Initialise(sd::Shader::Type::Fragment, "assets/shaders/quad.frag");
		
		if (!m_vertexShader.IsValid() || !m_fragmentShader.IsValid())
		{
			return sd::Status::Fail;
		}
		
		m_shaderProgram.Initialise({ &m_vertexShader, &m_fragmentShader });
		
		if (!m_shaderProgram.IsValid())
		{
			return sd::Status::Fail;
		}

		return sd::Status::Success;
	}

	virtual void OnUnload() noexcept
	{
		m_vertexLayout.Destroy();
		m_vertexBuffer.Destroy();
		m_indexBuffer.Destroy();
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
		m_shaderProgram.Use();

		m_shaderProgram.SetUniform<sd::Mat4>("u_MVP", sd::Mat4{ 1.0f });
		m_shaderProgram.SetUniform<sd::Vec4>("u_Colour", sd::Vec4{ 1.0f, 0.5f, 0.2f, 1.0f });

		m_vertexLayout.Bind();
		m_vertexLayout.DrawIndexed(m_indexBuffer);
		m_vertexLayout.Unbind();

		m_shaderProgram.Disuse();
	}

	virtual void PollEvent(const SDL_Event& event) override
	{
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
		{
			m_application.FinishCurrentScene();
		}
	}
};