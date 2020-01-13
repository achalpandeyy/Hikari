#pragma once

#include <list>
#include <utility>

namespace Hikari
{

#ifndef L1_CACHE_LINE_SIZE
#define L1_CACHE_LINE_SIZE 64
#endif

#define ALLOCA(TYPE, COUNT) static_cast<TYPE*>(alloca(COUNT * sizeof(TYPE)))

    // TODO(achal): This function is not portable, make it so.
    void* AllocAligned(size_t size);

    template <typename T>
    T* AllocAligned(size_t count)
    {
        return static_cast<T*>(AllocAligned(count * sizeof(T)));
    }

    void FreeAligned(void* ptr);

    class MemoryArena
    {
    public:
        MemoryArena(size_t blockSize = 262144) : m_BlockSize(blockSize) {}
        ~MemoryArena();

        void* Alloc(size_t numBytes);

        template <typename T>
        T* Alloc(size_t count = 1, bool runConstructor = true)
        {
            T* ret = static_cast<T*>(Alloc(count * sizeof(T)));

            if (runConstructor)
            {
                for (size_t i = 0; i < count; ++i)
                {
                    new (&ret[i]) T();
                }
            }
            return ret;
        }

        void Reset();
        size_t TotalAllocated() const;

    private:
        const size_t m_BlockSize;

        // `m_CurrentBlockPos` is the offset of the first free location in
        // the current memory block.
        size_t m_CurrentBlockPos = 0, m_CurrentAllocSize = 0;
        uint8_t* m_CurrentBlock = nullptr;
        std::list< std::pair<size_t, uint8_t*> > m_UsedBlocks, m_AvailableBlocks;
    };

}   // namespace Hikari
