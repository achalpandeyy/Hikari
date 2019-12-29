#pragma once

#include "glm/vec2.hpp"
#include "tbb/mutex.h"

namespace Hikari { class ImageBlock; }

namespace Hikari
{

class BlockGenerator
{
public:
    BlockGenerator(const glm::vec2& framebufferDim, unsigned int blockDim);
    unsigned int GetBlockCount() const;
    bool NextBlock(ImageBlock& imageBlock);

private:
    glm::vec2 m_FramebufferDimensions;
    unsigned int m_BlockDimension;
    glm::vec2 m_NumBlocks;
    unsigned int m_BlocksLeft;
    glm::vec2 m_CurrentBlockPosition;
    tbb::mutex m_Mutex;
};

}   // namespace Hikari
