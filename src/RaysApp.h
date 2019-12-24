#pragma once

#include "Renderer.h"

#include "glm/vec2.hpp"

#include <memory>

namespace Rays
{

enum E_Resolution
{
    _640x400 = 0,
    _320x200,
    _1x1
};

class RaysApp
{
private:
    std::unique_ptr<Renderer> m_Renderer;

    // GUI Exposed Render options
    bool m_MultiThreading = true;
    bool m_FixedPrimitivesMode = false;
    unsigned int m_IndirectRays = 1u;
    E_Resolution m_Resolution = _640x400;

public:
    RaysApp();
    ~RaysApp();

    void Start() const;
    void Render();
};

}   // namespace Rays