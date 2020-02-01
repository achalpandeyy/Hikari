#pragma once

#include <algorithm>

namespace Hikari
{

    // Largest representable floating point number less than 1.
    // Useful for clamping random numbers which shouldn't be greater than 1.
    static const double DOUBLE_ONE_MINUS_EPSILON = 0.99999999999999989;
    static const float FLOAT_ONE_MINUS_EPSILON = 0.99999994;

    #define PCG32_DEFAULT_STATE 0x853c49e6748fea9bULL
    #define PCG32_DEFAULT_STREAM 0xda3e39cb94b95bdbULL
    #define PCG32_MULT 0x5851f42d4c957f2dULL

    // Directly taken from PBRT A.1.2
    class RNG
    {
    public:
        RNG();
        RNG(uint64_t sequenceIndex) { SetSequence(sequenceIndex); }

        void SetSequence(uint64_t sequenceIndex);
        uint32_t UniformUInt32();
        uint32_t UniformUInt32(uint32_t b)
        {
            uint32_t threshold = (~b + 1u) % b;
            while (true)
            {
                uint32_t r = UniformUInt32();
                if (r >= threshold)
                    return r % b;
            }
        }

        float UniformFloat()
        {
            return std::min(FLOAT_ONE_MINUS_EPSILON, float(UniformUInt32() * 2.3283064365386963e-10f));
        }

        template <typename Iterator>
        void Shuffle(Iterator begin, Iterator end)
        {
            for (Iterator it = end - 1; it > begin; --it)
                std::iter_swap(it, begin + UniformUInt32((uint32_t)(it - begin + 1)));
        }

        void Advance(int64_t idelta)
        {
            uint64_t cur_mult = PCG32_MULT, cur_plus = m_Inc, acc_mult = 1u,
                acc_plus = 0u, delta = (uint64_t)idelta;

            while (delta > 0)
            {
                if (delta & 1)
                {
                    acc_mult *= cur_mult;
                    acc_plus = acc_plus * cur_mult + cur_plus;
                }
                cur_plus = (cur_mult + 1) * cur_plus;
                cur_mult *= cur_mult;
                delta /= 2;
            }
            m_State = acc_mult * m_State + acc_plus;
        }

        int64_t operator - (const RNG& other) const
        {
            uint64_t cur_mult = PCG32_MULT, cur_plus = m_Inc, cur_state = other.m_State,
                the_bit = 1u, distance = 0u;
            while (m_State != cur_state)
            {
                if ((m_State & the_bit) != (cur_state & the_bit))
                {
                    cur_state = cur_state * cur_mult + cur_plus;
                    distance |= the_bit;
                }
                the_bit <<= 1;
                cur_plus = (cur_mult + 1ULL) * cur_plus;
                cur_mult *= cur_mult;
            }
            return (int64_t)distance;
        }

    private:
        uint64_t m_State, m_Inc;
    };

    // RNG Inline Method Definitions
    inline RNG::RNG() : m_State(PCG32_DEFAULT_STATE), m_Inc(PCG32_DEFAULT_STREAM) {}

    inline void RNG::SetSequence(uint64_t initseq)
    {
        m_State = 0u;
        m_Inc = (initseq << 1u) | 1u;
        UniformUInt32();
        m_State += PCG32_DEFAULT_STATE;
        UniformUInt32();
    }

    inline uint32_t RNG::UniformUInt32()
    {
        uint64_t oldstate = m_State;
        m_State = oldstate * PCG32_MULT + m_Inc;
        uint32_t xorshifted = (uint32_t)(((oldstate >> 18u) ^ oldstate) >> 27u);
        uint32_t rot = (uint32_t)(oldstate >> 59u);
        return (xorshifted >> rot) | (xorshifted << ((~rot + 1u) & 31));
    }

}   // namespace Hikari