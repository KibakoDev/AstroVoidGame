// AstroVoidLayer.h
#pragma once

#include "KibakoEngine/Core/Layer.h"
#include "KibakoEngine/Renderer/SpriteTypes.h"
#include "KibakoEngine/Scene/Scene2D.h"

namespace KibakoEngine {
    class Application;
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
    KibakoEngine::Application& m_app;

    KibakoEngine::Scene2D m_scene;

    float m_time = 0.0f;
};