// AstroVoidLayer.cpp
#include "AstroVoidLayer.h"

#include "KibakoEngine/Core/Application.h"
#include "KibakoEngine/Core/Log.h"

using namespace KibakoEngine;

namespace
{
    constexpr const char* kLogChannel = "AstroVoid";

    constexpr float kTitleYOffset = 84.0f;
    constexpr float kButtonHeight = 56.0f;
    constexpr float kButtonSpacing = 18.0f;
    constexpr float kButtonWidth = 320.0f;

    struct MenuLayout
    {
        KibakoEngine::Vec2 titlePosition{};
        KibakoEngine::Vec2 newGamePosition{};
        KibakoEngine::Vec2 quitGamePosition{};
        KibakoEngine::Vec2 buttonSize{};
    };

    MenuLayout CalculateLayout(float width, float height)
    {
        MenuLayout layout{};

        const float centerX = width * 0.5f;

        layout.buttonSize = { kButtonWidth, kButtonHeight };
        const float stackX = centerX - (layout.buttonSize.x * 0.5f);
        const float firstButtonY = height * 0.38f;

        layout.titlePosition = { centerX - (layout.buttonSize.x * 0.45f), kTitleYOffset };
        layout.newGamePosition = { stackX, firstButtonY };
        layout.quitGamePosition = { stackX, firstButtonY + layout.buttonSize.y + kButtonSpacing };

        return layout;
    }

    KibakoEngine::UIStyle BuildMenuStyle(const KibakoEngine::Font* font)
    {
        KibakoEngine::UIStyle style{};
        style.font = font;
        style.headingColor = KibakoEngine::Color4{ 1.0f, 1.0f, 1.0f, 1.0f };
        style.primaryTextColor = KibakoEngine::Color4{ 0.90f, 0.93f, 1.0f, 1.0f };
        style.mutedTextColor = KibakoEngine::Color4{ 0.65f, 0.68f, 0.76f, 1.0f };
        style.headingScale = 1.3f;
        style.bodyScale = 1.0f;
        style.captionScale = 0.85f;

        return style;
    }
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

    m_lastScreenSize = {};

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

    const UIStyle style = BuildMenuStyle(m_uiFont);

    auto screen = std::make_unique<UIScreen>();
    auto& root = screen->Root();

    const float width = static_cast<float>(m_app.Width());
    const float height = static_cast<float>(m_app.Height());

    m_lastScreenSize = { width, height };
    const MenuLayout layout = CalculateLayout(width, height);

    // --- Build Menu ---

    // Title
    auto& title = root.EmplaceChild<KibakoEngine::UILabel>("Title");
    style.ApplyHeading(title);
    title.SetText("ASTRO VOID");
    title.SetPosition(layout.titlePosition);

    // New Game Button
    auto& newGameBtn = root.EmplaceChild<KibakoEngine::UIButton>("NewGameButton");
    newGameBtn.SetText("NEW GAME");
    newGameBtn.SetPosition(layout.newGamePosition);

    // Quit Game Button
    auto& quitBtn = root.EmplaceChild<KibakoEngine::UIButton>("QuitButton");
    quitBtn.SetText("QUIT GAME");
    quitBtn.SetPosition(layout.quitGamePosition);

    m_titleLabel = &title;
    m_newGameButton = &newGameBtn;
    m_quitButton = &quitBtn;
    m_menuScreen = screen.get();

    m_uiSystem.PushScreen(std::move(screen));

}

// Update UI
void AstroVoidLayer::UpdateUI(float dt)
{
    const float width = static_cast<float>(m_app.Width());
    const float height = static_cast<float>(m_app.Height());

    if (width != m_lastScreenSize.width || height != m_lastScreenSize.height)
    {
        BuildUI();
    }

    m_uiSystem.SetScreenSize(width, height);

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