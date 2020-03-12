#pragma once

#include <glm/glm.hpp>
#include <tbb/tbb.h>

// TODO(achal): Put it elsewhere.
#define BLOCK_DIMENSION 32u

namespace Hikari
{

class ImageBlock
{
public:
    ImageBlock(const glm::vec2& dim);

    ~ImageBlock();

    void Put(const ImageBlock& block);

    void WriteImage() const;

    glm::vec2 m_Position;
    glm::vec2 m_Dimensions;
    glm::vec3* m_Data;

private:
    tbb::mutex m_Mutex;
};

}   // namespace Hikari