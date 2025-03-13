//
//  Material.h
//  Raytracing
//
//  Created by Kashyap Rajpal on 4/26/20.
//  Copyright © 2020 Kashyap Rajpal. All rights reserved.
//

#ifndef Material_h
#define Material_h

#include "HitRecord.h"
#include "Vector.h"
#include "Ray.h"

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
         ) const {
    Vector3 scatter_direction = rec.m_normal + Vector3::GetRandomUnitVector();
    scattered_out = Ray(rec.m_point, scatter_direction);
    attenuation = m_albedo;
    return true;
}


    private:
        Vector3 m_albedo;
};

class Metal : public Material
{
public:
    Metal(const Vector3& a, double fuzz) : m_albedo(a), m_fuzz(fuzz) {}

    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered_out) const {
    Vector3 reflected = Vector3::Reflect(unit_vector(r_in.GetDirection()), rec.m_normal);
    scattered_out = Ray(rec.m_point, reflected + m_fuzz * Vector3::RandomInUnitSphere());
    attenuation = m_albedo;
    return (dot(scattered_out.GetDirection(), rec.m_normal) > 0);
}
        
private:
    Vector3 m_albedo;
    double m_fuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(double ri) : m_refractiveIndex(ri) {}

    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation,  Ray& scattered) const{
    attenuation = Vector3(1.0, 1.0, 1.0);
    double etai_over_etat;
    if (rec.m_frontFace)
    {
        etai_over_etat = 1.0 / m_refractiveIndex;
    }
    else
    {
        etai_over_etat = m_refractiveIndex;
    }
    
    Vector3 unit_direction = unit_vector(r_in.GetDirection());
    
    double cos_theta = ffmin(dot(-unit_direction, rec.m_normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    if (etai_over_etat * sin_theta > 1.0 )
    {
        Vector3 reflected = Vector3::Reflect(unit_direction, rec.m_normal);
        scattered = Ray(rec.m_point, reflected);
        return true;
    }
    
    double reflect_prob = SchlickApproximation(cos_theta, etai_over_etat);
    if (RandomDouble() < reflect_prob)
    {
        Vector3 reflected = Vector3::Reflect(unit_direction, rec.m_normal);
        scattered = Ray(rec.m_point, reflected);
        return true;
    }
    
    Vector3 refracted = Vector3::Refract(unit_direction, rec.m_normal, etai_over_etat);
    scattered = Ray(rec.m_point, refracted);
    return true;
}

private:
        double m_refractiveIndex;
};


#endif /* Material_h */
