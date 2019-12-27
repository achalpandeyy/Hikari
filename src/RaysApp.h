#pragma once

#include "Renderer.h"

#include <memory>

namespace Rays
{

class RaysApp
{
public:
    RaysApp();
    ~RaysApp();
    void Start() const;
    void Render();

private:
    std::unique_ptr<Renderer> m_Renderer;
};

}   // namespace Rays