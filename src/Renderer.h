#pragma once

#include "Camera.h"
#include "EmbreeRTEngine.h"
#include "Integrator.h"

#include <glm/vec2.hpp>

#include <memory>

namespace Hikari
{
    class ImageBlock;

    enum E_Resolution
    {
        _640x400_,
        _320x200_,
        _1x1_
    };

    // GUI controlled render options
    struct RenderOptions
    {
        bool m_Multithreading = true;
        bool m_FixedPrimitivesMode = false;
        unsigned int m_NumIndirectRays = 1u;
        E_Resolution m_Resolution = _640x400_;
    };

class Renderer
{
public:
    RenderOptions m_RenderOptions;

    Renderer();
    ~Renderer() {}
    void Render() const;
    void RenderBlock(ImageBlock& block) const;
    void SetResolution(const E_Resolution& res);

private:
    std::shared_ptr<EmbreeRTEngine> m_RTEngine = nullptr;
    std::unique_ptr<Integrator> m_Integrator = nullptr;
    std::unique_ptr<Camera> m_Camera = nullptr;

    glm::vec2 GetResolution() const;
};

}   // namespace Hikari