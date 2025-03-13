//
//  main.cpp
//  Raytracing
//
//  Created by Kashyap Rajpal on 2/10/20.
//  Copyright © 2020 Kashyap Rajpal. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string.h>
#include "Core/Math/Material.h"
#include "Core/Shape/Sphere.h"
#include "Core/Math/HitRecord.h"
#include "Core/Math/Hittablelist.h"
#include "Core/Camera/Camera.h"
#include "System/ThreadPool.h"

using namespace std;

#define ASSETS_PATH "/Users/kashyaprajpal/Desktop/Personal Projects/CPU-Ray-Tracing/Raytracing/Assets/"
#define DIFFUSE_IN_HEMISPHERE 0
#define USE_MULTITHREADED_SYSTEM 1
std::mutex main_mutex;

float HitSpehere(const Vector3& center, float radius, const Ray& r)
{
    Vector3 oc = r.GetOrigin() - center;
    float a = dot(r.GetDirection(), r.GetDirection());
    float b = 2.0 * dot(oc, r.GetDirection());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-b - sqrt(discriminant) ) / (2.0*a);
    }
}

Vector3 GetColor(const Ray& r, const Hittable& world, int depth)
{
    if (depth <= 0)
    {
        return Vector3(0.f, 0.f, 0.f);
    }
    
    HitRecord hitRec;
    if (world.hit(r, 0.001, s_kInfinity, hitRec))
    {
#if DIFFUSE_IN_HEMISHPHERE
        Vector3 target = hitRec.m_point + RandomInHemiSphere(hitRec.m_normal);
#else
        Ray scattered;
        Vector3 attenuation = Vector3::GetZero();
        if (hitRec.m_material->Scatter(r, hitRec, attenuation, scattered))
            return attenuation * GetColor(scattered, world, depth - 1);
        return attenuation;
#endif
    }
    
    Vector3 unit_direction = unit_vector(r.GetDirection());
    float t = 0.5 * (unit_direction.Y() + 1.0);
    return (1.0 - t) * Vector3(1.0, 1.0, 1.0) + t * Vector3(0.5, 0.7, 1.0);
}

HittableList GetScene()
{
    HittableList scene;
    
    // Add Background sphere
    scene.AddHittable(make_shared<Sphere>(
    Vector3(0,-1000,0), 1000, make_shared<Lambertian>(Vector3(0.5, 0.5, 0.5))));
    
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = RandomDouble();
            Vector3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());
            if ((center - Vector3(4, 0.2, 0)).Length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Vector3::Random() * Vector3::Random();
                    scene.AddHittable(
                        make_shared<Sphere>(center, 0.2, make_shared<Lambertian>(albedo)));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = Vector3::Random(.5, 1);
                    auto fuzz = RandomDouble(0, .5);
                    scene.AddHittable(
                        make_shared<Sphere>(center, 0.2, make_shared<Metal>(albedo, fuzz)));
                }
                else
                {
                    // glass
                    scene.AddHittable(make_shared<Sphere>(center, 0.2, make_shared<Dielectric>(1.5)));
                }
            }
        }
    }
    
    scene.AddHittable(make_shared<Sphere>(Vector3(0, 1, 0), 1.0, make_shared<Dielectric>(1.5)));

    scene.AddHittable(
        make_shared<Sphere>(Vector3(-4, 1, 0), 1.0, make_shared<Lambertian>(Vector3(0.4, 0.2, 0.1))));

    scene.AddHittable(
        make_shared<Sphere>(Vector3(4, 1, 0), 1.0, make_shared<Metal>(Vector3(0.7, 0.6, 0.5), 0.0)));

    
    return scene;
}

const auto aspect_ratio = 3.0 / 2.0;
const int image_width = 780;
const int image_height = static_cast<int>(image_width / aspect_ratio);
const int samplesPerPixel = 100;
const int kMaxDepth = 50;

Vector3 pixelColor[image_width][image_height];

void ComputeColor(const int i, const int j, const HittableList& world, Camera& camera)
{
    Vector3 color(0, 0, 0);
    for (int s = 0; s < samplesPerPixel; ++s)
    {
        auto u = (i + RandomDouble()) / image_width;
        auto v = (j + RandomDouble()) / image_height;
        Ray r = camera.GetRay(u, v);
        color += GetColor(r, world, kMaxDepth);
    }
    
    
    {
        std::unique_lock<std::mutex> lock(main_mutex);
        pixelColor[i][j] = color;
    }
}

int main(int argc, const char * argv[])
{
    auto startTime = std::chrono::high_resolution_clock::now();
    
    std::ofstream outputImage;
    string imageFileName(ASSETS_PATH);
#if USE_MULTITHREADED_SYSTEM
    imageFileName += "Basic_Image_6.ppm";
#else
    imageFileName += "Basic_Image_4.ppm";
#endif
    //cout << imageFileName << endl;
    outputImage.open(imageFileName.c_str());
    
    HittableList world = GetScene();
    
    // Lambertian Spheres
    world.AddHittable(make_shared<Sphere>(Vector3(0.f, 0.f, -1.f), 0.5, make_shared<Lambertian>(Vector3(0.1, 0.2, 0.5))));
    
    world.AddHittable(make_shared<Sphere>(Vector3(0.f, -100.5, -1.f), 100, make_shared<Lambertian>(Vector3(0.8, 0.8, 0.0))));
    
    // Metal Spheres
    world.AddHittable(make_shared<Sphere>(Vector3(1, 0, -1), 0.5, make_shared<Metal>(Vector3(0.8, 0.6, 0.2), RandomDouble())));
    world.AddHittable(make_shared<Sphere>(Vector3(-1,0,-1), 0.5, make_shared<Metal>(Vector3(0.8, 0.8, 0.8), RandomDouble())));
    
    world.AddHittable(make_shared<Sphere>(Vector3(1, 0, -1), 0.5, make_shared<Metal>(Vector3(0.8, 0.6, 0.2), 0.3)));
    
    // Dielectric
    world.AddHittable(make_shared<Sphere>(Vector3(-1, 0, -1), 0.5, make_shared<Dielectric>(1.5)));
    
    world.AddHittable(make_shared<Sphere>(Vector3(-1, 0, -1), -0.45, make_shared<Dielectric>(1.5)));
    
    Vector3 lookfrom(13, 2, 3);
    Vector3 lookat(0, 0, 0);
    Vector3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    const auto aspect_ratio = double(image_width) / image_height;
    
    Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    
    outputImage << "P3\n" << image_width << " " << image_height << "\n255\n";
    std::string pixelColorString;
    
    cout << "Creating image " << imageFileName << endl;
#if USE_MULTITHREADED_SYSTEM
    ThreadPool threadPool;
#endif
    for (int j = image_height - 1; j > -1; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
#if USE_MULTITHREADED_SYSTEM
            threadPool.QueueJob(std::bind(&ComputeColor, i, j, world, camera));
#else
            Vector3 color(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s)
            {
                auto u = (i + RandomDouble()) / image_width;
                auto v = (j + RandomDouble()) / image_height;
                Ray r = camera.GetRay(u, v);
                color += GetColor(r, world, kMaxDepth);
            }
            color.WriteColor(pixelColorString, samplesPerPixel);
#endif
        }
    }
    
#if USE_MULTITHREADED_SYSTEM
    threadPool.WaitUntilDone();
    
    //All Threads are done. Colate results.
    {
        std::unique_lock<std::mutex> mut(main_mutex);
        for (int j = image_height-1; j >= 0; --j)
        {
            for (int i = 0; i < image_width; ++i)
            {
                pixelColor[i][j].WriteColor(pixelColorString, samplesPerPixel);
            }
        }
    }
#endif
    outputImage << pixelColorString;
    
    outputImage.close();
    
    auto endTime = std::chrono::high_resolution_clock::now();
    
    std::cout << "Done. Execution Time: " << std::chrono::duration<double, std::milli>(endTime - startTime).count() / 60000.f << " min" << std::endl;
}
