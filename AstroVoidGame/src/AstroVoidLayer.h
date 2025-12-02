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

// Primary gameplay layer used by the sample. This keeps the engine hookup,
// UI, and scene ownership in one place so you can swap in your own gameplay
// without touching the application bootstrap.
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

private:
    KibakoEngine::Application& m_app;

    // Ready your gameplay content
    KibakoEngine::Scene2D m_scene;

    // UI
    KibakoEngine::UISystem  m_uiSystem;
    const KibakoEngine::Font* m_uiFont = nullptr;

    KibakoEngine::UILabel* m_titleLabel = nullptr;
    KibakoEngine::UILabel* m_subtitleLabel = nullptr;
    KibakoEngine::UILabel* m_hintLabel = nullptr;
    KibakoEngine::UIScreen* m_hudScreen = nullptr;

    float m_time = 0.0f;
};