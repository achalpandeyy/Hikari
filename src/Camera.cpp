#include "Camera.h"
#include "Ray.h"

#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cmath>

namespace Hikari
{

Camera::Camera(
    const glm::vec3&    position,
    const glm::vec3&    lookAt,
    const glm::vec2&    resolution,
    float               focalLength,
    float               fieldOfView)
    : m_Position(position), m_LookAt(lookAt), m_Resolution(resolution),
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

    // TODO: Not hardcode the world up vector.
    glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);

    // Note: The `glm::lookAt` matrix converts from world space to view (or camera space)
    // but here we want to convert the ray from view (or camera space) to world space so
    // that we can find intersections of it with the objects represented in world space,
    // hence we use inverse of the `glm::lookAt` matrix to convert the ray from view
    // space to world space.
    glm::mat4 inverseLookAtMatrix = glm::inverse(glm::lookAt(m_Position, m_LookAt, worldUp));

    // TODO: Divide by the w component of homogenous coordinate (glm::vec4)
    // instead of just dropping it, when you move from glm::vec4 to glm::vec3
    return Ray
    (
        glm::vec3(inverseLookAtMatrix * glm::vec4(ray.m_Origin, 1.f)),
        glm::vec3(inverseLookAtMatrix * glm::vec4(ray.m_Direction, 0.f))
    );
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