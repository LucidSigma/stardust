#include <atomic>
#include <cstdlib>

#include <SDL2/SDL.h>
#include <stardust/Stardust.h>

auto main(const sd::i32 argc, char** const argv) -> sd::i32
{
    sd::Log::Initialise("text render test", "log.txt");

    STARDUST_ASSERT_RELEASE(SDL_Init(SDL_INIT_EVERYTHING) == 0);
    STARDUST_ASSERT_RELEASE(SDL_SetHintWithPriority(SDL_HINT_OPENGL_ES_DRIVER, "1", SDL_HINT_OVERRIDE) == SDL_TRUE);
    STARDUST_ASSERT_RELEASE(SDL_SetHintWithPriority(SDL_HINT_VIDEO_WIN_D3DCOMPILER, "none", SDL_HINT_OVERRIDE) == SDL_TRUE);

    STARDUST_ASSERT_RELEASE(sd::fs::InitialiseApplicationBaseDirectory() == sd::Status::Success);
    STARDUST_ASSERT_RELEASE(sd::vfs::Initialise(argv[0]) == sd::Status::Success);
    STARDUST_ASSERT_RELEASE(sd::vfs::AddToSearchPath(sd::fs::GetApplicationBaseDirectory() + "../test_resources/fonts.zip") == sd::Status::Success);
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
            .title = "Text Render Test",
            .x = sd::Window::Position::Undefined,
            .y = sd::Window::Position::Undefined,
            .size = sd::UVector2{ 1280u, 720u },
            .flags = { sd::Window::CreateFlag::Shown, sd::Window::CreateFlag::Resizable, sd::Window::CreateFlag::OpenGL }
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

    sd::FontCache fontCache(
        sd::FontCache::FontData{
            .filepath = "fonts/ZenKurenaido-Regular.ttf",
        }
    );
    STARDUST_ASSERT_RELEASE(fontCache.Add(28u) == sd::Status::Success);

    sd::TextWriter textWriter(fontCache.Get(28u), sd::IVector2Zero);
    STARDUST_ASSERT_RELEASE(textWriter.IsValid());

    sd::TextCache textCache(textWriter);

    sd::List<sd::ObserverPointer<const sd::List<sd::GlyphRenderInfo>>> textLines{ };

    textLines.push_back(&textCache.Get("This is some text.", sd::Markup{ }, false));
    textWriter.FeedNewLine();

    textLines.push_back(&textCache.Get("Almost before we knew it, we had left the ground.", sd::Markup{ }, false));
    textWriter.FeedNewLine();

    textLines.push_back(&textCache.Get("The quick brown fox jumps over the lazy dog.", sd::Markup{ }, false));
    textWriter.FeedNewLine();

    textLines.push_back(&textCache.Get(u8"Αα, Ββ, Γγ, Δδ, Εε, Ζζ, Ηη, Θθ, Ιι, Κκ, Λλ, Μμ, Νν, Ξξ, Οο, Ππ, Ρρ, Σσς, Ττ, Υυ, Φφ, Χχ, Ψψ, Ωω", sd::Markup{ }, false));
    textWriter.FeedNewLine();

    const auto& wrappedText = textCache.Get("This text will be wrapped after a certain amount of pixels and aligned rightward.", sd::Markup{
        .colour = sd::colours::Magenta,
        .textAlignment = sd::TextAlignment::Right,
        .wrapLength = 256u,
    }, false);
    textWriter.FeedNewLine();

    const auto& outlinedText = textCache.Get("This text will have an outline.", sd::Markup{
        .colour = sd::colours::Purple,
        .outline = sd::TextOutline{
            .thickness = 4.0f,
            .colour = sd::colours::Gold,
        },
    }, false);
    textWriter.FeedNewLine();

    const auto& shadowText = textCache.Get("This text will have a drop shadow.", sd::Markup{
        .colour = sd::colours::Silver,
        .dropShadow = sd::TextDropShadow{
            .thickness = sd::Optional<sd::f32>(2.0f),
            .colour = sd::colours::DarkGrey,
            .offset = sd::IVector2{ 2, 2 },
        },
    }, false);
    textWriter.FeedNewLine();

    sd::FontCache arabicFontCache(
        sd::FontCache::FontData{
            .filepath = "fonts/Cairo-Regular.ttf",
        }
    );
    STARDUST_ASSERT_RELEASE(arabicFontCache.Add(28u) == sd::Status::Success);

    sd::TextWriter arabicTextWriter(arabicFontCache.Get(28u), textWriter.GetCaretLocation());
    STARDUST_ASSERT_RELEASE(arabicTextWriter.IsValid());

    sd::TextCache arabicTextCache(arabicTextWriter);

    const sd::u32 arabicTextLength = arabicTextWriter.GetTextWidth(
        u8"صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ",
        sd::l10n::TextLocalisationInfo{
            .direction = sd::l10n::Direction::RightToLeft,
            .script = sd::l10n::Script::Arabic,
            .language = "ar",
        }
    );

    const auto& arabicText = arabicTextCache.Get(u8"صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ", sd::Markup{
        .textAlignment = sd::TextAlignment::Right,
        .localisation = sd::l10n::TextLocalisationInfo{
            .direction = sd::l10n::Direction::RightToLeft,
            .script = sd::l10n::Script::Arabic,
            .language = "ar",
        },
    }, false);
    arabicTextWriter.FeedNewLine();

    std::atomic_bool isRunning = true;
    SDL_Event event{ };

    while (isRunning.load(std::memory_order::relaxed))
    {
        renderer.ClearColour();
        renderer.ClearDepthBuffer();

        renderer.StartQuadBatch();

        for (const auto& textLine : textLines)
        {
            for (const auto& glyph : *textLine)
            {
                renderer.BatchScreenRectangle(
                    glyph.size,
                    sd::comp::ScreenTransform{
                        .translation = glyph.offset + sd::IVector2{ 100, 100 },
                    },
                    glyph.sprite
                );
            }
        }

        for (const auto& glyph : wrappedText)
        {
            renderer.BatchScreenRectangle(
                glyph.size,
                sd::comp::ScreenTransform{
                    .translation = glyph.offset + sd::IVector2{ 356, 100 },
                },
                glyph.sprite
            );
        }

        for (const auto& glyph : outlinedText)
        {
            renderer.BatchScreenRectangle(
                glyph.size,
                sd::comp::ScreenTransform{
                    .translation = glyph.offset + sd::IVector2{ 100, 100 },
                },
                glyph.sprite
            );
        }

        for (const auto& glyph : shadowText)
        {
            renderer.BatchScreenRectangle(
                glyph.size,
                sd::comp::ScreenTransform{
                    .translation = glyph.offset + sd::IVector2{ 100, 100 },
                },
                glyph.sprite
            );
        }

        for (const auto& glyph : arabicText)
        {
            renderer.BatchScreenRectangle(
                glyph.size,
                sd::comp::ScreenTransform{
                    .translation = glyph.offset + sd::IVector2{ 100 + arabicTextLength, 100 },
                },
                glyph.sprite
            );
        }

        renderer.FlushQuadBatch();

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

                break;

            case SDL_QUIT:
                isRunning.store(false, std::memory_order::relaxed);

                break;
            }
        }
    }

    renderer.Destroy();
    openGLContext.Destroy();
    window.Destroy();

    sd::vfs::Quit();
    SDL_Quit();

    sd::Log::Shutdown();

    return EXIT_SUCCESS;
}
