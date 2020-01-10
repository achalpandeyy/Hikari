#pragma once

#include "Camera.h"
#include "Light.h"
#include "Primitive.h"

#include <memory>
#include <vector>

namespace Hikari
{

class Interaction;
class Ray;

class Scene
{
public:
    // TODO: Make this a vector of Objects (base class for all supported "hitables"
    // - spheres, boxes, indexed triangle meshes)
    // TODO: This memeber is `public` because we need it to hardcode a scene
    // in the `Renderer`, when we remove hardcoding of the scene I think
    // this will also be safe to make `private`.
    std::vector< std::shared_ptr<Primitive> > m_Primitives;

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
    bool Intersect(const Ray& ray, Interaction& interaction) const;
};

}   // namespace Hikari