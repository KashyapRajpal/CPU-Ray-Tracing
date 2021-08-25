//
//  Material.h
//  Raytracing
//
//  Created by Kashyap Rajpal on 4/26/20.
//  Copyright © 2020 Kashyap Rajpal. All rights reserved.
//

#ifndef Material_h
#define Material_h

//#include "Hittable.h"
#include "Vector.h"
#include "Ray.h"

struct HitRecord;

class Material
{
public:
    
    virtual bool Scatter(const Ray& ray_in, const HitRecord& hitRec, Vector3& attenuation, Ray& scatteredRay_out) const = 0;
    
    inline const double SchlickApproximation(double cosine, double ref_idx) const
    {
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

class Lambertian : public Material {
    public:
        Lambertian(const Vector3& a) : m_albedo(a) {}

        virtual bool Scatter
        (
            const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered_out
         ) const;

    private:
        Vector3 m_albedo;
};

class Metal : public Material
{
public:
    Metal(const Vector3& a, double fuzz) : m_albedo(a), m_fuzz(fuzz) {}

    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered_out) const;
        
private:
    Vector3 m_albedo;
    double m_fuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(double ri) : m_refractiveIndex(ri) {}

    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation,  Ray& scattered) const;
private:
        double m_refractiveIndex;
};


#endif /* Material_h */
