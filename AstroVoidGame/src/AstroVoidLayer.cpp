// AstroVoidLayer.cpp
#include "AstroVoidLayer.h"

#include "KibakoEngine/Core/Application.h"
#include "KibakoEngine/Core/Log.h"
#include "KibakoEngine/Renderer/TextRenderer.h"
#include "KibakoEngine/UI/UIButton.h"
#include "KibakoEngine/UI/UIElement.h"
#include "KibakoEngine/UI/UIPanel.h"
#include "KibakoEngine/UI/UIScreen.h"
#include "KibakoEngine/UI/UILabel.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>

#include <algorithm>
#include <cmath>
#include <cstdio>

using namespace KibakoEngine;

namespace
{
    constexpr const char* kLogChannel = "AstroVoid";

    struct AstroMenuTheme
    {
        Color4 background{ 0.0f, 0.0f, 0.0f, 0.82f };
        Color4 panelColor{ 0.08f, 0.09f, 0.14f, 0.9f };
        Color4 buttonNormal{ 0.92f, 0.92f, 0.92f, 1.0f };
        Color4 buttonHover{ 1.0f, 1.0f, 1.0f, 1.0f };
        Color4 buttonPressed{ 0.8f, 0.8f, 0.82f, 1.0f };
        Color4 muted{ 0.75f, 0.75f, 0.8f, 1.0f };

        DirectX::XMFLOAT2 buttonSize{ 430.0f, 60.0f };
        DirectX::XMFLOAT2 buttonPadding{ 42.0f, 12.0f };

        float titleScale = 0.52f;
        float bodyScale = 0.44f;
        float buttonTextScale = 0.46f;
        float verticalSpacing = 26.0f;
        float titleSpacing = 72.0f;
        float infoPanelHeight = 72.0f;
    };

    const AstroMenuTheme kMenuTheme{};

    DirectX::XMFLOAT2 ScaleVec(const DirectX::XMFLOAT2& v, float scale)
    {
        return DirectX::XMFLOAT2{ v.x * scale, v.y * scale };
    }
}

AstroVoidLayer::AstroVoidLayer(Application& app)
    : Layer("AstroVoid.Layer")
    , m_app(app)
{
}

void AstroVoidLayer::OnAttach()
{
    m_scene.Clear();
    m_time = 0.0f;
    m_menuVisible = true;

    auto& assets = m_app.Assets();
    m_uiFont = assets.LoadFontTTF("ui.default", "assets/fonts/dogica.ttf", 32);
    if (!m_uiFont)
        KbkWarn(kLogChannel, "Failed to load font: assets/fonts/dogica.ttf");

    const float screenW = static_cast<float>(m_app.Width());
    const float screenH = static_cast<float>(m_app.Height());

    m_uiSystem.SetInput(&m_app.InputSys());
    m_uiScale = ComputeUiScale(screenW, screenH);
    m_lastUiWidth = screenW;
    m_lastUiHeight = screenH;

    BuildUI();

    KbkLog(kLogChannel, "AstroVoidLayer attached");
}

void AstroVoidLayer::OnDetach()
{
    m_scene.Clear();
    m_time = 0.0f;
    m_menuVisible = true;
    m_uiScale = 1.0f;
    m_lastUiWidth = 0.0f;
    m_lastUiHeight = 0.0f;

    m_uiSystem.Clear();
    m_uiFont = nullptr;

    m_titleLabel = nullptr;
    m_timeLabel = nullptr;
    m_stateLabel = nullptr;
    m_entitiesLabel = nullptr;
    m_resumeButton = nullptr;
    m_exitButton = nullptr;
    m_hudScreen = nullptr;
    m_menuScreen = nullptr;
    m_menuBackdrop = nullptr;
}

void AstroVoidLayer::OnUpdate(float dt)
{
    auto& input = m_app.InputSys();

    if (input.KeyPressed(SDL_SCANCODE_F3))
        m_menuVisible = !m_menuVisible;

    if (input.KeyPressed(SDL_SCANCODE_ESCAPE)) {
        SDL_Event quit{};
        quit.type = SDL_QUIT;
        SDL_PushEvent(&quit);
    }

    if (!m_menuVisible) {
        m_time += dt;
        m_scene.Update(dt);
    }

    UpdateUI(dt);
}

void AstroVoidLayer::OnRender(SpriteBatch2D& batch)
{
    m_scene.Render(batch);
    m_uiSystem.Render(batch);
}

float AstroVoidLayer::ComputeUiScale(float screenWidth, float screenHeight) const
{
    constexpr float kReferenceWidth = 1600.0f;
    constexpr float kReferenceHeight = 900.0f;

    const float widthScale = screenWidth / kReferenceWidth;
    const float heightScale = screenHeight / kReferenceHeight;
    const float rawScale = std::min(widthScale, heightScale);

    return std::clamp(rawScale, 0.7f, 1.4f);
}

void AstroVoidLayer::BuildUI()
{
    m_titleLabel = nullptr;
    m_timeLabel = nullptr;
    m_stateLabel = nullptr;
    m_entitiesLabel = nullptr;
    m_resumeButton = nullptr;
    m_exitButton = nullptr;
    m_hudScreen = nullptr;
    m_menuScreen = nullptr;
    m_menuBackdrop = nullptr;
    m_uiSystem.Clear();

    if (!m_uiFont)
        return;

    const float screenW = static_cast<float>(m_app.Width());
    const float screenH = static_cast<float>(m_app.Height());

    const AstroMenuTheme scaledTheme{
        kMenuTheme.background,
        kMenuTheme.panelColor,
        kMenuTheme.buttonNormal,
        kMenuTheme.buttonHover,
        kMenuTheme.buttonPressed,
        kMenuTheme.muted,
        ScaleVec(kMenuTheme.buttonSize, m_uiScale),
        ScaleVec(kMenuTheme.buttonPadding, m_uiScale),
        kMenuTheme.titleScale * m_uiScale,
        kMenuTheme.bodyScale * m_uiScale,
        kMenuTheme.buttonTextScale * m_uiScale,
        kMenuTheme.verticalSpacing * m_uiScale,
        kMenuTheme.titleSpacing * m_uiScale,
        kMenuTheme.infoPanelHeight * m_uiScale,
    };

    const auto scaledCaption = scaledTheme.bodyScale * 0.85f;

    auto& style = m_uiSystem.Style();
    style.font = m_uiFont;
    style.headingColor = Color4::White();
    style.primaryTextColor = Color4::Black();
    style.mutedTextColor = kMenuTheme.muted;
    style.panelColor = kMenuTheme.panelColor;
    style.buttonNormal = kMenuTheme.buttonNormal;
    style.buttonHover = kMenuTheme.buttonHover;
    style.buttonPressed = kMenuTheme.buttonPressed;
    style.buttonSize = scaledTheme.buttonSize;
    style.buttonPadding = scaledTheme.buttonPadding;
    style.headingScale = scaledTheme.titleScale;
    style.bodyScale = scaledTheme.bodyScale;
    style.captionScale = scaledCaption;
    style.buttonTextScale = scaledTheme.buttonTextScale;

    const float headingHeight = TextRenderer::MeasureText(*style.font, "S", style.headingScale).lineHeight;
    const float bodyHeight = TextRenderer::MeasureText(*style.font, "S", style.bodyScale).lineHeight;
    const float lineSpacing = 6.0f * m_uiScale;

    UIScreen& hud = m_uiSystem.CreateScreen("HUD");
    auto& hudRoot = hud.Root();

    auto& hudGroup = hudRoot.EmplaceChild<UIElement>("HUD.Group");
    hudGroup.SetPosition({ 16.0f * m_uiScale, 16.0f * m_uiScale });

    float hudY = 0.0f;
    auto makeHudLabel = [&](const char* name, float height, auto styler, const char* text) -> UILabel& {
        auto& lbl = hudGroup.EmplaceChild<UILabel>(name);
        styler(lbl);
        lbl.SetPosition({ 0.0f, hudY });
        lbl.SetText(text);
        hudY += height + lineSpacing;
        return lbl;
    };

    m_titleLabel = &makeHudLabel("HUD.Title", headingHeight + (5.0f * m_uiScale), [&](UILabel& lbl) { style.ApplyHeading(lbl); }, "ASTRO VOID");
    m_timeLabel = &makeHudLabel("HUD.Time", bodyHeight, [&](UILabel& lbl) { style.ApplyBody(lbl); }, "TIME  0.00 s");
    m_stateLabel = &makeHudLabel("HUD.State", bodyHeight, [&](UILabel& lbl) { style.ApplyBody(lbl); }, "STATUS  STANDBY");
    m_entitiesLabel = &makeHudLabel("HUD.Entities", bodyHeight, [&](UILabel& lbl) { style.ApplyBody(lbl); }, "ENTITIES  0");

    auto& hintLabel = hudGroup.EmplaceChild<UILabel>("HUD.Hint");
    style.ApplyCaption(hintLabel);
    hintLabel.SetPosition({ 0.0f, hudY });
    hintLabel.SetText("F3: Menu  ·  ESC: Quit");

    m_hudScreen = &hud;

    UIScreen& menu = m_uiSystem.CreateScreen("Menu");
    auto& menuRoot = menu.Root();

    auto& dim = menuRoot.EmplaceChild<UIPanel>("Menu.Backdrop");
    dim.SetColor(kMenuTheme.background);
    dim.SetAnchor(UIAnchor::TopLeft);
    dim.SetSize({ screenW, screenH });
    m_menuBackdrop = &dim;

    auto& stack = menuRoot.EmplaceChild<UIElement>("Menu.Stack");
    stack.SetAnchor(UIAnchor::Center);

    float yOffset = -scaledTheme.titleSpacing;

    auto& title = stack.EmplaceChild<UILabel>("Menu.Title");
    style.ApplyHeading(title);
    title.SetAnchor(UIAnchor::Center);
    title.SetScale(scaledTheme.titleScale + (0.2f * m_uiScale));
    title.SetText("ASTRO VOID");
    title.SetPosition({ 0.0f, yOffset });
    yOffset += scaledTheme.titleSpacing;

    auto makeMenuButton = [&](const char* name, const char* text, auto onClick) -> UIButton& {
        auto& btn = stack.EmplaceChild<UIButton>(name);
        style.ApplyButton(btn);
        btn.SetAnchor(UIAnchor::Center);
        btn.SetPosition({ 0.0f, yOffset });
        btn.SetText(text);
        btn.SetOnClick(onClick);
        yOffset += scaledTheme.buttonSize.y + scaledTheme.verticalSpacing;
        return btn;
    };

    m_resumeButton = &makeMenuButton("Menu.Resume", "NEW SESSION", [this]() {
        m_time = 0.0f;
        m_menuVisible = false;
    });

    m_exitButton = &makeMenuButton("Menu.Exit", "EXIT", []() {
        SDL_Event quit{};
        quit.type = SDL_QUIT;
        SDL_PushEvent(&quit);
    });

    menu.SetVisible(true);
    m_menuScreen = &menu;
}

void AstroVoidLayer::UpdateUI(float dt)
{
    (void)dt;

    const float screenW = static_cast<float>(m_app.Width());
    const float screenH = static_cast<float>(m_app.Height());

    const float targetScale = ComputeUiScale(screenW, screenH);
    const bool sizeChanged = (screenW != m_lastUiWidth) || (screenH != m_lastUiHeight);
    if (sizeChanged || std::fabs(targetScale - m_uiScale) > 0.01f) {
        m_uiScale = targetScale;
        m_lastUiWidth = screenW;
        m_lastUiHeight = screenH;
        BuildUI();
    }

    m_uiSystem.SetScreenSize(screenW, screenH);

    if (m_menuBackdrop)
        m_menuBackdrop->SetSize({ screenW, screenH });

    if (m_timeLabel) {
        char buf[64]{};
        std::snprintf(buf, sizeof(buf), "TIME  %.2f s", m_time);
        m_timeLabel->SetText(buf);
    }

    if (m_entitiesLabel) {
        char buf[64]{};
        const auto count = m_scene.Entities().size();
        std::snprintf(buf, sizeof(buf), "ENTITIES  %zu", static_cast<std::size_t>(count));
        m_entitiesLabel->SetText(buf);
    }

    if (m_stateLabel) {
        m_stateLabel->SetText(m_menuVisible ? "STATUS  PAUSED" : "STATUS  ACTIVE");
    }

    if (m_titleLabel) {
        m_titleLabel->SetText(m_menuVisible ? "ASTRO VOID  ·  PAUSED" : "ASTRO VOID");
    }

    if (m_hudScreen)
        m_hudScreen->SetVisible(true);

    if (m_menuScreen)
        m_menuScreen->SetVisible(m_menuVisible);

    if (m_resumeButton)
        m_resumeButton->SetText(m_time > 0.01f ? "RESUME" : "NEW SESSION");

    m_uiSystem.Update(dt);
}