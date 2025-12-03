// AstroVoidLayer.h
#pragma once

#include <cstdint>
#include <string>

#include "KibakoEngine/Core/Layer.h"
#include "KibakoEngine/Renderer/SpriteTypes.h"
#include "KibakoEngine/Renderer/Texture2D.h"
#include "KibakoEngine/Scene/Scene2D.h"
#include "KibakoEngine/Fonts/Font.h"
#include "KibakoEngine/UI/UIElement.h"
#include "KibakoEngine/UI/UIControls.h"

namespace KibakoEngine {
    class Application;
}

enum class GameState
{
    Title, // Menu
    Playing // In Game
};

struct ScreenSize
{
    float width = 0.0f;
    float height = 0.0f;
};

struct MenuLayout
{
    float headingX = 0.0f;
    float headingY = 0.0f;
    float primaryButtonX = 0.0f;
    float primaryButtonY = 0.0f;
    float primaryButtonWidth = 0.0f;
    float primaryButtonHeight = 0.0f;
    float secondaryButtonX = 0.0f;
    float secondaryButtonY = 0.0f;
    float secondaryButtonWidth = 0.0f;
    float secondaryButtonHeight = 0.0f;
};

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

struct MenuContent
{
    std::string title = "ASTRO VOID";
    std::string startLabel = "NOUVELLE PARTIE";
    std::string controlsLabel = "TOUCHES";
};

// Primary gameplay layer
class AstroVoidLayer final : public KibakoEngine::Layer
{
public:
    explicit AstroVoidLayer(KibakoEngine::Application& app);

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float dt) override;
    void OnRender(KibakoEngine::SpriteBatch2D& batch) override;

private:
    // UI helpers
    void BuildUI();
    void UpdateUI(float dt);
    void ApplyMenuLayout(const struct MenuLayout& layout);
    MenuLayout CalculateLayout(const ScreenSize& screen) const;
    void BuildMenuElements(KibakoEngine::UIElement& root);
    void ResetMenuReferences();

    // State logic
    void UpdateTitle(float dt);
    void UpdatePlaying(float dt);

private:
    KibakoEngine::Application& m_app;

    GameState m_state = GameState::Title;

    // Ready your gameplay content
    KibakoEngine::Scene2D m_scene;

    // UI
    KibakoEngine::UISystem  m_uiSystem;
    const KibakoEngine::Font* m_uiFont = nullptr;
    KibakoEngine::UIStyle m_menuStyle{};
    MenuTheme m_menuTheme{};
    MenuContent m_menuContent{};

    // Menu elements
    KibakoEngine::UILabel* m_titleLabel = nullptr;
    KibakoEngine::UIButton* m_newGameButton = nullptr;
    KibakoEngine::UIButton* m_quitButton = nullptr;
    KibakoEngine::UIScreen* m_menuScreen = nullptr;

    ScreenSize m_lastScreenSize{};

    float m_time = 0.0f;
};
