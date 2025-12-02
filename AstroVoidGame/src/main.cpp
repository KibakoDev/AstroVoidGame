// main.cpp
#include "KibakoEngine/Core/Application.h"
#include "AstroVoidLayer.h"

using namespace KibakoEngine;

int main()
{
    Application app;

    if (!app.Init(1920, 1080, "Astro Void"))
        return -1;

    // Push the gameplay layer that owns the scene and UI.
    AstroVoidLayer game(app);
    app.PushLayer(&game);

    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    app.Run(clearColor, true);

    app.Shutdown();
    return 0;
}