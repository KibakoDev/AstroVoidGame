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
        float headingTop = 92.0f;
        float buttonHeight = 60.0f;
        float buttonWidth = 360.0f;
        float buttonSpacing = 22.0f;
        float firstButtonYFactor = 0.42f;
    };

    struct MenuLayout
    {
        float headingX = 0.0f;
        float headingY = 0.0f;
        float buttonX = 0.0f;
        float newGameY = 0.0f;
        float quitGameY = 0.0f;
    };

    KibakoEngine::UIStyle BuildMenuStyle(const KibakoEngine::Font* font)
    {
        KibakoEngine::UIStyle style{};
        style.font = font;
        style.headingColor = KibakoEngine::Color4{ 0.96f, 0.98f, 1.0f, 1.0f };
        style.primaryTextColor = KibakoEngine::Color4{ 0.86f, 0.90f, 1.0f, 1.0f };
        style.mutedTextColor = KibakoEngine::Color4{ 0.56f, 0.60f, 0.70f, 1.0f };
        style.headingScale = 1.4f;
        style.bodyScale = 1.0f;
        style.captionScale = 0.88f;

        return style;
    }

    MenuTheme BuildMenuTheme()
    {
        return MenuTheme{};
    }

    MenuLayout CalculateLayout(const AstroVoidLayer::ScreenSize& screen, const MenuTheme& theme)
    {
        MenuLayout layout{};

        const float centerX = screen.width * 0.5f;

        layout.headingX = centerX - (theme.buttonWidth * 0.5f);
        layout.headingY = theme.headingTop;

        layout.buttonX = centerX - (theme.buttonWidth * 0.5f);
        layout.newGameY = screen.height * theme.firstButtonYFactor;
        layout.quitGameY = layout.newGameY + theme.buttonHeight + theme.buttonSpacing;

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
    newGameBtn.SetText("NEW GAME");

    // Quit Game Button
    auto& quitBtn = root.EmplaceChild<KibakoEngine::UIButton>("QuitButton");
    quitBtn.SetText("QUIT GAME");

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
        m_newGameButton->SetPosition({ layout.buttonX, layout.newGameY });
    }

    if (m_quitButton)
    {
        m_quitButton->SetPosition({ layout.buttonX, layout.quitGameY });
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