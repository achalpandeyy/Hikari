#include "Transform.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Hikari
{

Transform::Transform()
{
    m_Matrix = glm::mat4
    (
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    );
    m_MatrixInverse = m_Matrix;
}

Transform::Transform(const glm::mat4& matrix)
    : m_Matrix(matrix), m_MatrixInverse(glm::inverse(matrix))
{}

Transform::Transform(const glm::mat4& matrix, const glm::mat4& matrixInverse)
    : m_Matrix(matrix), m_MatrixInverse(matrixInverse)
{}

Transform Transform::operator * (const Transform& t) const
{
    return Transform(m_Matrix * t.m_Matrix, t.m_MatrixInverse * m_MatrixInverse);
}

bool Transform::SwapsHandedness() const
{
    float determinant3x3 =
        m_Matrix[0][0] * (m_Matrix[1][1] * m_Matrix[2][2] - m_Matrix[1][2] * m_Matrix[2][1]) -
        m_Matrix[0][1] * (m_Matrix[1][0] * m_Matrix[2][2] - m_Matrix[1][2] * m_Matrix[2][0]) +
        m_Matrix[0][2] * (m_Matrix[1][0] * m_Matrix[2][1] - m_Matrix[1][1] * m_Matrix[2][0]);
    return determinant3x3 < 0.f;
}

Transform Inverse(const Transform& transform)
{
    return Transform(transform.m_MatrixInverse, transform.m_Matrix);
}

Transform Transpose(const Transform& transform)
{
    return Transform(glm::transpose(transform.m_Matrix), glm::transpose(transform.m_MatrixInverse));
}

Transform Translate(const glm::vec3& delta)
{
    glm::mat4 matrix = glm::translate(matrix, delta);
    glm::mat4 matrixInverse = glm::translate(matrixInverse, -delta);
    return Transform(matrix, matrixInverse);
}

Transform Scale(const glm::vec3& factor)
{
    glm::mat4 matrix = glm::scale(matrix, factor);
    glm::mat4 matrixInverse = glm::scale(matrixInverse, 1.f / factor);
    return Transform(matrix, matrixInverse);
}

Transform Rotate(float degrees, const glm::vec3& axis)
{
    glm::mat4 matrix = glm::rotate(matrix, glm::radians(degrees), axis);
    glm::mat4 matrixInverse = glm::transpose(matrix);
    return Transform(matrix, matrixInverse);
}

// Note: The `glm::lookAt` matrix converts from world space to view (or camera space)
// but here we want to convert from view (or camera space) to world space so
// that we can find ray-object intersections with the objects represented in world space,
// hence we use inverse of the `glm::lookAt` matrix to convert the ray from view
// space to world space.
// `up` determines how the camera will be oriented along the viewing direction
Transform LookAt(const glm::vec3& origin, const glm::vec3& looksAt, const glm::vec3& up)
{
    glm::mat4 matrixInverse = glm::lookAt(origin, looksAt, glm::normalize(up));
    glm::mat4 matrix = glm::inverse(matrixInverse);
    return Transform(matrix, matrixInverse);
}

}   // namespace Hikari