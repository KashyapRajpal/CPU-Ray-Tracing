//
//  Camera.h
//  Raytracing
//
//  Created by Kashyap Rajpal on 3/26/20.
//  Copyright © 2020 Kashyap Rajpal. All rights reserved.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "../Math/Ray.h"

class Camera {

public:
    Camera(
            Vector3 lookfrom, Vector3 lookat, Vector3 vup,
            double vfov, // top to bottom, in degrees
            double aspect,
            double aperture,
            double focusDistance
          )
    {
        m_origin = lookfrom;
        m_lensRadius = aperture / 2;
        
        auto theta = DegreesToRadian(vfov);
        auto half_height = tan(theta/2);
        auto half_width = aspect * half_height;
        
        m_w = unit_vector(lookfrom - lookat);
        m_u = unit_vector(cross(vup, m_w));
        m_v = cross(m_w, m_u);

        m_lowerLeftCorner = m_origin - half_width * m_u
        * focusDistance - half_height * m_v * focusDistance - m_w * focusDistance;

        m_horizontal = 2 * half_width * m_u * focusDistance;
        m_vertical = 2 * half_height * m_v * focusDistance;
    }
    
    Camera()
    {
            m_origin = Vector3(0.0, 0.0, 0.0);
            m_lowerLeftCorner = Vector3(-2.0, -1.0, -1.0);
            m_horizontal = Vector3(4.0, 0.0, 0.0);
            m_vertical = Vector3(0.0, 2.0, 0.0);
    }

    Ray GetRay(double u, double v)
    {
        Vector3 randomPointInDisk = m_lensRadius * Vector3::RandomInUnitDisk();
        Vector3 offset = m_u * randomPointInDisk.X() + m_v * randomPointInDisk.Y();
        
        return Ray(m_origin + offset, m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin - offset);
    }

private:
        double m_lensRadius;
        Vector3 m_origin;
        Vector3 m_lowerLeftCorner;
        Vector3 m_horizontal;
        Vector3 m_vertical;
        Vector3 m_u;
        Vector3 m_v;
        Vector3 m_w;
};
#endif
