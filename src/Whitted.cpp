#define _USE_MATH_DEFINES
#include "Whitted.h"

#include "Interaction.h"
#include "Ray.h"
#include "Scene.h"

#include "glm/vec3.hpp"
#include "glm/geometric.hpp"

#include <algorithm>
#include <cmath>

namespace Hikari
{

glm::vec3 WhittedIntegrator::Li(const Ray& ray, const Scene& scene, unsigned int rayDepth) const
{
    Interaction interaction;

    // TODO: Do not hard code background color
    glm::vec3 hitColor(1.f);

    if (!scene.Intersect(ray, interaction))
        return hitColor;

    // Reset hitColor to black, so that it does not have contribution
    // from the background color.
    hitColor = glm::vec3(0.f);

    // Calculate light contributions and shadows.
    for (size_t i = 0; i < scene.m_Lights.size(); i++)
    {
        glm::vec3 lightDirection;
        float distanceToLight;
        switch (scene.m_Lights[i]->m_LightType)
        {
            case Light::E_POINT_LIGHT:
                lightDirection = glm::normalize(scene.m_Lights[i]->m_Position - interaction.m_HitPoint);
                distanceToLight = glm::distance(interaction.m_HitPoint, scene.m_Lights[i]->m_Position);
                break;
            case Light::E_DIRECTIONAL_LIGHT:
                lightDirection = scene.m_Lights[i]->m_Direction;
                distanceToLight = std::numeric_limits<float>::max();
                break;
            case Light::E_SPOT_LIGHT:
                // TODO: Update this after implementing Spot Light
                lightDirection = glm::vec3(0.f);
                distanceToLight = -1.f;
                break;
        }

        // TODO: Move bias somewhere else with other non user-controlled render options
        float bias = 1e-3f;

        Ray shadowRay(interaction.m_HitPoint + interaction.m_Normal * bias, lightDirection, distanceToLight);
        bool inShadow = scene.Intersect(shadowRay);

        // Note: Divding the albedo by PI enables us to specify the albedo in the range [0, 1]
        // while making sure that energy is conserved i.e. the total amount of light reflected
        // off the surface should always be less than or equal to the sum of amount of light
        // received by the surface and emitted by the surface.
        glm::vec3 diffuse = (interaction.m_Albedo / glm::vec3(M_PI)) * (scene.m_Lights[i]->GetIncidentLight(interaction.m_HitPoint)) * std::max(0.f, glm::dot(lightDirection, interaction.m_Normal) * static_cast<int>(!inShadow));

        // Compute Specular component.
        // glm::vec3 viewDirection = -ray.m_Direction;
        // Ray reflectedRay = getReflectedRay(Ray(scene.m_Lights[i]->m_Position(), -lightDirection), hitPoint, intersection.m_Normal);
        // float n = 250.f;
        // glm::vec3 specular = scene.mLights[i]->getIncidentLight(hitPoint) * (std::pow(std::max(0.f, glm::dot(viewDirection, reflectedRay.mDirection)), n) * static_cast<int>(!inShadow));

        // hitColor += 0.15f * specular + 0.85f * diffuse;
        hitColor += 0.85f * diffuse;
    }

    return hitColor;
}

}   // namespace Hikari