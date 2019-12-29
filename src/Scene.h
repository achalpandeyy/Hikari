#pragma once

#include "Sphere.h"
#include "Camera.h"
#include "Light.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include <memory>
#include <vector>

namespace Hikari { class Ray; }

struct Intersection
{
    bool m_Valid;
    float m_Distance;
    glm::vec3 m_Normal;
    glm::vec2 m_TextureCoordinates;
    glm::vec3 m_Albedo;
};

namespace Hikari
{

class Scene
{
public:
    // TODO: Make this a vector of Objects (base class for all supported "hitables"
    // - spheres, boxes, indexed triangle meshes)
    // TODO: This memeber is `public` because we need it to hardcode a scene
    // in the `Renderer`, when we remove hardcoding of the scene I think
    // this will also be safe to make `private`.
    std::vector< std::unique_ptr<Sphere> > m_Spheres;

    // TODO: Make this functioning.
    // TODO: This memeber is `public` because we need it to hardcode a scene
    // in the `Renderer`, when we remove hardcoding of the scene I think
    // this will also be safe to make `private`.
    std::vector< std::unique_ptr<Light> > m_Lights;

    // TODO: This memeber is `public` because we need it to hardcode a scene
    // in the `Renderer`, when we remove hardcoding of the scene I think
    // this will also be safe to make `private`.
    std::unique_ptr<Camera> m_Camera;

    bool Intersect(const Ray& ray) const;
    bool Intersect(const Ray& ray, Intersection& intersection) const;
};

}   // namespace Hikari