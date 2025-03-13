//
//  Sphere.h
//  Raytracing
//
//  Created by Kashyap Rajpal on 3/3/20.
//  Copyright © 2020 Kashyap Rajpal. All rights reserved.
//

#ifndef Sphere_h
#define Sphere_h
#include "../Math/Hittable.h"

class Sphere : public Hittable
{
public:
    Sphere(){}
    Sphere(Vector3 center, float radius, shared_ptr<Material> mat):
    m_center(center), m_radius(radius), m_material(mat) {}
    
    virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
    const Vector3& GetCenter() const { return m_center; }
    const float GetRadius() const { return m_radius; }
    
private:
    float   m_radius;
    Vector3 m_center;
    shared_ptr<Material> m_material;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
    Vector3 oc = r.GetOrigin() - GetCenter();
    float a = dot(r.GetDirection(), r.GetDirection());
    float b = dot(oc, r.GetDirection());
    float c = dot(oc, oc) - GetRadius() * GetRadius();
    float discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        float temp = (-b - sqrt(discriminant))/a;
        if (temp < t_max && temp > t_min)
        {
            rec.m_t = temp;
            rec.m_point = r.PointAtParameter(rec.m_t);
            
            Vector3 outwardNormal = (rec.m_point - GetCenter()) / GetRadius();
            rec.SetFaceNormal(r, outwardNormal);
            rec.m_material = m_material;
            return true;
        }
        
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.m_t = temp;
            rec.m_point = r.PointAtParameter(rec.m_t);
            Vector3 outwardNormal = (rec.m_point - GetCenter()) / GetRadius();
            rec.SetFaceNormal(r, outwardNormal);
            rec.m_material = m_material;
            return true;
        }
    }
    return false;
}

#endif /* Sphere_h */
