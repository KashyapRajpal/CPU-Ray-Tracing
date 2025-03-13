//
//  Hittable.h
//  Raytracing
//
//  Created by Kashyap Rajpal on 3/2/20.
//  Copyright © 2020 Kashyap Rajpal. All rights reserved.
//

#ifndef Hittable_h
#define Hittable_h

#include "Utils.h"
#include "Ray.h"
#include "HitRecord.h"


class Hittable
{
    public:
    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};

#endif /* Hittable_h */
