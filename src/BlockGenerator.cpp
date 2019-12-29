#include "BlockGenerator.h"
#include "ImageBlock.h"

#include <iostream>

namespace Hikari
{

BlockGenerator::BlockGenerator(const glm::vec2& framebufferDim, unsigned int blockDim)
    : m_FramebufferDimensions(framebufferDim), m_BlockDimension(blockDim)
{
    m_NumBlocks = glm::vec2
    (
        static_cast<unsigned int>(std::ceil(static_cast<float>(m_FramebufferDimensions.x) / m_BlockDimension)),
        static_cast<unsigned int>(std::ceil(static_cast<float>(m_FramebufferDimensions.y) / m_BlockDimension))
    );
    m_BlocksLeft = m_NumBlocks.x * m_NumBlocks.y;
    m_CurrentBlockPosition = glm::vec2(-1, 0);
}

unsigned int BlockGenerator::GetBlockCount() const
{
    return m_NumBlocks.x * m_NumBlocks.y;
}

bool BlockGenerator::NextBlock(ImageBlock& imageBlock)
{
    tbb::mutex::scoped_lock lock(m_Mutex);

    if (m_BlocksLeft == 0u)
    {
        return false;
    }

    if (m_CurrentBlockPosition.x < m_NumBlocks.x - 1)
    {
        imageBlock.m_Position.x = m_CurrentBlockPosition.x + 1;
        imageBlock.m_Position.y = m_CurrentBlockPosition.y;
    }
    // The case shouldn't arise when we exceed both x and y because 
    // `m_BlocksLeft` would become 0
    else
    {
        imageBlock.m_Position.x = 0;
        imageBlock.m_Position.y = m_CurrentBlockPosition.y + 1;
    }
    m_CurrentBlockPosition = imageBlock.m_Position;
    m_BlocksLeft--;
    return true;
}

}   // namespace Hikari