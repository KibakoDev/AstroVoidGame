// AstroVoidLayer.cpp
#include "AstroVoidLayer.h"

#include "KibakoEngine/Core/Application.h"
#include "KibakoEngine/Core/Log.h"

#include <SDL2/SDL_events.h>

#include <algorithm>
#include <cmath>

using namespace KibakoEngine;

namespace
{
    constexpr const char* kLogChannel = "AstroVoid";

    struct AstroVoidMenuTheme
    {
        Color4 background{ 0.0f, 0.0f, 0.0f, 0.78f };
        Color4 panelColor{ 0.55f, 0.05f, 0.05f, 0.88f };
        Color4 buttonNormal{ 1.0f, 1.0f, 1.0f, 1.0f };
        Color4 buttonHover{ 0.88f, 0.88f, 0.88f, 1.0f };
        Color4 buttonPressed{ 0.78f, 0.78f, 0.78f, 1.0f };
        Color4 muted{ 0.75f, 0.75f, 0.75f, 1.0f };

        DirectX::XMFLOAT2 buttonSize{ 440.0f, 64.0f };
        DirectX::XMFLOAT2 buttonPadding{ 48.0f, 14.0f };

        float titleScale = 0.50f;
        float bodyScale = 0.42f;
        float buttonTextScale = 0.46f;
        float verticalSpacing = 28.0f;
        float titleSpacing = 74.0f;
    };

    const AstroVoidMenuTheme kMenuTheme{};

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

    auto& assets = m_app.Assets();
    m_uiFont = assets.LoadFontTTF("ui.default", "assets/fonts/dogica.ttf", 32);
    if (!m_uiFont)
        KbkWarn(kLogChannel, "Failed to load font: assets/fonts/dogica.ttf");

    m_uiSystem.SetInput(&m_app.InputSys());

    const float screenW = static_cast<float>(m_app.Width());
    const float screenH = static_cast<float>(m_app.Height());
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
    m_state = GameState::Title;
    m_uiScale = 1.0f;
    m_lastUiWidth = 0.0f;
    m_lastUiHeight = 0.0f;
    m_uiFont = nullptr;

    m_uiSystem.Clear();
    m_menuBackdrop = nullptr;
    m_menuScreen = nullptr;
}

void AstroVoidLayer::OnUpdate(float dt)
{
    if (m_state == GameState::Playing)
        m_time += dt;

    UpdateUI(dt);
}

void AstroVoidLayer::OnRender(SpriteBatch2D& batch)
{
    m_scene.Render(batch);
    m_uiSystem.Render(batch);
}

void AstroVoidLayer::BuildUI()
{
    m_menuBackdrop = nullptr;
    m_menuScreen = nullptr;
    m_uiSystem.Clear();

    if (!m_uiFont)
        return;

    const float screenW = static_cast<float>(m_app.Width());
    const float screenH = static_cast<float>(m_app.Height());

    const AstroVoidMenuTheme scaledTheme{
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
    };

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
    style.captionScale = scaledTheme.bodyScale * 0.85f;
    style.buttonTextScale = scaledTheme.buttonTextScale;

    UIScreen& menu = m_uiSystem.CreateScreen("Menu");
    auto& menuRoot = menu.Root();

    constexpr int kMenuButtonCount = 2;
    const float panelPaddingX = scaledTheme.buttonPadding.x * 2.0f;
    const float panelPaddingY = scaledTheme.buttonPadding.y * 4.0f;
    const float contentWidth = scaledTheme.buttonSize.x + (panelPaddingX * 2.0f);
    const float buttonsHeight = (scaledTheme.buttonSize.y * kMenuButtonCount) +
        (scaledTheme.verticalSpacing * static_cast<float>(kMenuButtonCount - 1));
    const float contentHeight = scaledTheme.titleSpacing + (scaledTheme.buttonSize.y * 0.75f) +
        buttonsHeight + panelPaddingY;

    auto& dim = menuRoot.EmplaceChild<UIPanel>("Menu.Backdrop");
    dim.SetColor(kMenuTheme.background);
    dim.SetAnchor(UIAnchor::Center);
    dim.SetSize({ contentWidth, contentHeight });
    m_menuBackdrop = &dim;

    auto& stack = menuRoot.EmplaceChild<UIElement>("Menu.Stack");
    stack.SetAnchor(UIAnchor::Center);

    float yOffset = -scaledTheme.titleSpacing;

    auto& title = stack.EmplaceChild<UILabel>("Menu.Title");
    style.ApplyHeading(title);
    title.SetAnchor(UIAnchor::Center);
    title.SetScale(scaledTheme.titleScale + (0.25f * m_uiScale));
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

    makeMenuButton("Menu.NewGame", "NEW GAME", [this]() {
        m_time = 0.0f;
        m_state = GameState::Playing;
    });

    makeMenuButton("Menu.Quit", "QUIT", []() {
        SDL_Event quit{};
        quit.type = SDL_QUIT;
        SDL_PushEvent(&quit);
    });

    menu.SetVisible(true);
    m_menuScreen = &menu;
}

void AstroVoidLayer::UpdateUI(float dt)
{
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

    if (m_menuScreen)
        m_menuScreen->SetVisible(m_state == GameState::Title);

    m_uiSystem.Update(dt);
}

float AstroVoidLayer::ComputeUiScale(float screenWidth, float screenHeight) const
{
    constexpr float kReferenceWidth = 1600.0f;
    constexpr float kReferenceHeight = 900.0f;

    const float widthScale = screenWidth / kReferenceWidth;
    const float heightScale = screenHeight / kReferenceHeight;
    const float rawScale = (std::min)(widthScale, heightScale);

    return std::clamp(rawScale, 0.6f, 1.5f);
}

