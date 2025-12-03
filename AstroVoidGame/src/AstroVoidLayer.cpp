// AstroVoidLayer.cpp
#include "AstroVoidLayer.h"

#include "KibakoEngine/Core/Application.h"
#include "KibakoEngine/Core/Log.h"

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
    m_scene.Clear();
    m_time = 0.0f;

    KbkLog(kLogChannel, "AstroVoidLayer attached");
}

void AstroVoidLayer::OnDetach()
{
    m_scene.Clear();
    m_time = 0.0f;
}

void AstroVoidLayer::OnUpdate(float dt)
{
    m_time += dt;
}

void AstroVoidLayer::OnRender(SpriteBatch2D& batch)
{
    m_scene.Render(batch);
}