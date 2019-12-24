#include "RaysApp.h"

#include "nanogui/nanogui.h"

#include <iostream>

namespace Rays
{

RaysApp::RaysApp()
{
    m_Renderer = std::make_unique<Renderer>();

    nanogui::init();

    nanogui::Screen* screen(new nanogui::Screen(nanogui::Vector2i(1280, 720),
        "NanoGUI test [GL 4.6]",
        true, false, 8, 8, 24, 8, 0, 4, 6));

    nanogui::FormHelper* formHelper(new nanogui::FormHelper(screen));

    nanogui::ref<nanogui::Window> window = formHelper->addWindow(Eigen::Vector2i(100, 50), "Render Options");

    bool multithreading = true;
    formHelper->addVariable("Multithreading", multithreading);

    bool fixedPrims = false;
    formHelper->addVariable("Fixed Primitives", fixedPrims);

    int rpp = 7;
    auto var = formHelper->addVariable("No. of Indirect Rays", rpp);
    var->setMinValue(0);
    var->setMaxValue(2048);
    var->setSpinnable(true);

    enum E_Resolution
    {
        res640x400 = 0,
        res320x200,
        res1x1
    };
    E_Resolution resolution;
    formHelper->addVariable("Resolution", resolution, true)
        ->setItems({ "640 x 400", "320 x 200", "1 x 1" });

    formHelper->addButton("Load Scene", []() {std::cout << "Scene Loaded." << std::endl; });

    formHelper->addButton("Render", [this]() { Render(); });
    screen->setVisible(true);
    screen->performLayout();
}

RaysApp::~RaysApp()
{
    nanogui::shutdown();
}

void RaysApp::Start() const
{
    nanogui::mainloop();
}

void RaysApp::Render()
{
    std::cout << "Rendering.." << std::endl;
    m_Renderer->Render();
    std::cout << "Rendered" << std::endl;
}

}   // namespace Rays