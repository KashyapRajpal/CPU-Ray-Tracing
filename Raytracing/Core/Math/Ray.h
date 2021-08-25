//
//  Ray.h
//  Raytracing
//
//  Created by Kashyap Rajpal on 2/17/20.
//  Copyright © 2020 Kashyap Rajpal. All rights reserved.
//

#ifndef Ray_h
#define Ray_h

#include "Vector.h"

class Ray
{
public:
        Ray() {}
        Ray(const Vector3& origin, const Vector3& direction) { m_origin = origin; m_direction = direction; }
        Vector3 GetOrigin() const       { return m_origin; }
        Vector3 GetDirection() const    { return m_direction; }
        Vector3 PointAtParameter(float t) const { return m_origin + t * m_direction; }
        
        

private:
        Vector3 m_origin;
        Vector3 m_direction;
};

#endif /* Ray_h */
