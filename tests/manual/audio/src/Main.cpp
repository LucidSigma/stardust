#include <atomic>
#include <cstdlib>

#include <stardust/Stardust.h>

auto main(const sd::i32 argc, char** const argv) -> sd::i32
{
    sd::Log::Initialise("audio test", "log.txt");

    STARDUST_ASSERT_RELEASE(SDL_Init(SDL_INIT_EVERYTHING) == 0);

    STARDUST_ASSERT_RELEASE(sd::fs::InitialiseApplicationBaseDirectory() == sd::Status::Success);
    STARDUST_ASSERT_RELEASE(sd::vfs::Initialise(argv[0]) == sd::Status::Success);
    STARDUST_ASSERT_RELEASE(sd::vfs::AddToSearchPath(sd::fs::GetApplicationBaseDirectory() + "../test_resources/sounds.zip") == sd::Status::Success);

    sd::audio::SoundSystem soundSystem;
    soundSystem.Initialise();
    STARDUST_ASSERT_RELEASE(soundSystem.IsValid());

    sd::Window window(
        sd::Window::CreateInfo{
            .title = "Audio Test",
            .x = sd::Window::Position::Undefined,
            .y = sd::Window::Position::Undefined,
            .size = sd::UVector2{ 512u, 512u },
            .flags = { sd::Window::CreateFlag::Shown },
        }
    );

    STARDUST_ASSERT_RELEASE(window.IsValid());

    {
        sd::audio::Sound sound("sounds/blip.wav");
        STARDUST_ASSERT_RELEASE(sound.IsValid());

        sd::audio::MixingBus mixingBus(soundSystem);
        mixingBus.Play();

        sd::Log::Info("1 - Play blip.");
        sd::Log::Info("2 - Play blip (left pan).");
        sd::Log::Info("3 - Play blip (right pan).");
        sd::Log::Info("4 - Play positional blip (left).");
        sd::Log::Info("5 - Play positional blip (middle).");
        sd::Log::Info("6 - Play positional blip (right).");
        sd::Log::Info("7 - Play blip (reverb).");
        sd::Log::Info("7 - Play blip (flanger).");
        sd::Log::Info("9 - Play blip (lo-fi).");
        sd::Log::Info("W - Move listener up one unit.");
        sd::Log::Info("S - Move listener down one unit.");
        sd::Log::Info("A - Move listener left one unit.");
        sd::Log::Info("D - Move listener right one unit.");
        sd::Log::Info("E - Rotate listener 15 degrees counterclockwise.");
        sd::Log::Info("Q - Rotate listener 15 degrees clockwise.");

        std::atomic_bool isRunning = true;
        SDL_Event event{ };

        constexpr sd::f32 Volume = 0.15f;

        while (isRunning.load(std::memory_order::relaxed))
        {
            while (SDL_PollEvent(&event) != 0)
            {
                switch (event.type)
                {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_1:
                        soundSystem.PlaySound(sound, sd::audio::SoundPlayData{
                            .volume = Volume,
                            .pan = 0.0f,
                        });

                        break;

                    case SDL_SCANCODE_2:
                        soundSystem.PlaySound(sound, sd::audio::SoundPlayData{
                            .volume = Volume,
                            .pan = -1.0f,
                        });

                        break;

                    case SDL_SCANCODE_3:
                        soundSystem.PlaySound(sound, sd::audio::SoundPlayData{
                            .volume = Volume,
                            .pan = 1.0f,
                        });

                        break;

                    case SDL_SCANCODE_4:
                        soundSystem.PlayPositionalSound(sound, sd::audio::PositionalSoundPlayData{
                            .volume = Volume,
                            .position = sd::Vector2{ -5.0f, 0.0f },
                            .attenuation = sd::audio::AttenuationInfo{
                                .model = sd::audio::AttenuationModel::Exponential,
                                .rolloffFactor = 1.0f,
                                .minDistance = 1.0f,
                                .maxDistance = 10.0f,
                            },
                        });

                        break;

                    case SDL_SCANCODE_5:
                        soundSystem.PlayPositionalSound(sound, sd::audio::PositionalSoundPlayData{
                            .volume = Volume,
                            .position = sd::Vector2{ 0.0f, 0.0f },
                            .attenuation = sd::audio::AttenuationInfo{
                                .model = sd::audio::AttenuationModel::Exponential,
                                .rolloffFactor = 1.0f,
                                .minDistance = 1.0f,
                                .maxDistance = 10.0f,
                            },
                        });

                        break;

                    case SDL_SCANCODE_6:
                        soundSystem.PlayPositionalSound(sound, sd::audio::PositionalSoundPlayData{
                            .volume = Volume,
                            .position = sd::Vector2{ 5.0f, 0.0f },
                            .attenuation = sd::audio::AttenuationInfo{
                                .model = sd::audio::AttenuationModel::Exponential,
                                .rolloffFactor = 1.0f,
                                .minDistance = 1.0f,
                                .maxDistance = 10.0f,
                            },
                        });

                        break;

                    case SDL_SCANCODE_7:
                        mixingBus.ClearAllFilters();
                        mixingBus.SetReverbFilter(0.5f, 0.5f, 1.0f);

                        mixingBus.PlaySound(sound, sd::audio::SoundPlayData{
                            .volume = Volume,
                        });

                        break;

                    case SDL_SCANCODE_8:
                        mixingBus.ClearAllFilters();
                        mixingBus.SetFlangerFilter(0.005f, 10.0f);

                        mixingBus.PlaySound(sound, sd::audio::SoundPlayData{
                            .volume = Volume,
                        });

                        break;

                    case SDL_SCANCODE_9:
                        mixingBus.ClearAllFilters();
                        mixingBus.SetLoFiFilter(8000.0f, 5.0f);

                        mixingBus.PlaySound(sound, sd::audio::SoundPlayData{
                            .volume = Volume,
                        });

                        break;

                    case SDL_SCANCODE_W:
                        soundSystem.GetListener().SetPosition(
                            soundSystem.GetListener().GetPosition().x,
                            soundSystem.GetListener().GetPosition().y + 1.0f
                        );

                        break;

                    case SDL_SCANCODE_S:
                        soundSystem.GetListener().SetPosition(
                            soundSystem.GetListener().GetPosition().x,
                            soundSystem.GetListener().GetPosition().y - 1.0f
                        );

                        break;

                    case SDL_SCANCODE_A:
                        soundSystem.GetListener().SetPosition(
                            soundSystem.GetListener().GetPosition().x - 1.0f,
                            soundSystem.GetListener().GetPosition().y
                        );

                        break;

                    case SDL_SCANCODE_D:
                        soundSystem.GetListener().SetPosition(
                            soundSystem.GetListener().GetPosition().x + 1.0f,
                            soundSystem.GetListener().GetPosition().y
                        );

                        break;

                    case SDL_SCANCODE_Q:
                        soundSystem.GetListener().SetRotation(soundSystem.GetListener().GetRotation() + 15.0f);

                        break;

                    case SDL_SCANCODE_E:
                        soundSystem.GetListener().SetRotation(soundSystem.GetListener().GetRotation() - 15.0f);

                        break;

                    case SDL_SCANCODE_ESCAPE:
                        isRunning.store(false, std::memory_order::relaxed);

                        break;
                    }

                    break;

                case SDL_QUIT:
                    isRunning = false;

                    break;
                }
            }

            soundSystem.Update();
        }

        mixingBus.ClearAllFilters();
        mixingBus.Stop();

        soundSystem.StopAllSounds();
    }

    window.Destroy();
    soundSystem.Destroy();

    sd::vfs::Quit();
    SDL_Quit();

    sd::Log::Shutdown();

    return EXIT_SUCCESS;
}
