// AstroVoidLayer.cpp
#include "AstroVoidLayer.h"

#include "KibakoEngine/Core/Application.h"
#include "KibakoEngine/Core/Log.h"

using namespace KibakoEngine;

namespace
{
    constexpr const char* kLogChannel = "AstroVoid";

    struct MenuTheme
    {
        float headingTop = 48.0f;
        float primaryButtonHeight = 86.0f;
        float primaryButtonWidth = 420.0f;
        float secondaryButtonHeight = 56.0f;
        float secondaryButtonWidth = 260.0f;
        float primaryButtonYFactor = 0.36f;
        float buttonSpacing = 96.0f;
    };

    KibakoEngine::UIStyle BuildMenuStyle(const KibakoEngine::Font* font)
    {
        KibakoEngine::UIStyle style{};
        style.font = font;
        style.headingColor = KibakoEngine::Color4{ 1.0f, 1.0f, 1.0f, 1.0f };
        style.primaryTextColor = KibakoEngine::Color4{ 1.0f, 1.0f, 1.0f, 1.0f };
        style.mutedTextColor = KibakoEngine::Color4{ 0.78f, 0.78f, 0.78f, 1.0f };
        style.headingScale = 2.4f;
        style.bodyScale = 1.26f;
        style.captionScale = 1.0f;

        return style;
    }

    MenuTheme BuildMenuTheme()
    {
        return MenuTheme{};
    }

    MenuLayout CalculateLayout(const ScreenSize& screen, const MenuTheme& theme)
    {
        MenuLayout layout{};

        const float centerX = screen.width * 0.5f;

        layout.headingX = centerX - (theme.primaryButtonWidth * 0.5f);
        layout.headingY = theme.headingTop;

        layout.primaryButtonWidth = theme.primaryButtonWidth;
        layout.primaryButtonHeight = theme.primaryButtonHeight;
        layout.secondaryButtonWidth = theme.secondaryButtonWidth;
        layout.secondaryButtonHeight = theme.secondaryButtonHeight;

        layout.primaryButtonX = centerX - (theme.primaryButtonWidth * 0.5f);
        layout.primaryButtonY = screen.height * theme.primaryButtonYFactor;
        layout.secondaryButtonX = centerX - (theme.secondaryButtonWidth * 0.5f);
        layout.secondaryButtonY = layout.primaryButtonY + theme.primaryButtonHeight + theme.buttonSpacing;

        return layout;
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

    m_menuStyle = BuildMenuStyle(m_uiFont);

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
    m_menuStyle = {};
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

    const MenuTheme theme = BuildMenuTheme();

    // Hold onto the style to re-apply if we rebuild individual elements later on.
    if (!m_menuStyle.font)
    {
        m_menuStyle = BuildMenuStyle(m_uiFont);
    }

    auto screen = std::make_unique<UIScreen>();
    auto& root = screen->Root();

    const ScreenSize screenSize{
        static_cast<float>(m_app.Width()),
        static_cast<float>(m_app.Height())
    };

    m_lastScreenSize = screenSize;
    const MenuLayout layout = CalculateLayout(screenSize, theme);

    // --- Build Menu ---

    // Title
    auto& title = root.EmplaceChild<KibakoEngine::UILabel>("Title");
    m_menuStyle.ApplyHeading(title);
    title.SetText("ASTRO VOID");

    // New Game Button
    auto& newGameBtn = root.EmplaceChild<KibakoEngine::UIButton>("NewGameButton");
    newGameBtn.SetText("NOUVELLE PARTIE");

    // Quit Game Button
    auto& quitBtn = root.EmplaceChild<KibakoEngine::UIButton>("QuitButton");
    quitBtn.SetText("TOUCHES");

    m_titleLabel = &title;
    m_newGameButton = &newGameBtn;
    m_quitButton = &quitBtn;
    m_menuScreen = screen.get();

    ApplyMenuLayout(layout);

    m_uiSystem.PushScreen(std::move(screen));

}

// Update UI
void AstroVoidLayer::UpdateUI(float dt)
{
    const float width = static_cast<float>(m_app.Width());
    const float height = static_cast<float>(m_app.Height());

    if (width != m_lastScreenSize.width || height != m_lastScreenSize.height)
    {
        const ScreenSize newSize{ width, height };
        m_lastScreenSize = newSize;

        if (m_menuScreen)
        {
            ApplyMenuLayout(CalculateLayout(newSize, BuildMenuTheme()));
        }
        else
        {
            BuildUI();
        }
    }

    m_uiSystem.SetScreenSize(width, height);

    m_uiSystem.Update(dt);
}

void AstroVoidLayer::ApplyMenuLayout(const MenuLayout& layout)
{
    if (m_titleLabel)
    {
        m_titleLabel->SetPosition({ layout.headingX, layout.headingY });
    }

    if (m_newGameButton)
    {
        m_newGameButton->SetPosition({ layout.primaryButtonX, layout.primaryButtonY });
        m_newGameButton->SetSize({ layout.primaryButtonWidth, layout.primaryButtonHeight });
    }

    if (m_quitButton)
    {
        m_quitButton->SetPosition({ layout.secondaryButtonX, layout.secondaryButtonY });
        m_quitButton->SetSize({ layout.secondaryButtonWidth, layout.secondaryButtonHeight });
    }
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