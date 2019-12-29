#pragma once

#include "Renderer.h"

#include <memory>

namespace Hikari
{

class App
{
public:
    App();
    ~App();
    void Start() const;
    void Render();

private:
    std::unique_ptr<Renderer> m_Renderer;
};

}   // namespace Hikari