// AstroVoidLayer.h
#pragma once

#include <cstdint>

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
    float buttonX = 0.0f;
    float newGameY = 0.0f;
    float quitGameY = 0.0f;
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

    // Menu elements
    KibakoEngine::UILabel* m_titleLabel = nullptr;
    KibakoEngine::UIButton* m_newGameButton = nullptr;
    KibakoEngine::UIButton* m_quitButton = nullptr;
    KibakoEngine::UIScreen* m_menuScreen = nullptr;

    ScreenSize m_lastScreenSize{};

    float m_time = 0.0f;
};
