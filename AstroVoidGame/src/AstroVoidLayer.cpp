// AstroVoidLayer.cpp
#include "AstroVoidLayer.h"

#include "KibakoEngine/Core/Application.h"
#include "KibakoEngine/Core/Log.h"

using namespace KibakoEngine;

namespace
{
    constexpr const char* kLogChannel = "AstroVoid";

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
    m_menuTheme = MenuTheme{};
    m_menuContent = MenuContent{};
    ResetMenuReferences();

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
    ResetMenuReferences();

    m_uiSystem.Clear();

    if (!m_uiFont)
        return;

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

    m_menuScreen = screen.get();

    ApplyMenuLayout(CalculateLayout(screenSize));

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
            ApplyMenuLayout(CalculateLayout(newSize));
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

MenuLayout AstroVoidLayer::CalculateLayout(const ScreenSize& screen) const
{
    MenuLayout layout{};

    const float centerX = screen.width * 0.5f;

    layout.headingX = centerX - (m_menuTheme.primaryButtonWidth * 0.5f);
    layout.headingY = m_menuTheme.headingTop;

    layout.primaryButtonWidth = m_menuTheme.primaryButtonWidth;
    layout.primaryButtonHeight = m_menuTheme.primaryButtonHeight;
    layout.secondaryButtonWidth = m_menuTheme.secondaryButtonWidth;
    layout.secondaryButtonHeight = m_menuTheme.secondaryButtonHeight;

    layout.primaryButtonX = centerX - (m_menuTheme.primaryButtonWidth * 0.5f);
    layout.primaryButtonY = screen.height * m_menuTheme.primaryButtonYFactor;
    layout.secondaryButtonX = centerX - (m_menuTheme.secondaryButtonWidth * 0.5f);
    layout.secondaryButtonY = layout.primaryButtonY + m_menuTheme.primaryButtonHeight + m_menuTheme.buttonSpacing;

    return layout;
}

void AstroVoidLayer::BuildMenuElements(UIElement& root)
{
    ResetMenuReferences();

    auto& title = root.EmplaceChild<KibakoEngine::UILabel>("Title");
    m_menuStyle.ApplyHeading(title);
    title.SetText(m_menuContent.title.c_str());

    auto& newGameBtn = root.EmplaceChild<KibakoEngine::UIButton>("NewGameButton");
    newGameBtn.SetText(m_menuContent.startLabel.c_str());

    auto& quitBtn = root.EmplaceChild<KibakoEngine::UIButton>("QuitButton");
    quitBtn.SetText(m_menuContent.controlsLabel.c_str());

    m_titleLabel = &title;
    m_newGameButton = &newGameBtn;
    m_quitButton = &quitBtn;
}

void AstroVoidLayer::ResetMenuReferences()
{
    m_titleLabel = nullptr;
    m_newGameButton = nullptr;
    m_quitButton = nullptr;
    m_menuScreen = nullptr;
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