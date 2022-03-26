#include <atomic>
#include <cstdlib>

#include <SDL2/SDL.h>
#include <stardust/Stardust.h>
#include <stb/stb_image.h>

auto main(const sd::i32 argc, char** const argv) -> sd::i32
{
    sd::Log::Initialise("renderer test", "log.txt");

    STARDUST_ASSERT_RELEASE(SDL_Init(SDL_INIT_EVERYTHING) == 0);
    STARDUST_ASSERT_RELEASE(SDL_SetHintWithPriority(SDL_HINT_OPENGL_ES_DRIVER, "1", SDL_HINT_OVERRIDE) == SDL_TRUE);
    STARDUST_ASSERT_RELEASE(SDL_SetHintWithPriority(SDL_HINT_VIDEO_WIN_D3DCOMPILER, "none", SDL_HINT_OVERRIDE) == SDL_TRUE);

    STARDUST_ASSERT_RELEASE(sd::fs::InitialiseApplicationBaseDirectory() == sd::Status::Success);
    STARDUST_ASSERT_RELEASE(sd::vfs::Initialise(argv[0]) == sd::Status::Success);
    STARDUST_ASSERT_RELEASE(sd::vfs::AddToSearchPath(sd::fs::GetApplicationBaseDirectory() + "../test_resources/render_assets.zip") == sd::Status::Success);

    const sd::List<sd::Pair<SDL_GLattr, sd::i32>> openGLWindowAttributes{
        { SDL_GL_CONTEXT_EGL, SDL_TRUE },
        { SDL_GL_CONTEXT_MAJOR_VERSION, 3 },
        { SDL_GL_CONTEXT_MINOR_VERSION, 0 },
        { SDL_GL_DOUBLEBUFFER, SDL_TRUE },

        { SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES },
        { SDL_GL_ACCELERATED_VISUAL, SDL_TRUE },

        { SDL_GL_RED_SIZE, 8 },
        { SDL_GL_GREEN_SIZE, 8 },
        { SDL_GL_BLUE_SIZE, 8 },
        { SDL_GL_ALPHA_SIZE, 8 },
        { SDL_GL_DEPTH_SIZE, 24 },
        { SDL_GL_STENCIL_SIZE, 8 },
    };

    for (const auto [attribute, value] : openGLWindowAttributes)
    {
        STARDUST_ASSERT_RELEASE(SDL_GL_SetAttribute(attribute, value) == 0);
    }

    sd::Window window(
        sd::Window::CreateInfo{
            .title = "Rectangle Render Test",
            .x = sd::Window::Position::Undefined,
            .y = sd::Window::Position::Undefined,
            .size = sd::UVector2{ 1280u, 720u },
            .flags = { sd::Window::CreateFlag::Shown, sd::Window::CreateFlag::Resizable, sd::Window::CreateFlag::OpenGL },
        }
    );

    STARDUST_ASSERT_RELEASE(window.IsValid());

    sd::opengl::Context openGLContext(window);
    STARDUST_ASSERT_RELEASE(openGLContext.IsValid());

    sd::Camera2D camera(8.0f, window.GetSize());

    sd::gfx::Renderer renderer(
        sd::gfx::Renderer::CreateInfo{
            .window = &window,
            .camera = &camera,

            .shadersDirectoryPath = "assets/shaders",

            .maxShapesPerBatch = 4'000u,
            .maxTextureSlotsPerBatch = 16u,
        }
    );

    STARDUST_ASSERT_RELEASE(renderer.IsValid());
    renderer.SetClearColour(sd::colours::DeepSkyBlue);

    sd::gfx::SetPresentationMode(sd::gfx::PresentationMode::Immediate);

    stbi_set_flip_vertically_on_load(1);

    sd::gfx::Texture crateTexture("assets/textures/crate.jpg");
    STARDUST_ASSERT_RELEASE(crateTexture.IsValid());

    sd::gfx::Texture statueTexture("assets/textures/statue.jpg");
    STARDUST_ASSERT_RELEASE(statueTexture.IsValid());

    std::atomic_bool isRunning = true;
    SDL_Event event{ };

    bool useBatch = false;
    sd::Log::Info("Hold spacebar to use batch rendering.");

    while (isRunning.load(std::memory_order::relaxed))
    {
        renderer.ClearColour();
        renderer.ClearDepthBuffer();

        if (useBatch)
        {
            renderer.StartQuadBatch();

            renderer.BatchScreenRectangle(
                sd::UVector2{ 250u, 200u },
                sd::comp::ScreenTransform{
                    .translation = sd::IVector2{ 100, 100 },
                    .reflection = sd::gfx::Reflection::Vertical,
                },
                sd::comp::Sprite{
                    .texture = &statueTexture,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::GreenYellow,
                }
            );

            renderer.BatchScreenRectangle(
                sd::UVector2{ 175u, 180u },
                sd::comp::ScreenTransform{
                    .translation = sd::IVector2{ 325, 115 },
                    .reflection = sd::gfx::Reflection::Vertical,
                    .rotation = SDL_GetTicks() * 0.05f,
                    .pivot = sd::Vector2{ -50, -40 },
                },
                sd::comp::Sprite{
                    .texture = &crateTexture,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::Maroon,
                }
            );

            renderer.BatchScreenRectangle(
                sd::UVector2{ 120u, 120u },
                sd::comp::ScreenTransform{
                    .translation = sd::IVector2{ 600, 100 },
                    .reflection = sd::gfx::Reflection::Both,
                    .rotation = SDL_GetTicks() * 0.05f,
                },
                sd::comp::Sprite{
                    .texture = &statueTexture,
                    .subTextureArea = sd::gfx::TextureCoordinatePair{
                        .lowerLeft = sd::Vector2{ 0.25f, 0.25f },
                        .upperRight = sd::Vector2{ 0.75f, 0.75f },
                    },
                    .colourMod = sd::colours::White,
                }
            );

            renderer.BatchScreenRectangle(
                sd::UVector2{ 110u, 100u },
                sd::comp::ScreenTransform{
                    .translation = sd::IVector2{ 750, 90 },
                    .reflection = sd::gfx::Reflection::None,
                    .rotation = SDL_GetTicks() * 0.05f,
                    .pivot = sd::IVector2{ -55, 50 },
                    .shear = sd::Vector2{ 30.0f, 0.0f },
                },
                sd::comp::Sprite{
                    .texture = nullptr,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::DarkSeaGreen,
                }
            );

            renderer.BatchScreenRectangle(
                sd::UVector2{ 80u, 75u },
                sd::comp::ScreenTransform{
                    .translation = sd::IVector2{ 1000, 95 },
                    .reflection = sd::gfx::Reflection::None,
                    .rotation = 0.0f,
                    .pivot = sd::None,
                    .shear = sd::Vector2{ 10.0f, 15.0f },
                },
                sd::comp::Sprite{
                    .texture = nullptr,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::BlueViolet,
                }
            );

            renderer.BatchRectangle(
                sd::comp::Transform{
                    .translation = sd::Vector2{ -5.5f, -2.0f },
                    .scale = sd::Vector2{ 2.5f, 2.5f },
                },
                sd::comp::Sprite{
                    .texture = &statueTexture,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::GreenYellow,
                }
            );

            renderer.BatchRectangle(
                sd::comp::Transform{
                    .translation = sd::Vector2{ -2.75f, -2.0f },
                    .scale = sd::Vector2{ 2.0f, 2.0f },
                    .reflection = sd::gfx::Reflection::Vertical,
                    .rotation = SDL_GetTicks() * 0.05f,
                    .pivot = sd::Vector2{ -0.5f, 0.5f },
                },
                sd::comp::Sprite{
                    .texture = &crateTexture,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::Maroon,
                }
            );

            renderer.BatchRectangle(
                sd::comp::Transform{
                    .translation = sd::Vector2{ 0.0f, -2.0f },
                    .scale = sd::Vector2{ 1.5f, 1.5f },
                    .reflection = sd::gfx::Reflection::Both,
                    .rotation = SDL_GetTicks() * 0.05f,
                },
                sd::comp::Sprite{
                    .texture = &statueTexture,
                    .subTextureArea = sd::gfx::TextureCoordinatePair{
                        .lowerLeft = sd::Vector2{ 0.25f, 0.25f },
                        .upperRight = sd::Vector2{ 0.75f, 0.75f },
                    },
                    .colourMod = sd::colours::White,
                }
            );

            renderer.BatchRectangle(
                sd::comp::Transform{
                    .translation = sd::Vector2{ 2.75f, -2.0f },
                    .scale = sd::Vector2{ 1.0f, 1.0f },
                    .reflection = sd::gfx::Reflection::None,
                    .rotation = SDL_GetTicks() * 0.05f,
                    .pivot = sd::Vector2{ -0.5f, 0.5f },
                    .shear = sd::Vector2{ 30.0f, 0.0f },
                },
                sd::comp::Sprite{
                    .texture = nullptr,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::DarkSeaGreen,
                }
            );

            renderer.BatchRectangle(
                sd::comp::Transform{
                    .translation = sd::Vector2{ 5.5f, -2.0f },
                    .scale = sd::Vector2{ 0.5f, 0.5f },
                    .reflection = sd::gfx::Reflection::None,
                    .rotation = 0.0f,
                    .pivot = sd::None,
                    .shear = sd::Vector2{ 10.0f, 15.0f },
                },
                sd::comp::Sprite{
                    .texture = nullptr,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::BlueViolet,
                }
            );

            renderer.FlushQuadBatch();
        }
        else
        {
            renderer.DrawScreenRectangle(
                sd::UVector2{ 250u, 200u },
                sd::comp::ScreenTransform{
                    .translation = sd::IVector2{ 100, 100 },
                    .reflection = sd::gfx::Reflection::Vertical,
                },
                sd::comp::Sprite{
                    .texture = &statueTexture,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::GreenYellow,
                }
            );

            renderer.DrawScreenRectangle(
                sd::UVector2{ 175u, 180u },
                sd::comp::ScreenTransform{
                    .translation = sd::IVector2{ 325, 115 },
                    .reflection = sd::gfx::Reflection::Vertical,
                    .rotation = SDL_GetTicks() * 0.05f,
                    .pivot = sd::Vector2{ -50, -40 },
                },
                sd::comp::Sprite{
                    .texture = &crateTexture,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::Maroon,
                }
            );

            renderer.DrawScreenRectangle(
                sd::UVector2{ 120u, 120u },
                sd::comp::ScreenTransform{
                    .translation = sd::IVector2{ 600, 100 },
                    .reflection = sd::gfx::Reflection::Both,
                    .rotation = SDL_GetTicks() * 0.05f,
                },
                sd::comp::Sprite{
                    .texture = &statueTexture,
                    .subTextureArea = sd::gfx::TextureCoordinatePair{
                        .lowerLeft = sd::Vector2{ 0.25f, 0.25f },
                        .upperRight = sd::Vector2{ 0.75f, 0.75f },
                    },
                    .colourMod = sd::colours::White,
                }
            );

            renderer.DrawScreenRectangle(
                sd::UVector2{ 110u, 100u },
                sd::comp::ScreenTransform{
                    .translation = sd::IVector2{ 750, 90 },
                    .reflection = sd::gfx::Reflection::None,
                    .rotation = SDL_GetTicks() * 0.05f,
                    .pivot = sd::IVector2{ -55, 50 },
                    .shear = sd::Vector2{ 30.0f, 0.0f },
                },
                sd::comp::Sprite{
                    .texture = nullptr,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::DarkSeaGreen,
                }
            );

            renderer.DrawScreenRectangle(
                sd::UVector2{ 80u, 75u },
                sd::comp::ScreenTransform{
                    .translation = sd::IVector2{ 1000, 95 },
                    .reflection = sd::gfx::Reflection::None,
                    .rotation = 0.0f,
                    .pivot = sd::None,
                    .shear = sd::Vector2{ 10.0f, 15.0f },
                },
                sd::comp::Sprite{
                    .texture = nullptr,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::BlueViolet,
                }
            );

            renderer.DrawRectangle(
                sd::comp::Transform{
                    .translation = sd::Vector2{ -5.5f, -2.0f },
                    .scale = sd::Vector2{ 2.5f, 2.5f },
                },
                sd::comp::Sprite{
                    .texture = &statueTexture,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::GreenYellow,
                }
            );

            renderer.DrawRectangle(
                sd::comp::Transform{
                    .translation = sd::Vector2{ -2.75f, -2.0f },
                    .scale = sd::Vector2{ 2.0f, 2.0f },
                    .reflection = sd::gfx::Reflection::Vertical,
                    .rotation = SDL_GetTicks() * 0.05f,
                    .pivot = sd::Vector2{ -0.5f, 0.5f },
                },
                sd::comp::Sprite{
                    .texture = &crateTexture,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::Maroon,
                }
            );

            renderer.DrawRectangle(
                sd::comp::Transform{
                    .translation = sd::Vector2{ 0.0f, -2.0f },
                    .scale = sd::Vector2{ 1.5f, 1.5f },
                    .reflection = sd::gfx::Reflection::Both,
                    .rotation = SDL_GetTicks() * 0.05f,
                },
                sd::comp::Sprite{
                    .texture = &statueTexture,
                    .subTextureArea = sd::gfx::TextureCoordinatePair{
                        .lowerLeft = sd::Vector2{ 0.25f, 0.25f },
                        .upperRight = sd::Vector2{ 0.75f, 0.75f },
                    },
                    .colourMod = sd::colours::White,
                }
            );

            renderer.DrawRectangle(
                sd::comp::Transform{
                    .translation = sd::Vector2{ 2.75f, -2.0f },
                    .scale = sd::Vector2{ 1.0f, 1.0f },
                    .reflection = sd::gfx::Reflection::None,
                    .rotation = SDL_GetTicks() * 0.05f,
                    .pivot = sd::Vector2{ -0.5f, 0.5f },
                    .shear = sd::Vector2{ 30.0f, 0.0f },
                },
                sd::comp::Sprite{
                    .texture = nullptr,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::DarkSeaGreen,
                }
            );

            renderer.DrawRectangle(
                sd::comp::Transform{
                    .translation = sd::Vector2{ 5.5f, -2.0f },
                    .scale = sd::Vector2{ 0.5f, 0.5f },
                    .reflection = sd::gfx::Reflection::None,
                    .rotation = 0.0f,
                    .pivot = sd::None,
                    .shear = sd::Vector2{ 10.0f, 15.0f },
                },
                sd::comp::Sprite{
                    .texture = nullptr,
                    .subTextureArea = sd::None,
                    .colourMod = sd::colours::BlueViolet,
                }
            );
        }

        window.Present();

        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    window.ProcessResize(sd::UVector2{
                        event.window.data1,
                        event.window.data2,
                    });

                    camera.ResetVirtualScreenSize(sd::UVector2{
                        event.window.data1,
                        event.window.data2,
                    });

                    renderer.ProcessResize();

                    break;
                }

                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    isRunning.store(false, std::memory_order::relaxed);
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    useBatch = true;
                }

                break;

            case SDL_KEYUP:
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    useBatch = false;
                }

                break;

            case SDL_QUIT:
                isRunning.store(false, std::memory_order::relaxed);

                break;
            }
        }
    }

    crateTexture.Destroy();
    statueTexture.Destroy();

    renderer.Destroy();
    openGLContext.Destroy();
    window.Destroy();

    sd::vfs::Quit();
    SDL_Quit();

    sd::Log::Shutdown();

    return EXIT_SUCCESS;
}
