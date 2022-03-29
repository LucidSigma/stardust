#pragma once
#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include <stardust/Stardust.h>

class TestScene final
    : public sd::Scene
{
private:
    sd::gfx::SortingLayer m_defaultSortingLayer;

    sd::ObserverPointer<sd::gfx::Texture> m_crateTexture = nullptr;
    sd::ObserverPointer<sd::gfx::Texture> m_crumbleTexture = nullptr;

    sd::FontCache m_fontCache;
    sd::TextCache m_textCache;
    sd::TextWriter m_textWriter;

    sd::AssetManager<sd::gfx::Texture> m_textures;

    sd::gfx::Texture m_conveyorTextures;
    sd::gfx::TextureAtlas m_conveyorTextureAtlas;
    sd::gfx::Texture m_colourTextures;
    sd::gfx::TextureAtlas m_colourTextureAtlas;
    sd::gfx::Texture m_groundTiles;
    sd::gfx::TextureAtlas m_groundTilesTextureAtlas;

    sd::GameController* m_controller = nullptr;

    sd::AssetManager<sd::audio::Sound> m_sounds{ };
    
    sd::ParticleSystem m_particles;
    sd::f32 m_clickParticleDelay = 0.01f;

public:
    TestScene(sd::Application& application, const sd::String& name)
        : Scene(application, name)
    { }

    virtual ~TestScene() noexcept override = default;

    [[nodiscard]] virtual sd::Status OnLoad() override
    {
        sd::cursor::SetType(sd::cursor::Type::No);

        GetRenderer().SetClearColour(sd::Colour(0.3f, 0.05f, 0.5f, 1.0f));

        const auto textures = sd::vfs::GetAllFilepaths("assets/textures");

        for (const auto& texture : textures)
        {
            const sd::String textureName = sd::fs::GetStem(texture);
            m_textures.Add(textureName, texture);

            if (!m_textures[textureName].IsValid())
            {
                return sd::Status::Fail;
            }
        }

        m_crateTexture = &m_textures["crate"];
        m_crumbleTexture = &m_textures["crumble"];

        m_fontCache.Initialise(
            sd::FontCache::FontData{
                .filepath = "assets/fonts/TheanoModern.ttf",
            }
        );

        if (m_fontCache.Add(128u) != sd::Status::Success)
        {
            return sd::Status::Fail;
        }

        m_textWriter.Initialise(m_fontCache[128u], sd::IVector2Zero);
        m_textCache.Initialise(m_textWriter);

        m_conveyorTextures.Initialise("assets/textures/texture_atlases/conveyors.png");
        m_conveyorTextureAtlas.Initialise(m_conveyorTextures, "assets/textures/texture_atlases/conveyors.taj");

        m_colourTextures.Initialise("assets/textures/texture_atlases/colours.png");
        m_colourTextureAtlas.Initialise(m_colourTextures, "assets/textures/texture_atlases/colours.taj");

        m_groundTiles.Initialise("assets/textures/texture_atlases/tiles.png");
        m_groundTilesTextureAtlas.Initialise(m_groundTiles, "assets/textures/texture_atlases/tiles.taj");

        if (!m_conveyorTextures.IsValid() || !m_conveyorTextureAtlas.IsValid() || !m_colourTextures.IsValid() || !m_colourTextureAtlas.IsValid() || !m_groundTiles.IsValid() || !m_groundTilesTextureAtlas.IsValid())
        {
            return sd::Status::Fail;
        }

        m_sounds.Add("blip", "assets/sounds/blip.wav");

        if (!m_sounds["blip"].IsValid())
        {
            return sd::Status::Fail;
        }

        m_particles.SetWind(575.0f);

        GetInputManager().AddKeyToButton("quit", sd::KeyCode::Escape);
        GetInputManager().AddKeyToButton("outline", sd::KeyCode::Space);
        GetInputManager().AddKeyToButton("speed", sd::KeyCode::LeftShift);
        GetInputManager().AddMouseButtonToButton("coords", sd::MouseButton::Thumb1);

        GetInputManager().AddMouseButtonToButton("click", sd::MouseButton::Left);

        GetInputManager().AddKeyToButton("play_sound", sd::KeyCode::P);
        GetInputManager().AddGameControllerButtonToButton("play_sound", sd::GameControllerButton::Y);

        GetInputManager().AddKeyToButton("reset", sd::KeyCode::Tab);
        GetInputManager().AddGameControllerButtonToButton("reset", sd::GameControllerButton::RightShoulder);

        GetInputManager().AddMouseButtonToButton("particle", sd::MouseButton::Left);
        GetInputManager().AddGameControllerButtonToButton("particle", sd::GameControllerButton::A);

        GetInputManager().AddMouseAxisToAxis("scroll", sd::MouseAxis::Scroll);

        GetInputManager().AddKeyToPositiveAxis("x", sd::KeyCode::D);
        GetInputManager().AddKeyToNegativeAxis("x", sd::KeyCode::A);

        GetInputManager().AddKeyToPositiveAxis("y", sd::KeyCode::S);
        GetInputManager().AddKeyToNegativeAxis("y", sd::KeyCode::W);

        return sd::Status::Success;
    }

    virtual void OnUnload() noexcept override
    { }

    virtual void FixedUpdate(const sd::f32 fixedDeltaTime) override
    { }

    virtual void ProcessInput(const sd::InputController& inputController, const sd::InputManager& inputManager) override
    {
        if (m_application.HasWindowFocus())
        {
            if (inputManager.IsButtonDown("quit"))
            {
                Finish();
            }

            if (inputManager.IsButtonUp("coords"))
            {
                const sd::Vector2 mouseClick = GetCamera().ScreenSpaceToWorldSpace(GetMouseState().GetVirtualCoordintes(GetWindow(), GetCamera()));

                sd::Log::Trace("Screen: {} {}; World: {} {}", GetMouseState().GetVirtualCoordintes(GetWindow(), GetCamera()).x, GetMouseState().GetVirtualCoordintes(GetWindow(), GetCamera()).y, mouseClick.x, mouseClick.y);
            }

            GetCamera().SetZoom(GetCamera().GetZoom() + GetInputManager().GetAxis("scroll") * 0.1f);

            if (inputManager.IsButtonDown("reset", { m_controller }))
            {
                GetCamera().ResetTransform();
            }

            if (m_controller != nullptr)
            {
                GetCamera().SetRotation(GetCamera().GetRotation() - (m_controller->GetGyroscopeData().z * 0.04f));
            }

            if (inputManager.IsButtonDown("play_sound", { m_controller }))
            {
                GetSoundSystem().PlaySound(m_sounds["blip"]);
            }

            if (inputManager.IsButtonPressed("particle", { m_controller }) && m_clickParticleDelay < 0.0f)
            {
                m_clickParticleDelay = 0.01f;

                auto finalColour = sd::colours::Yellow;
                finalColour.alpha = 0u;

                m_particles.Emit(sd::ParticleData{
                    .initialPosition = GetMouseState().GetVirtualCoordintes(GetWindow(), GetCamera()),
                    .initialRotation = 0.0f,
                    .initialVelocityRange = { { -100.0f, -400.0f }, { 100.0f, -10.0f } },
                    .initialAccelerationRange = { 0.4f, 0.4f },
                    .initialAngularVelocityRange = { 0.0f, 180.0f },
                    .initialAngularAccelerationRange = { -0.1f, -0.1f },
                    .pivot = sd::None,
                    .isAffectedByGravity = false,
                    .isAffectedByWind = true,
                    .initialSizeRange = { { 20.0f, 20.0f }, { 40.0f, 40.0f } },
                    .sizeUpdateMultiplier = -0.2f,
                    .keepAsSquare = true,
                    .initialShearRange = sd::None,
                    .startColour = sd::colours::Red,
                    .endColour = finalColour,
                    .texture = nullptr,
                    .textureArea = sd::None,
                    .reflection = sd::gfx::Reflection::None,
                    .colourEasingFunction = sd::easings::EaseOutCubic,
                    .initialLifetimeRange = { 0.5f, 1.0f },
                    .callback = sd::None,
                    .callbackUserData = sd::None,
                });
            }
        }
    }

    virtual void Update(const sd::f32 deltaTime) override
    {
        GetCamera().SetPosition(
            GetCamera().GetPosition().x + GetInputManager().GetAxis("x", { m_controller }) * 10.0f * deltaTime,
            GetCamera().GetPosition().y + GetInputManager().GetAxis("y", { m_controller }) * 10.0f * deltaTime
        );

        m_clickParticleDelay -= deltaTime;
        m_particles.Update(deltaTime);
    }

    virtual void PostUpdate(const sd::f32 deltaTime) override { }

    virtual void Render(sd::gfx::Renderer& renderer) override
    {
        for (sd::i32 x = -3; x <= 3; ++x)
        {
            for (sd::i32 y = -3; y <= 3; ++y)
            {
                renderer.BatchRectangle(
                    sd::comp::Transform{
                        .translation = sd::Vector2{ x, y },
                        .rotation = 2.5f,
                        .shear = sd::Vector2{ 10.0f, 0.0f },
                    },
                    sd::comp::Sprite{
                        .texture = (x + y) % 2 == 0 ? m_crateTexture : m_crumbleTexture
                    }
                );
            }
        }

        renderer.BatchRectangle(
            sd::comp::Transform{
                .translation = sd::Vector2{ 1.0f, 1.0f },
                .scale = sd::Vector2{ 2.0f, 2.0f },
                .rotation = static_cast<sd::f32>(GetApplication().GetElapsedTime()) * 100.0f,
                .pivot = sd::Vector2{ -1.0f, -1.0f },
            },
            sd::comp::Sprite{
                .colourMod = sd::Colour(1.0f, 1.0f, 0.0f, 0.5f),
            }
        );

        renderer.BatchQuad(
            sd::geom::Quad{
                .lowerLeft = { -0.5f, -0.5f },
                .upperLeft = { -0.225f, 0.4f },
                .upperRight = { 0.5f, 0.5f },
                .lowerRight = { 0.4f, -0.5f },
            },
            sd::comp::Transform{
                .translation = sd::Vector2{ -6.0f, 0.0f },
                .scale = sd::Vector2{ 2.0f, 2.0f },
                .rotation = 15.0f,
                .shear = sd::Vector2{ 30.0f, 0.0f },
            },
            sd::comp::Sprite{
                .colourMod = sd::colours::Blue,
            }
        );

        for (const auto& word : { "one", "two", "three", "four", "five" })
        {
            for (const auto& [offset, size, sprite] : m_textCache.Get(word))
            {
                renderer.BatchScreenRectangle(
                    size,
                    sd::comp::ScreenTransform{
                        .translation = sd::IVector2{ 1400, 100 } + offset,
                    },
                    sprite
                );
            }
        }

        for (const auto& [transform, sprite] : m_particles.GenerateParticleComponents())
        {
            renderer.BatchRectangle(
                transform,
                sprite
            );
        }
    }

    virtual sd::EventStatus OnGameControllerAdded(const sd::events::GameControllerAdded& event) override
    {
        if (m_controller == nullptr)
        {
            m_controller = event.gameController;
            m_controller->SetPlayerIndex(0u);
            m_controller->SetLED(sd::colours::Purple);
        }

        return sd::EventStatus::NotHandled;
    }

    virtual sd::EventStatus OnGameControllerRemoved(const sd::events::GameControllerRemoved& event) override
    {
        m_controller = nullptr;

        return sd::EventStatus::NotHandled;
    }
};

#endif