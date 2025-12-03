// AstroVoidLayer.h
#pragma once

#include "KibakoEngine/Core/Layer.h"
#include "KibakoEngine/Renderer/SpriteTypes.h"
#include "KibakoEngine/Scene/Scene2D.h"
#include "KibakoEngine/Fonts/Font.h"
#include "KibakoEngine/UI/UIControls.h"

namespace KibakoEngine {
    class Application;
}

enum class GameState
{
    Title,
    Playing,
};

class AstroVoidLayer final : public KibakoEngine::Layer
{
public:
    explicit AstroVoidLayer(KibakoEngine::Application& app);

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float dt) override;
    void OnRender(KibakoEngine::SpriteBatch2D& batch) override;

private:
    void BuildUI();
    void UpdateUI(float dt);
    [[nodiscard]] float ComputeUiScale(float screenWidth, float screenHeight) const;

private:
    KibakoEngine::Application& m_app;

    KibakoEngine::Scene2D m_scene;
    const KibakoEngine::Font* m_uiFont = nullptr;
    KibakoEngine::UISystem m_uiSystem;
    KibakoEngine::UIPanel* m_menuBackdrop = nullptr;
    KibakoEngine::UIScreen* m_menuScreen = nullptr;

    GameState m_state = GameState::Title;
    float m_time = 0.0f;
    float m_uiScale = 1.0f;
    float m_lastUiWidth = 0.0f;
    float m_lastUiHeight = 0.0f;
};
