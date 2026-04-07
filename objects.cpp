#include <algorithm>
#include "objects.hpp"
#include "perspNmath.hpp"
#include <cmath>
#include <memory>



LightSource::LightSource(Vec3 cordinates, unsigned char brightness, Pixel color)   :   cordinates(cordinates), brightness(brightness), color(color)   {}


Sphere::Sphere(Vec3 location, float radius, Pixel color)    :    location(location), radius(radius), color(color)    {}

locPixelv Sphere::hitRR(const Ray& ray)  const  {
//t = [−(a·b) ± sqrt((a·b)² − (b·b)((a·a) − r²))] / (b·b)
Vec3 oc = subtract3v(ray.origin, location);
float a = dotProduct3v(ray.direction, ray.direction );
float b = 2.0f * (dotProduct3v(oc, ray.direction)) ;
float c = dotProduct3v(oc, oc) - (radius * radius);

float discrimenant = b*b - 4.0f*a*c;

if (discrimenant <= 0.000001f) {
return {{0,0,0,0}, {0,0,0}, {0,0,0}, -1};
}

float t1 = (-b - sqrt(discrimenant)) / (2.0f * a);
float t2 = (-b + sqrt(discrimenant)) / (2.0f * a);

Vec3 pointCordinates = add3v(ray.origin, scelar3v(ray.direction, std::min(t1, t2)));
Vec3 normal = normalize3v(subtract3v(pointCordinates, location));


return {color, pointCordinates, normal, std::min(t1, t2)}; //color, cordinates, t
}




Triangle::Triangle(Vec3 a, Vec3 b, Vec3 c, Pixel color)   :   a(a), b(b), c(c), color(color) {
        e = subtract3v(b, a);
        f = subtract3v(c, a);
        normal = normalize3v(crossProduct3v(e, f));
}

locPixelv Triangle::hitRR(const Ray& ray)  const  {
//Vec3 e = subtract3v(b, a); optimization
//Vec3 f = subtract3v(c, a); optimization
//Vec3 h = cross(ray.direction, e2);
//float det = dot(e1, h);
Vec3 q = crossProduct3v(e, ray.direction);
float D = dotProduct3v(f, q);
float invD = 1 / D;
if (fabs(D) < 0.00001f) return {{0,0,0,0}, {0,0,0}, {0,0,0}, -1};

Vec3 B = subtract3v(ray.origin, a);

float v = (dotProduct3v(B, q)) * invD; 
if (v < 0.0f || v > 1.0f) return {{0,0,0,0}, {0,0,0}, {0,0,0}, -1};

Vec3 p = crossProduct3v(f, B);

float u = (dotProduct3v(ray.direction, p)) * invD;
if (u < 0.0f || u + v > 1.0f) return {{0,0,0,0}, {0,0,0}, {0,0,0}, -1};

float t = (dotProduct3v(e, p)) * invD;
if(t < 0.0001f)return{{0,0,0,0}, {0,0,0}, {0,0,0}, -1};

Vec3 cord = add3v(ray.origin, scelar3v(ray.direction, t)); 

//Vec3 normal = normalize3v(crossProduct3v(e, f)); optimization
//if(D < 0) normal = normalize3v(crossProduct3v(f, e));
Vec3 finalNormal = normal;

if (dotProduct3v(finalNormal, ray.direction) > 0) finalNormal = scelar3v(finalNormal, -1);
return {color, cord, finalNormal, t};
}





Tetrahedron::Tetrahedron(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Pixel color)   :   a(a), b(b), c(c), d(d), color(color), 
alpha(a, b, c, color), 
beta(d, b, c, color),
gamma(a, d, c, color),
delta(a, b, d, color) {}


//could use closestp
/*
locPixelv hitRR(Ray ray)
{
    std::array<Triangle*, 4> faces = { &alpha, &beta, &gamma, &delta };

    ClosestP closest;

    for (Triangle* tri : faces)
        closest.closestH(tri->hitRR(ray));
 
    return closest.colCord;
}
*/

locPixelv Tetrahedron::hitRR(const Ray& ray) const
{
    ClosestP closest;

    closest.closestH(alpha.hitRR(ray));
    closest.closestH(beta.hitRR(ray));
    closest.closestH(gamma.hitRR(ray));
    closest.closestH(delta.hitRR(ray));

    return closest.colCord;
}





Pixel postLighting(const LightSource &light, const locPixelv &point, float ambient ){
Vec3 lightNormal = normalize3v(subtract3v(light.cordinates, point.cordinates));

float lightAlignment = std::max(dotProduct3v(point.normal, lightNormal), ambient);


return {(unsigned char)(point.color.r * lightAlignment),
        (unsigned char)(point.color.g * lightAlignment),
        (unsigned char)(point.color.b * lightAlignment), point.color.a };
}


/*
Pixel shadowImplementationnocap( const std::vector<std::unique_ptr<Object>> &scene,  const locPixelv &colCord,  const std::vector<std::unique_ptr<LightSource>> &lights, float ambient)
{
float brightR = 0.0f;
float brightG = 0.0f;
float brightB = 0.0f;

for(const auto& ligh : lights){
bool blocked = false;

Vec3 offsetOrigin = add3v(colCord.cordinates, scelar3v(colCord.normal, 0.001f));

Vec3 toLight = subtract3v(ligh->cordinates, colCord.cordinates);
float lightDistance = sqrt(dotProduct3v(toLight, toLight));

Ray shadowray{
    offsetOrigin,
    normalize3v(toLight)
};

for (const auto& obj : scene) {

    auto hit = obj->hitRR(shadowray);

    if (hit.t > 0.001f && hit.t < lightDistance) {
        blocked = true;
        break;
    }
}

if(!blocked){
float NdotL = std::max(dotProduct3v(colCord.normal, shadowray.direction), 0.0f);

    brightR += (ligh->color.r / 255.0f) * NdotL;
    brightG += (ligh->color.g / 255.0f) * NdotL;
    brightB += (ligh->color.b / 255.0f) * NdotL;
    brightR = std::min(brightR, 1.0f);
    brightG = std::min(brightG, 1.0f);
    brightB = std::min(brightB, 1.0f);
}
}
    return {static_cast<unsigned char>(colCord.color.r * brightR),
            static_cast<unsigned char>(colCord.color.g * brightG),
            static_cast<unsigned char>(colCord.color.b * brightB),
            colCord.color.a};
}

*/


Pixel shadowImplementation( const std::vector<std::unique_ptr<Object>> &scene,  const locPixelv &colCord,  
                            const std::vector<std::unique_ptr<LightSource>> &lights, float ambient)
{
float brightR = 0.0f;
float brightG = 0.0f;
float brightB = 0.0f;

for(const auto& ligh : lights){
bool blocked = false;

Vec3 offsetOrigin = add3v(colCord.cordinates, scelar3v(colCord.normal, 0.001f));

Vec3 toLight = subtract3v(ligh->cordinates, colCord.cordinates);
float lightDistanceSq = dotProduct3v(toLight, toLight);    //float lightDistance = sqrt(dotProduct3v(toLight, toLight)); 

Ray shadowray{
    offsetOrigin,
    normalize3v(toLight)
};

for (const auto& obj : scene) {

    auto hit = obj->hitRR(shadowray);

    if (hit.t > 0.001f && (hit.t * hit.t) < lightDistanceSq) { //if (hit.t > 0.001f && (hit.t * hit.t) < lightDistanceSq) compare Sq, less math to Sq than root
        blocked = true;
        break;
    }
}

if(!blocked){
float NdotL = std::max(dotProduct3v(colCord.normal, shadowray.direction), 0.0f);

    brightR += (ligh->color.r / 255.0f) * NdotL;
    brightG += (ligh->color.g / 255.0f) * NdotL;
    brightB += (ligh->color.b / 255.0f) * NdotL;
    brightR = std::min(brightR, 1.0f);
    brightG = std::min(brightG, 1.0f);
    brightB = std::min(brightB, 1.0f);
/*
    brightR = std::max(brightR, ambient);
    brightG = std::max(brightG, ambient);
    brightB = std::max(brightB, ambient);
*/
}
}
    return {static_cast<unsigned char>(colCord.color.r * std::max(brightR, ambient)),
            static_cast<unsigned char>(colCord.color.g * std::max(brightG, ambient)),
            static_cast<unsigned char>(colCord.color.b * std::max(brightB, ambient)),
            colCord.color.a};
}