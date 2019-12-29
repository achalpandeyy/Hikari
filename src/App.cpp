#include "App.h"

#include "nanogui/nanogui.h"

namespace Hikari
{

App::App()
{
    m_Renderer = std::make_unique<Renderer>();

    nanogui::init();

    nanogui::Screen* screen(new nanogui::Screen(nanogui::Vector2i(512, 512), "Hikari"));
    
    nanogui::FormHelper* formHelper(new nanogui::FormHelper(screen));

    nanogui::ref<nanogui::Window> window = formHelper->addWindow(Eigen::Vector2i(50, 50), "Render Options");

    formHelper->addVariable("Multithreading", m_Renderer->m_RenderOptions.m_Multithreading);
    formHelper->addVariable("Fixed Primitives", m_Renderer->m_RenderOptions.m_FixedPrimitivesMode);

    auto var = formHelper->addVariable("No. of Indirect Rays", m_Renderer->m_RenderOptions.m_NumIndirectRays);
    var->setMinValue(0);
    var->setMaxValue(2048);
    var->setSpinnable(true);

    auto resolutionVar = formHelper->addVariable("Resolution", m_Renderer->m_RenderOptions.m_Resolution, true);
    resolutionVar->setItems({ "640 x 400", "320 x 200", "1 x 1" });
    resolutionVar->setCallback([&](const E_Resolution& res)
    {
        m_Renderer->SetResolution(res);
    });

    formHelper->addButton("Load Scene", []() {std::cout << "Scene Loaded." << std::endl; });

    formHelper->addButton("Render", [this]() { Render(); });

    screen->setVisible(true);
    screen->performLayout();
}

App::~App()
{
    nanogui::shutdown();
}

void App::Start() const
{
    nanogui::mainloop();
}

void App::Render()
{
    m_Renderer->Render();
}

}   // namespace Hikari