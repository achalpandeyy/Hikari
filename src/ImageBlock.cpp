#define NOMINMAX

#include "ImageBlock.h"

#include <glm/geometric.hpp>

#include <fstream>

namespace Hikari
{
    ImageBlock::ImageBlock(const glm::vec2& dim) : m_Position(0, 0), m_Dimensions(dim)
    {
        m_Data = new glm::vec3[m_Dimensions.x * m_Dimensions.y];
    }

    ImageBlock::~ImageBlock()
    {
        delete[] m_Data;
    }

    void ImageBlock::Put(const ImageBlock& block)
    {
        // 1. Decide the for loop ranges
        //
        unsigned int maxY = glm::min((block.m_Position.y + 1) * BLOCK_DIMENSION, m_Dimensions.y);
        unsigned int maxX = glm::min((block.m_Position.x + 1) * BLOCK_DIMENSION, m_Dimensions.x);

        // 2. Lock the `m_Data` framebuffer
        //
        tbb::mutex::scoped_lock lock(m_Mutex);

        unsigned int k = 0u;
        for (unsigned int j = block.m_Position.y * BLOCK_DIMENSION; j < maxY; ++j)
        {
            for (unsigned int i = block.m_Position.x * BLOCK_DIMENSION; i < maxX; ++i)
            {
                // 3. Copy over the data
                unsigned int idx = j * m_Dimensions.x + i;
                m_Data[idx] = block.m_Data[k++];
            }
        }
    }

    void ImageBlock::WriteImage() const
    {
        std::ofstream imageFile("Image.ppm");
        imageFile << "P3\n" << m_Dimensions.x << ' ' << m_Dimensions.y << "\n255\n";
        for (uint32_t i = 0; i < m_Dimensions.x * m_Dimensions.y; i++)
        {
            unsigned int r = static_cast<unsigned int>(255.f * glm::clamp(std::sqrt(m_Data[i].r), 0.f, 1.f));
            unsigned int g = static_cast<unsigned int>(255.f * glm::clamp(std::sqrt(m_Data[i].g), 0.f, 1.f));
            unsigned int b = static_cast<unsigned int>(255.f * glm::clamp(std::sqrt(m_Data[i].b), 0.f, 1.f));
            imageFile << r << ' ' << g << ' ' << b << '\n';
        }
        imageFile.close();
    }

}   // namespace Hikari