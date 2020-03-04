#include "App.h"

namespace Hikari
{

App::App()
{
    m_Renderer = std::make_unique<Renderer>();
}


void App::Start() const
{
    m_Renderer->Render();
}

}   // namespace Hikari