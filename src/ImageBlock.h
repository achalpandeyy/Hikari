#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "tbb/mutex.h"

#define BLOCK_DIMENSION 32

namespace Hikari
{

class ImageBlock
{
public:
    glm::vec2 m_Position;
    glm::vec2 m_Dimensions;
    glm::vec3* m_Data;

    ImageBlock(const glm::vec2& dim);
    ~ImageBlock();
    void Put(const ImageBlock& block);
    void WriteImage() const;

private:
    tbb::mutex m_Mutex;
};

}   // namespace Hikari