#define _USE_MATH_DEFINES
#include "Whitted.h"

#include "Interaction.h"
#include "Ray.h"

#include <glm/glm.hpp>

#include <algorithm>
#include <cmath>
#include <random>

namespace Hikari
{
    glm::vec3 WhittedIntegrator::Li(const Ray& ray, Sampler& sampler, const std::shared_ptr<Scene>& scene) const
    {
        // TODO: Do not hard code background color
        glm::vec3 hitColor(0.f);

        Interaction interaction = scene->Intersect(ray);
        if (!interaction.m_Shape)
            return hitColor;

        // Reset hitColor to black, so that it does not have contribution
        // from the background color.
        hitColor = glm::vec3(0.f);

        // Calculate light contributions and shadows.
        for (auto light : scene->m_Lights)
        {
            Ray lightRay = light->GetLightRay(interaction);

            if (scene->Occluded(lightRay))
                continue;

            // Note: Divding the albedo by PI enables us to specify the albedo in the
            // range [0, 1] while making sure that energy is conserved i.e. the total
            // amount of light reflected off the surface should always be less than or
            // equal to the sum of amount of light received by the surface and emitted
            // by the surface.
            //
            glm::vec3 albedo = interaction.m_Shape->m_Material->m_Color->Evaluate(glm::vec2(0.f));
            glm::vec3 diffuse = (albedo / glm::vec3(M_PI))
                * (light->GetIncidentRadiance(interaction.m_HitPoint))
                * std::max(0.f, glm::dot(lightRay.m_Direction, interaction.m_Normal));

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