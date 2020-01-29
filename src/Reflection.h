#pragma once

#include "Spectrum.h"

#include <glm/glm.hpp>

#include <algorithm>
#include <cstdlib>

namespace Hikari
{

    inline float CosTheta(const glm::vec3& w) { return w.y; }
    inline float CosSqTheta(const glm::vec3& w) { return w.y * w.y; }
    inline float AbsCosTheta(const glm::vec3& w) { return std::abs(w.y); }
    inline float SinSqTheta(const glm::vec3& w)
    {
        return std::max(0.f, 1.f - CosSqTheta(w));
    }
    inline float SinTheta(const glm::vec3& w)
    {
        return std::sqrt(SinSqTheta(w));
    }
    inline float TanTheta(const glm::vec3& w)
    {
        return SinTheta(w) / CosTheta(w);
    }
    inline float TanSqTheta(const glm::vec3& w)
    {
        return SinSqTheta(w) / CosSqTheta(w);
    }

    inline float CosPhi(const glm::vec3& w)
    {
        float sinTheta = SinTheta(w);
        return sinTheta == 0.f ? 1.f : glm::clamp(w.x / sinTheta, -1.f, 1.f);
    }
    inline float CosSqPhi(const glm::vec3& w)
    {
        return CosPhi(w) * CosPhi(w);
    }
    inline float SinPhi(const glm::vec3& w)
    {
        float sinTheta = SinTheta(w);
        return sinTheta == 0.f ? 1.f : glm::clamp(w.z / sinTheta, -1.f, 1.f);
    }
    inline float SinSqPhi(const glm::vec3& w)
    {
        return SinPhi(w) * SinPhi(w);
    }
    inline float CosDPhi(const glm::vec3& wa, const glm::vec3& wb)
    {
        return glm::clamp((wa.x * wb.x + wa.y * wb.y) /
            std::sqrt((wa.x * wa.x + wa.y * wa.y) * (wb.x * wb.x + wb.y * wb.y)),
            -1.f, 1.f);
    }

    class BxDF
    {
    public:
        enum BxDFType
        {
            BSDF_REFLECTIVE = 1 << 0,
            BSDF_TRANSMISSIVE = 1 << 1,

            BSDF_DIFFUSE = 1 << 2,
            BSDF_SPECULAR = 1 << 3,
            BSDF_GLOSSY = 1 << 4,

            BSDF_ALL = BSDF_REFLECTIVE | BSDF_TRANSMISSIVE |
            BSDF_DIFFUSE | BSDF_SPECULAR | BSDF_GLOSSY
        };

        BxDF(BxDFType type) : m_Type(type) {}

        bool IsType(BxDFType type) const
        {
            return (type & m_Type) == type;
        }

        // Here we assume the that wavelengths of light are completely independent,
        // which means that, for example, a wavelength lamda1 after reflection would
        // not contribute to any other wavelength (say) lambda2.
        // This also means that we won't be able to simulate fluroscent materials
        // where this assumption is not true.
        virtual RGBSpectrum f(const glm::vec3& wo, const glm::vec3& wi) const = 0;

        // For some materials, for example specular materials, it can be hard
        // to predict the incoming direction of light, hence the requirement of this
        // method which computes both `wi` and function's value at this (wo, wi)
        virtual RGBSpectrum Sample_f(const glm::vec3& wo, glm::vec3& wi,
            const glm::vec2& sample, float& pdf, BxDFType* sampledType = nullptr) const;

        // Hemispherical-directional reflectance rho_hd
        virtual RGBSpectrum Rho(const glm::vec3& wo, int numSamples,
            const glm::vec2* samples) const;

        // Hemispherical-hemispherical reflectance rho_hh
        virtual RGBSpectrum Rho(int numSamples, const glm::vec2* samples1,
            const glm::vec2* samples2) const;

        const BxDFType m_Type;
    };

    /*
    *   `BxDF`s scaled by a `Spectrum`
    */
    class ScaledBxDF : public BxDF
    {
    public:
        ScaledBxDF(BxDF* bxdf, const RGBSpectrum& scale)
            : BxDF(BxDFType(bxdf->m_Type)), m_BxDF(bxdf), m_Scale(scale)
        {}

        RGBSpectrum f(const glm::vec3& wo, const glm::vec3& wi) const override
        {
            return m_Scale * m_BxDF->f(wo, wi);
        }

        RGBSpectrum Sample_f(const glm::vec3& wo, glm::vec3& wi,
            const glm::vec2& sample, float& pdf, BxDFType* sampledType = nullptr) const override
        {
            return m_Scale * m_BxDF->Sample_f(wo, wi, sample, pdf, sampledType);
        }

        virtual RGBSpectrum Rho(const glm::vec3& wo, int numSamples,
            const glm::vec2* samples) const override
        {
            return m_Scale * m_BxDF->Rho(wo, numSamples, samples);
        }

        virtual RGBSpectrum Rho(int numSamples, const glm::vec2* samples1,
            const glm::vec2* samples2) const override
        {
            return m_Scale * m_BxDF->Rho(numSamples, samples1, samples2);
        }

    private:
        BxDF* m_BxDF;
        RGBSpectrum m_Scale;
    };

}   // namespace Hikari