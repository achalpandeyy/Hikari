#pragma once

#include <cmath>

namespace Hikari
{
    class RGBSpectrum;

    // Our computation model of SPDs i.e. `*Spectrum` classes
    // will only sample the SPDs in the range specified below.
    // This is set to [400nm, 700nm] - the visible wavelengths.
    static const float SAMPLED_WAVELENGTH_START = 400u;
    static const float SAMPLED_WAVELENGTH_END = 700u;
    static const unsigned int NUM_SAMPLES = 60u;

    // Spectral Data Declarations
    static const unsigned int CIE_NumSamples = 471u;
    extern const float CIE_X[CIE_NumSamples];
    extern const float CIE_Y[CIE_NumSamples];
    extern const float CIE_Z[CIE_NumSamples];
    extern const float CIE_wavelengths[CIE_NumSamples];
    static const float CIE_YIntegral = 106.856895f;

    // For RGB to SPD conversion using Smits (1999)
    // http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.40.9608&rep=rep1&type=pdf
    // Type: Reflectance
    static const unsigned int numRGB2SpectSamples = 32u;
    extern const float RGB2SpectLambda[numRGB2SpectSamples];
    extern const float RGBRefl2SpectWhite[numRGB2SpectSamples];
    extern const float RGBRefl2SpectCyan[numRGB2SpectSamples];
    extern const float RGBRefl2SpectMagenta[numRGB2SpectSamples];
    extern const float RGBRefl2SpectYellow[numRGB2SpectSamples];
    extern const float RGBRefl2SpectRed[numRGB2SpectSamples];
    extern const float RGBRefl2SpectGreen[numRGB2SpectSamples];
    extern const float RGBRefl2SpectBlue[numRGB2SpectSamples];

    // For RGB to SPD conversion based on Smits (1999)
    // Type: Illumination
    extern const float RGBIllum2SpectWhite[numRGB2SpectSamples];
    extern const float RGBIllum2SpectCyan[numRGB2SpectSamples];
    extern const float RGBIllum2SpectMagenta[numRGB2SpectSamples];
    extern const float RGBIllum2SpectYellow[numRGB2SpectSamples];
    extern const float RGBIllum2SpectRed[numRGB2SpectSamples];
    extern const float RGBIllum2SpectGreen[numRGB2SpectSamples];
    extern const float RGBIllum2SpectBlue[numRGB2SpectSamples];

    enum class SpectrumType
    {
        Reflectance,
        Illuminant
    };

    inline void XYZToRGB(const float xyz[3], float rgb[3])
    {
        rgb[0] = 3.240479f * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
        rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
        rgb[2] = 0.055648f * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];
    }

    inline void RGBToXYZ(const float rgb[3], float xyz[3])
    {
        xyz[0] = 0.412453f * rgb[0] + 0.357580f * rgb[1] + 0.180423f * rgb[2];
        xyz[1] = 0.212671f * rgb[0] + 0.715160f * rgb[1] + 0.072169f * rgb[2];
        xyz[2] = 0.019334f * rgb[0] + 0.119193f * rgb[1] + 0.950227f * rgb[2];
    }

    bool SpectrumSamplesSorted(
        const float*    wavelengths,
        const float*    values,
        unsigned int    count);

    void SortSpectrumSamples(float* wavelengths, float* values, unsigned int count);

    float AverageSpectrumSamples(
        const float*    wavelengths,
        const float*    values,
        unsigned int    count,
        float           wavLow,
        float           wavHigh);

    template <unsigned int numSpectrumSamples>
    class CoefficientSpectrum
    {
    public:
        CoefficientSpectrum(float value = 0.f)
        {
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                m_Coeffs[i] = 0.f;
        }

        // TODO(achal): Implement a method (and macro) to Log if the incoming
        // CoefficientSpectrum `s2` has NaNs, for the following methods.
        CoefficientSpectrum operator + (const CoefficientSpectrum& s2) const
        {
            // Log(!s.HasNaNs());
            CoefficientSpectrum result = *this;
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                result.m_Coeffs += s2.m_Coeffs[i];
            return result;
        }

        CoefficientSpectrum operator - (const CoefficientSpectrum& s2) const
        {
            // Log(!s.HasNaNs());
            CoefficientSpectrum result = *this;
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                result.m_Coeffs[i] -= s2.m_Coeffs[i];
            return result;
        }

        CoefficientSpectrum operator - () const
        {
            CoefficientSpectrum ret;
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                ret.m_Coeffs[i] = -m_Coeffs[i];
            return ret;
        }

        CoefficientSpectrum operator * (const CoefficientSpectrum& sp) const
        {
            // Log(!s.HasNaNs());
            CoefficientSpectrum result = *this;
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                result.m_Coeffs[i] *= sp.m_Coeffs[i];
            return result;
        }

        CoefficientSpectrum operator / (const CoefficientSpectrum& s2) const
        {
            // Log(!s.HasNaNs());
            CoefficientSpectrum result = *this;
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
            {
                // TODO(achal): Implement a macro to check if the incoming
                // CoefficientSpectrum `s2` is zero.
                // LogNotEquals(s2.c[i], 0);
                result.m_Coeffs[i] /= s2.m_Coeffs[i];
            }
            return result;
        }

        CoefficientSpectrum& operator += (const CoefficientSpectrum& s2)
        {
            // Log(!s.HasNaNs());
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                m_Coeffs[i] += s2.m_Coeffs[i];
            return *this;
        }

        CoefficientSpectrum& operator *= (const CoefficientSpectrum& sp)
        {
            // Log(!s.HasNaNs());
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                m_Coeffs[i] *= sp.m_Coeffs[i];
            return *this;
        }

        CoefficientSpectrum operator * (float a) const
        {
            CoefficientSpectrum result = *this;
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                result.m_Coeffs[i] *= a;

            // Log(!s.HasNaNs());
            return result;
        }

        CoefficientSpectrum operator / (float a) const
        {
            // LogNotEquals(a, 0);
            // Log(!std::isnan(a));

            CoefficientSpectrum ret = *this;
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                ret.m_Coeffs[i] /= a;

            // Log(!s.HasNaNs());
            return ret;
        }

        CoefficientSpectrum& operator *= (float a)
        {
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                m_Coeffs[i] *= a;

            // Log(!s.HasNaNs());
            return *this;
        }

        CoefficientSpectrum& operator /= (float a)
        {
            // LogNotEquals(a, 0);
            // Log(!std::isnan(a));
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                m_Coeffs[i] /= a;
            return *this;
        }

        bool operator == (const CoefficientSpectrum& sp) const
        {
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
            {
                if (m_Coeffs[i] != sp.m_Coeffs[i])
                    return false;
            }
            return true;
        }

        bool operator != (const CoefficientSpectrum& sp) const
        {
            return !(*this == sp);
        }

        float& operator [] (size_t i)
        {
            // DCHECK(i >= 0 && i < numSpectrumSamples);
            return m_Coeffs[i];
        }

        float operator [] (size_t i) const
        {
            // DCHECK(i >= 0 && i < numSpectrumSamples);
            return m_Coeffs[i];
        }

        bool IsBlack() const
        {
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
            {
                if (m_Coeffs[i] != 0.f)
                    return false;
            }
            return true;
        }

        CoefficientSpectrum Clamp(float low = 0, float high = INFINITY) const
        {
            CoefficientSpectrum ret;
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                ret.m_Coeffs[i] = glm::clamp(m_Coeffs[i], low, high);

            // DCHECK(!ret.HasNaNs());
            return ret;
        }

        float MaxComponentValue() const
        {
            float max = m_Coeffs[0];
            for (size_t i = 1u; i < numSpectrumSamples; ++i)
                max = std::max(max, m_Coeffs[i]);
            return max;
        }

        bool HasNaNs() const
        {
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
            {
                if (std::isnan(m_Coeffs[i]))
                    return true;
            }
            return false;
        }

        // Friends
        friend inline CoefficientSpectrum operator * (float a, const CoefficientSpectrum& s)
        {
            // DCHECK(!std::isnan(a) && !s.HasNaNs());
            return s * a;
        }

        friend CoefficientSpectrum Sqrt(const CoefficientSpectrum& s)
        {
            CoefficientSpectrum ret;
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                ret.m_Coeffs[i] = std::sqrt(s.m_Coeffs[i]);

            // DCHECK(!ret.HasNaNs());
            return ret;
        }

        template <unsigned int n>
        friend inline CoefficientSpectrum<n> Pow(const CoefficientSpectrum<n>& s, float e);

        friend CoefficientSpectrum Exp(const CoefficientSpectrum& s)
        {
            CoefficientSpectrum ret;
            for (size_t i = 0u; i < numSpectrumSamples; ++i)
                ret.m_Coeffs[i] = std::exp(s.c[i]);

            // DCHECK(!ret.HasNaNs());
            return ret;
        }

        static const int s_NumSamples = numSpectrumSamples;

    protected:
        float m_Coeffs[numSpectrumSamples];
    };

    // This class might not be used for now.
    class SampledSpectrum : public CoefficientSpectrum<NUM_SAMPLES>
    {
    public:
        SampledSpectrum(float value = 0.f) : CoefficientSpectrum<s_NumSamples>(value) {}
        SampledSpectrum(const CoefficientSpectrum<s_NumSamples>& s) : CoefficientSpectrum<s_NumSamples>(s) {}
        SampledSpectrum(const RGBSpectrum& r, SpectrumType type);

        static SampledSpectrum FromSampledSPD(
            const float*    wavelengths,
            const float*    values,
            unsigned int    count);

        static SampledSpectrum FromRGB(const float rgb[3], SpectrumType type);
        static SampledSpectrum FromXYZ(const float xyz[3], SpectrumType type = SpectrumType::Reflectance);

        static void Init();

        void ToXYZ(float xyz[3]) const;
        float y() const;
        void ToRGB(float rgb[3]) const;
        RGBSpectrum ToRGBSpectrum() const;

    private:
        // X, Y, Z matching curves in spectral representation
        static SampledSpectrum s_X, s_Y, s_Z;

        // Smits (1999) Reflectance SPDs
        static SampledSpectrum s_RgbRefl2SpectWhite, s_RgbRefl2SpectCyan,
            s_RgbRefl2SpectMagenta, s_RgbRefl2SpectYellow, s_RgbRefl2SpectRed,
            s_RgbRefl2SpectGreen, s_RgbRefl2SpectBlue;

        // Smits (1999) Illumination SPDs
        static SampledSpectrum s_RgbIllum2SpectWhite, s_RgbIllum2SpectCyan,
            s_RgbIllum2SpectMagenta, s_RgbIllum2SpectYellow, s_RgbIllum2SpectRed,
            s_RgbIllum2SpectGreen, s_RgbIllum2SpectBlue;
    };

    class RGBSpectrum : public CoefficientSpectrum<3u>
    {
    public:
        RGBSpectrum(float value = 0.f) : CoefficientSpectrum<3u>(value) {}
        RGBSpectrum(const CoefficientSpectrum<3u>& s) : CoefficientSpectrum<3u>(s) {}

        static RGBSpectrum FromRGB(const float rgb[3], SpectrumType type = SpectrumType::Reflectance);
        static RGBSpectrum FromSampledSPD(const float* wavelengths, const float* values, unsigned int count);

        void ToRGB(float rgb[3]) const;
        const RGBSpectrum& ToRGBSpectrum() const { return *this; }
        void ToXYZ(float xyz[3]) const { RGBToXYZ(m_Coeffs, xyz); }

        static RGBSpectrum FromXYZ(const float xyz[3], SpectrumType type = SpectrumType::Reflectance);

        float y() const;
    };

}   // namespace Hikari
