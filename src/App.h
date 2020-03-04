#pragma once

#include "Renderer.h"

#include <memory>

namespace Hikari
{

class App
{
public:
    App();

    void Start() const;

private:
    std::unique_ptr<Renderer> m_Renderer;
};

}   // namespace Hikari