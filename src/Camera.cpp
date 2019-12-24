#include "Camera.h"
#include "Ray.h"

#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cmath>
#include <fstream>

namespace Rays
{

Camera::Camera(
    const glm::vec3&    position,
    const glm::vec3&    lookAt,
    unsigned int        filmWidth,
    unsigned int        filmHeight,
    double              focalLength,
    float               fieldOfView)
    : m_Position(position), m_LookAt(lookAt), m_FilmWidth(filmWidth), m_FilmHeight(filmHeight),
    m_FocalLength(focalLength), m_FieldOfView(fieldOfView)
{
    m_Film = new glm::vec3[m_FilmHeight * m_FilmWidth];
}

Camera::~Camera()
{
    if (m_Film)
    {
        delete[] m_Film;
    }
}

Ray Camera::SpawnRay(unsigned int rasterX, unsigned int rasterY) const
{
    float aspectRatio = static_cast<float>(m_FilmWidth) / m_FilmHeight;
    double fieldOfViewScale = std::tan(glm::radians(m_FieldOfView) * 0.5f);
    float cameraX = (2.f * ((rasterX + 0.5f) / m_FilmWidth) - 1.f) * aspectRatio * fieldOfViewScale;
    float cameraY = (1.f - 2.f * ((rasterY + 0.5f) / m_FilmHeight)) * fieldOfViewScale;

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
    return Ray(
        glm::vec3(inverseLookAtMatrix * glm::vec4(ray.m_Origin, 1.f)),
        glm::vec3(inverseLookAtMatrix * glm::vec4(ray.m_Direction, 0.f))
    );
}

void Camera::SetPixelRadiance(unsigned int rasterX, unsigned int rasterY, const glm::vec3& radiance)
{
    m_Film[rasterX + rasterY * m_FilmWidth] = radiance;
}

void Camera::WriteImage()
{
    std::ofstream imageFile("Image.ppm");
    imageFile << "P3\n" << m_FilmWidth << ' ' << m_FilmHeight << "\n255\n";
    for (uint32_t i = 0; i < m_FilmWidth * m_FilmHeight; i++)
    {
        unsigned int r = (unsigned int)(255.f * glm::clamp(m_Film[i].x, 0.f, 1.f));
        unsigned int g = (unsigned int)(255.f * glm::clamp(m_Film[i].y, 0.f, 1.f));
        unsigned int b = (unsigned int)(255.f * glm::clamp(m_Film[i].z, 0.f, 1.f));
        imageFile << r << ' ' << g << ' ' << b << '\n';
    }
    imageFile.close();
}

}   // namespace Rays