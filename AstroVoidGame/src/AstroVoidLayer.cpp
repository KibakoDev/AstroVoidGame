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
    m_subtitleLabel = nullptr;
    m_hintLabel = nullptr;
    m_hudScreen = nullptr;

    m_time = 0.0f;
}

void AstroVoidLayer::OnUpdate(float dt)
{
    m_time += dt;

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
    m_titleLabel = nullptr;
    m_subtitleLabel = nullptr;
    m_hintLabel = nullptr;
    m_hudScreen = nullptr;

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

    const float centerX = width * 0.5f;
    const float centerY = height * 0.5f;
    const float left = centerX - kTitleOffsetX;
    const float baselineY = centerY - kTitleOffsetY;

    // Title
    auto& title = root.EmplaceChild<UILabel>("Title");
    style.ApplyHeading(title);
    title.SetText("ASTRO VOID");
    title.SetPosition({ left, baselineY });

    // Subtitle
    auto& subtitle = root.EmplaceChild<UILabel>("Subtitle");
    style.ApplyBody(subtitle);
    subtitle.SetText("Prototype powered by Kibako Engine");
    subtitle.SetPosition({ left, baselineY + kSubtitleSpacing });
    subtitle.SetColor(style.mutedTextColor);

    // Hint
    auto& hint = root.EmplaceChild<UILabel>("Hint");
    style.ApplyCaption(hint);
    hint.SetText("Press ENTER to start (wire input in OnUpdate)");
    hint.SetPosition({ left, baselineY + kSubtitleSpacing + kHintSpacing });
    hint.SetColor(style.mutedTextColor);

    m_titleLabel = &title;
    m_subtitleLabel = &subtitle;
    m_hintLabel = &hint;
    m_hudScreen = screen.get();

    m_uiSystem.PushScreen(std::move(screen));

}

void AstroVoidLayer::UpdateUI(float dt)
{
    (void)dt;

    m_uiSystem.SetScreenSize(
        static_cast<float>(m_app.Width()),
        static_cast<float>(m_app.Height()));

    m_uiSystem.Update(dt);
}