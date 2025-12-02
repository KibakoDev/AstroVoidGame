// AstroVoidLayer.cpp
#include "AstroVoidLayer.h"

#include "KibakoEngine/Core/Application.h"
#include "KibakoEngine/Core/Log.h"

using namespace KibakoEngine;

namespace
{
    constexpr const char* kLogChannel = "AstroVoid";

    constexpr float kTitleOffsetX = 220.0f;
    constexpr float kTitleOffsetY = 60.0f;
    constexpr float kSubtitleSpacing = 36.0f;
    constexpr float kHintSpacing = 42.0f;
}

AstroVoidLayer::AstroVoidLayer(Application& app)
    : Layer("AstroVoid.Layer")
    , m_app(app)
{
}

void AstroVoidLayer::OnAttach()
{
    auto& assets = m_app.Assets();

    // Load UI font; swap this for your own typeface when branding the game.
    m_uiFont = assets.LoadFontTTF("ui.astrovoid", "assets/fonts/dogica.ttf", 32);
    if (!m_uiFont)
    {
        KbkWarn(kLogChannel, "Failed to load UI font for Astro Void");
    }

    // Hook UISystem to engine input so controls can react to keyboard/gamepad later.
    m_uiSystem.SetInput(&m_app.InputSys());

    BuildUI();

    KbkLog(kLogChannel, "AstroVoidLayer attached");
}

void AstroVoidLayer::OnDetach()
{
    m_scene.Clear();
    m_uiSystem.Clear();

    m_uiFont = nullptr;
    m_titleLabel = nullptr;
    m_newGameButton = nullptr;
    m_quitButton = nullptr;
    m_menuScreen = nullptr;

    m_time = 0.0f;
}


void AstroVoidLayer::OnUpdate(float dt)
{
    m_time += dt;

    // State logic
    switch (m_state)
    {
    case GameState::Title:
        UpdateTitle(dt);
        break;

    case GameState::Playing:
        UpdatePlaying(dt);
        break;
    }

    UpdateUI(dt);
}

void AstroVoidLayer::OnRender(SpriteBatch2D& batch)
{
    // Render your gameplay scene here once you start populating m_scene.
    // m_scene.Render(batch);

    m_uiSystem.Render(batch);
}

// --------------------------------------------------------
// UI
// --------------------------------------------------------

void AstroVoidLayer::BuildUI()
{
    // Menu elements
    m_titleLabel = nullptr;
    m_newGameButton = nullptr;
    m_quitButton = nullptr;
    m_menuScreen = nullptr;

    m_uiSystem.Clear();

    if (!m_uiFont)
        return;

    UIStyle style{};
    style.font = m_uiFont;
    style.headingColor = Color4::White();
    style.primaryTextColor = Color4::White();
    style.mutedTextColor = Color4{ 0.6f, 0.6f, 0.6f, 1.0f };
    style.headingScale = 1.2f;
    style.bodyScale = 0.9f;
    style.captionScale = 0.75f;

    auto screen = std::make_unique<UIScreen>();
    auto& root = screen->Root();

    const float width = static_cast<float>(m_app.Width());
    const float height = static_cast<float>(m_app.Height());

    // Text Values
    const float centerX = width * 0.5f;
    const float centerY = height * 0.5f;
    const float left = centerX - kTitleOffsetX;
    const float baselineY = centerY - kTitleOffsetY;

    // Button Values
    const float buttonWidth = 260.0f;
    const float buttonHeight = 48.0f;

    const float firstButtonY = centerY - 20.0f;   // New Game
    const float secondButtonY = firstButtonY + buttonHeight + 16.0f; // Quit

    // --- Build Menu ---

    // Title
    auto& title = root.EmplaceChild<KibakoEngine::UILabel>("Title");
    style.ApplyHeading(title);
    title.SetText("ASTRO VOID");
    title.SetPosition({ centerX - 200.0f, height * 0.20f });

    // New Game Button
    auto& newGameBtn = root.EmplaceChild<KibakoEngine::UIButton>("NewGameButton");
    newGameBtn.SetText("NEW GAME");
    newGameBtn.SetPosition({ centerX - buttonWidth * 0.5f,firstButtonY });

    // Quit Game Button
    auto& quitBtn = root.EmplaceChild<KibakoEngine::UIButton>("QuitButton");
    quitBtn.SetText("QUIT");
    quitBtn.SetPosition({centerX - buttonWidth * 0.5f,secondButtonY});

    m_titleLabel = &title;
    m_newGameButton = &newGameBtn;
    m_quitButton = &quitBtn;
    m_menuScreen = screen.get();

    m_uiSystem.PushScreen(std::move(screen));

}

// Update UI
void AstroVoidLayer::UpdateUI(float dt)
{
    (void)dt;

    m_uiSystem.SetScreenSize(
        static_cast<float>(m_app.Width()),
        static_cast<float>(m_app.Height()));

    m_uiSystem.Update(dt);
}

// Update Title State
void AstroVoidLayer::UpdateTitle(float dt)
{
    (void)dt;
}

//Update Playing State
void AstroVoidLayer::UpdatePlaying(float dt)
{
    (void)dt;
}