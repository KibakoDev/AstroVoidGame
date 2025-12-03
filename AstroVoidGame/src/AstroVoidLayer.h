// AstroVoidLayer.h
#pragma once

#include "KibakoEngine/Core/Layer.h"
#include "KibakoEngine/Renderer/SpriteTypes.h"
#include "KibakoEngine/Scene/Scene2D.h"
#include "KibakoEngine/UI/UISystem.h"

#include <DirectXMath.h>

namespace KibakoEngine {
    class Application;
    class Font;
    class UILabel;
    class UIButton;
    class UIScreen;
    class UIPanel;
}

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
    float ComputeUiScale(float screenWidth, float screenHeight) const;
    void BuildUI();
    void UpdateUI(float dt);

    KibakoEngine::Application& m_app;

    KibakoEngine::Scene2D m_scene;

    KibakoEngine::UISystem m_uiSystem;

    KibakoEngine::Font* m_uiFont = nullptr;

    KibakoEngine::UILabel* m_titleLabel = nullptr;
    KibakoEngine::UILabel* m_timeLabel = nullptr;
    KibakoEngine::UILabel* m_stateLabel = nullptr;
    KibakoEngine::UILabel* m_entitiesLabel = nullptr;
    KibakoEngine::UIButton* m_resumeButton = nullptr;
    KibakoEngine::UIButton* m_exitButton = nullptr;
    KibakoEngine::UIScreen* m_hudScreen = nullptr;
    KibakoEngine::UIScreen* m_menuScreen = nullptr;
    KibakoEngine::UIPanel* m_menuBackdrop = nullptr;

    bool m_menuVisible = true;
    float m_time = 0.0f;
    float m_uiScale = 1.0f;
    float m_lastUiWidth = 0.0f;
    float m_lastUiHeight = 0.0f;
};
