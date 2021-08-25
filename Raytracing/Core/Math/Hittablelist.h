//
//  Hittablelist.h
//  Raytracing
//
//  Created by Kashyap Rajpal on 3/3/20.
//  Copyright © 2020 Kashyap Rajpal. All rights reserved.
//

#ifndef Hittablelist_h
#define Hittablelist_h

#include "Hittable.h"
#include <memory>
#include <vector>

class HittableList : public Hittable
{
public:
    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { AddHittable(object); }
    
    void ClearList() { m_hittableObjectList.clear(); }
    void AddHittable(shared_ptr<Hittable> object)
    { m_hittableObjectList.push_back(object); }
    
    virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
    
private:
    std::vector<shared_ptr<Hittable>> m_hittableObjectList;
        
};

bool HittableList::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
    HitRecord hitRec;
    bool bHitAnything = false;
    double closest_so_far = t_max;
    for (auto itr = m_hittableObjectList.begin(); itr != m_hittableObjectList.end(); ++itr)
    {
        if ((*itr)->hit(r, t_min, closest_so_far, hitRec))
        {
            bHitAnything = true;
            closest_so_far = hitRec.m_t;
            rec = hitRec;
        }
    }
    return bHitAnything;
}

#endif /* Hittablelist_h */
