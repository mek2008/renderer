#pragma once
#include "perspNmath.hpp"
#include <memory>
#include <limits>
#include <vector>

using std::vector;

class Object{public:
virtual locPixelv hitRR(const Ray& ray) const = 0;
virtual ~Object() = default;
};



class LightSource{public:
    Vec3 cordinates;
    unsigned char brightness;
    Pixel color;

LightSource(Vec3 cordinates, unsigned char brightness, Pixel color);
};


class Sphere : public Object { public:
Vec3 location;
float radius; 
Pixel color;
Sphere(Vec3 location, float radius, Pixel color);

locPixelv hitRR(const Ray& ray) const override;
};


class Triangle : public Object { public:
Vec3 a, b, c;
Vec3 e, f;
Vec3 normal;
Pixel color;
Triangle(Vec3 a, Vec3 b, Vec3 c, Pixel color);

locPixelv hitRR(const Ray& ray) const override;
};


class Tetrahedron : public Object {public:
Triangle alpha, beta, gamma, delta;
Vec3 a, b, c, d;
Pixel color;
Tetrahedron(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Pixel color);

locPixelv hitRR(const Ray& ray) const override;
};



Pixel postLighting(const LightSource &light, const locPixelv &point, float ambient );

struct RelevantObjects{vector<Object> rObj;};
struct RelevantLightSources{vector<LightSource> rLight;};

/*
Pixel shadowImplementationnocap(
    const std::vector<std::unique_ptr<Object>> &scene,
    const locPixelv &colCord,
    const std::vector<std::unique_ptr<LightSource>> &lights
);
*/

Pixel shadowImplementation(
    const std::vector<std::unique_ptr<Object>> &scene,
    const locPixelv &colCord,
    const std::vector<std::unique_ptr<LightSource>> &lights,
    float ambient
);
