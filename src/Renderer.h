#pragma once

#include "Scene.h"
#include "Integrator.h"

#include <memory>

namespace Rays { class ImageBlock; }

namespace Rays
{

class Renderer
{
public:
    Renderer();
    ~Renderer() {}

    void Render() const;
    void RenderBlock(ImageBlock& block) const;

private:
    std::unique_ptr<Scene> m_Scene = nullptr;
    std::unique_ptr<Integrator> m_Integrator = nullptr;

    void CreateScene(Scene& scene);
};

}   // namespace Rays