#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace Hikari { class Ray; }

namespace Hikari
{

class Camera
{
public:
    Camera(
        const glm::vec3&    position,
        const glm::vec3&    lookAt,
        const glm::vec2&    resolution,
        float               focalLength = 1.f,
        float               fieldOfView = 90.f);

    Ray SpawnRay(const glm::vec2& rasterCoordinates) const;
    glm::vec2 GetResolution() const;
    void SetResolution(const glm::vec2& resolution);

private:
    glm::vec3 m_Position;
    glm::vec3 m_LookAt;
    glm::vec2 m_Resolution;
    float m_FocalLength;
    float m_FieldOfView;
};

}   // namespace Hikari