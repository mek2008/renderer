#pragma once
#include <limits>

struct Pixel { unsigned char r, g, b, a; };

struct Vec3 { float x, y, z; };


struct Rotation3matrix { Vec3 x, y, z; }; //x, y, z



struct Ray { Vec3 origin, direction; };

class ClosestP{ public:
float nearT = std::numeric_limits<float>::max();
Pixel color = {0, 0, 0};
};


float dotProduct3v(const Vec3 &a, const Vec3 &b);

Vec3 scelar3v(const Vec3 &vector, float scelar);

Vec3 crossProduct3v(const Vec3 &a, Vec3 const &b);

Vec3 add3v(const Vec3 &a, const Vec3 &b);

Vec3 subtract3v(const Vec3 &a, const Vec3 &b);

Vec3 normalize3v(Vec3 vector);

float degreesToRadians(float degrees);




class Perspective{ public:
Vec3 position;
Vec3 i{1, 0, 0}, j{0, 1, 0}, k{0, 0, 1}; //rotation matrix
float focalLength;


Perspective(Vec3 position, float focalLength);

void rotatetioni(float radrotation);

void rotatetionj(float radrotation);

void rotatetionk(float radrotation);

void reOrthonormalization();

Vec3 globaToLocalv(Vec3 world);
Vec3 globaToLocal(float xo, float yo, float zo);

Vec3 localToGlobalv(Vec3 local);
Vec3 localToGlobal(float xo, float yo, float zo);
};




