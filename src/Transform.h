#pragma once

#include "Interaction.h"
#include "Ray.h"

#include "glm/mat4x4.hpp"

namespace Hikari
{

class Transform
{
public:
    Transform();
    Transform(const glm::mat4& matrix);
    Transform(const glm::mat4& matrix, const glm::mat4& matrixInverse);

    // TODO: Distinguish between Points, Vectors and Normals so that
    // we can overload a single operator `*` or `operator()` to apply
    // transforms.
    inline glm::vec3 Transform::TransformPoint(const glm::vec3& point) const
    {
        glm::vec4 transformedPointHomogenous = m_Matrix * glm::vec4(point, 1.f);
        return transformedPointHomogenous.w == 1.f
            ? glm::vec3
            (
                transformedPointHomogenous.x,
                transformedPointHomogenous.y,
                transformedPointHomogenous.z
            )
            : glm::vec3
            (
                transformedPointHomogenous.x / transformedPointHomogenous.w,
                transformedPointHomogenous.y / transformedPointHomogenous.w,
                transformedPointHomogenous.z / transformedPointHomogenous.w
            );
    }

    inline glm::vec3 Transform::TransformVector(const glm::vec3& vector) const
    {
        return glm::vec3(m_Matrix * glm::vec4(vector, 0.f));
    }

    inline glm::vec3 Transform::TransformNormal(const glm::vec3& normal) const
    {
        return glm::vec3(glm::transpose(m_MatrixInverse) * glm::vec4(normal, 0.f));
    }
    
    inline Ray operator () (const Ray& ray) const
    {
        return Ray(TransformPoint(ray.m_Origin), TransformVector(ray.m_Direction), ray.m_tMax);
    }

    inline Interaction operator () (const Interaction& interaction) const
    {
        Interaction result;
        result.m_HitPoint = TransformPoint(interaction.m_HitPoint);
        result.m_Normal = glm::normalize(TransformNormal(interaction.m_Normal));
        result.m_Albedo = interaction.m_Albedo;
        result.m_Shape = interaction.m_Shape;

        return result;
    }

    Transform operator * (const Transform& t) const;
    
    // Tells if this `Transform` swaps handedness i.e. from left-handed
    // coordinate system to right-handed coordinate system
    bool SwapsHandedness() const;

    friend Transform Inverse(const Transform& transform);
    friend Transform Transpose(const Transform& transform);

private:
    glm::mat4 m_Matrix;
    glm::mat4 m_MatrixInverse;
};

Transform Translate(const glm::vec3& delta);
Transform Scale(const glm::vec3& factor);
Transform Rotate(float degrees, const glm::vec3& axis);
Transform LookAt(const glm::vec3& origin, const glm::vec3& looksAt, const glm::vec3& up);

}   // namespace Hikari