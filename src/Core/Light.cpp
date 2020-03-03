#include "Light.h"

#include "Core/Scene.h"

namespace Hikari
{
    bool VisibilityTester::Unoccluded(const Scene& scene) const
    {
        return !scene.Occluded(m_P0.SpawnRayTo(m_P1));
    }

}	// namespace Hikari