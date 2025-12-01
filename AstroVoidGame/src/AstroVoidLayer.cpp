// AstroVoidLayer.cpp - Minimal title/setup layer for Astro Void
#include "AstroVoidLayer.h"

#include "KibakoEngine/Core/Application.h"
#include "KibakoEngine/Core/Log.h"
#include "KibakoEngine/Core/Profiler.h"
#include "KibakoEngine/Core/Debug.h"

#include <SDL2/SDL_scancode.h>

using namespace KibakoEngine;

namespace
{
    constexpr const char* kLogChannel = "AstroVoid";
}

AstroVoidLayer::AstroVoidLayer(Application& app)
    : Layer("AstroVoid.Layer")
    , m_app(app)
{
}

void AstroVoidLayer::OnAttach()
{

    auto& assets = m_app.Assets();

    // Load UI font (same as sandbox for now, tu changeras pour ta vraie font Astro Void)
    m_uiFont = assets.LoadFontTTF("ui.astrovoid", "assets/fonts/dogica.ttf", 32);
    if (!m_uiFont) {
        KbkWarn(kLogChannel, "Failed to load UI font for Astro Void");
    }

    // Hook UISystem to engine input
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

    auto& input = m_app.InputSys();

    m_time += dt;

    // Plus tard ici : state machine (Title -> Playing -> GameOver)
    // Pour l’instant, on ne fait que mettre à jour l’UI.
    UpdateUI(dt);

    // Exemple futur : si ENTER => passer en "Playing"
    (void)input;
}

void AstroVoidLayer::OnRender(SpriteBatch2D& batch)
{

    // Quand tu auras de la scène (player, asteroids...), tu feras :
    // m_scene.Render(batch);

    // Pour l’instant : seulement l’UI titre
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

    // Title
    auto& title = root.EmplaceChild<UILabel>("Title");
    style.ApplyHeading(title);
    title.SetText("ASTRO VOID");
    title.SetPosition({ centerX - 220.0f, centerY - 60.0f });

    // Subtitle
    auto& subtitle = root.EmplaceChild<UILabel>("Subtitle");
    style.ApplyBody(subtitle);
    subtitle.SetText("Prototype powered by Kibako Engine");
    subtitle.SetPosition({ centerX - 220.0f, centerY - 24.0f });
    subtitle.SetColor(style.mutedTextColor);

    // Hint
    auto& hint = root.EmplaceChild<UILabel>("Hint");
    style.ApplyCaption(hint);
    hint.SetText("Press ENTER to start (not implemented yet)");
    hint.SetPosition({ centerX - 220.0f, centerY + 18.0f });
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