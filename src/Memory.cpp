#include "Memory.h"

#include <algorithm>

namespace Hikari
{

    void* AllocAligned(size_t size)
    {
        return _aligned_malloc(size, L1_CACHE_LINE_SIZE);
    }

    void FreeAligned(void* ptr)
    {
        if (!ptr) return;
        _aligned_free(ptr);
    }

    MemoryArena::~MemoryArena()
    {
        FreeAligned(m_CurrentBlock);
        for (auto& block : m_UsedBlocks) FreeAligned(block.second);
        for (auto& block : m_AvailableBlocks) FreeAligned(block.second);
    }

    void* MemoryArena::Alloc(size_t numBytes)
    {
        // Round up `numBytes` to minimum machine alignment
        // *(Don't know how this works?!)*
        numBytes = ((numBytes + 15) & (~15));
        if (m_CurrentBlockPos + numBytes > m_CurrentAllocSize)
        {
            // Add current block to used blocks list
            if (m_CurrentBlock)
            {
                m_UsedBlocks.push_back(std::make_pair(m_CurrentAllocSize, m_CurrentBlock));
                m_CurrentBlock = nullptr;
            }

            // Try to get a memory block from `m_AvailableBlocks` so we don't
            // have to reallocate space.
            for (auto iter = m_AvailableBlocks.begin(); iter != m_AvailableBlocks.end(); ++iter)
            {
                if (iter->first >= numBytes)
                {
                    m_CurrentAllocSize = iter->first;
                    m_CurrentBlock = iter->second;
                    m_AvailableBlocks.erase(iter);
                    break;
                }
            }

            // Get a new block of memory for `MemoryArena`
            if (!m_CurrentBlock)
            {
                m_CurrentAllocSize = std::max(numBytes, m_BlockSize);
                m_CurrentBlock = AllocAligned<uint8_t>(m_CurrentAllocSize);
            }

            m_CurrentBlockPos = 0;
        }
        void* ret = m_CurrentBlock + m_CurrentBlockPos;
        m_CurrentBlockPos += numBytes;
        return ret;
    }

    void MemoryArena::Reset()
    {
        m_CurrentBlockPos = 0;
        m_AvailableBlocks.splice(m_AvailableBlocks.begin(), m_UsedBlocks);
    }

    size_t MemoryArena::TotalAllocated() const
    {
        size_t total = m_CurrentAllocSize;
        for (auto& block : m_UsedBlocks) total += block.first;
        for (auto& block : m_AvailableBlocks) total += block.first;
        return total;
    }

}   // namespace Hikari