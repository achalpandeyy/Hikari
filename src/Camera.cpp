#define _USE_MATH_DEFINES

#include "Camera.h"

#include "Ray.h"
#include "Transform.h"

#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cmath>
#include <iostream>

namespace Hikari
{

Camera::Camera(
    const glm::vec3&    position,
    const glm::vec3&    lookAt,
    const glm::vec3&    up,
    const glm::vec2&    resolution,
    float               focalLength,
    float               fieldOfView)
    : m_Position(position), m_LookAt(lookAt), m_Up(up), m_Resolution(resolution),
    m_FocalLength(focalLength), m_FieldOfView(fieldOfView)
{}

Ray Camera::SpawnRay(const glm::vec2& rasterCoordinates) const
{
    float aspectRatio = static_cast<float>(m_Resolution.x) / m_Resolution.y;
    double fieldOfViewScale = std::tan(glm::radians(m_FieldOfView) * 0.5f);
    float cameraX = (2.f * ((rasterCoordinates.x + 0.5f) / m_Resolution.x) - 1.f) * aspectRatio * fieldOfViewScale;
    float cameraY = (1.f - 2.f * ((rasterCoordinates.y + 0.5f) / m_Resolution.y)) * fieldOfViewScale;

    // Construct a Ray in camera space.
    glm::vec3 rayOrigin = glm::vec3(0.f);
    glm::vec3 rayDirection = glm::normalize(glm::vec3(cameraX, cameraY, -m_FocalLength));
    Ray ray(rayOrigin, rayDirection);

    Transform lookAt = LookAt(m_Position, m_LookAt, m_Up);
    return lookAt(ray);
}

glm::vec2 Camera::GetResolution() const
{
    return m_Resolution;
}

void Camera::SetResolution(const glm::vec2& resolution)
{
    m_Resolution = resolution;
}

}   // namespace Hikari