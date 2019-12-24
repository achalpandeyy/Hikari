#pragma once

#include "glm/vec3.hpp"

namespace Rays { class Ray; }

namespace Rays
{

class Camera
{
public:
    Camera(
        const glm::vec3&    position,
        const glm::vec3&    lookAt,
        unsigned int        filmWidth,
        unsigned int        filmHeight,
        double              focalLength = 1.0,
        float               fieldOfView = 90.f);

    ~Camera();

    Ray SpawnRay(unsigned int rasterX, unsigned int rasterY) const;
    void SetPixelRadiance(unsigned int rasterX, unsigned int rasterY, const glm::vec3& radiance);
    void WriteImage();

private:
    glm::vec3 m_Position;
    glm::vec3 m_LookAt;
    double m_FocalLength;
    float m_FieldOfView;

    // Film
    unsigned int m_FilmWidth;
    unsigned int m_FilmHeight;
    glm::vec3* m_Film;
};

}   // namespace Rays