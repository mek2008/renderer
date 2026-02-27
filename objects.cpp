#include <iostream>
#include "objects.hpp"
#include <cmath>
#include <limits>


float dotProduct3v(const Vec3 &a, const Vec3 &b){
return a.x*b.x+a.y*b.y+a.z*b.z;
}

Vec3 scelar3v(const Vec3 &vector, float scelar){
return{vector.x * scelar, vector.y * scelar, vector.z * scelar};
}

Vec3 crossProduct3v(const Vec3 &a, Vec3 const &b){
return {a.y*b.z - a.z*b.y  ,  a.z*b.x - a.x*b.z  ,  a.x*b.y - a.y*b.x};
}
Vec3 add3v(const Vec3 &a, const Vec3 &b){ //could be used to find vector from 2 points by doing (endPoint, origin)
return {a.x+b.x, a.y+b.y, a.z+b.z };
}
Vec3 subtract3v(const Vec3 &a, const Vec3 &b){ //could be used to find vector from 2 points by doing (endPoint, origin)
return {a.x-b.x, a.y-b.y, a.z-b.z };
}

Vec3 normalize3v(Vec3 vector) {
float lenght = sqrt(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
if(lenght != 0){
return {vector.x/lenght, vector.y/lenght, vector.z/lenght};
} else {return {0, 0, 0};}
}

/*
Vec3 rodriguesrad(Vec3 u, Vec3 v, float radrotation){
v = add3v(
        add3v(
            scelar3v(v, cos(radrotation)), 
            scelar3v(crossProduct3v(u, v), sin(radrotation))
        ), 
        scelar3v(u, dotProduct3v(u, v) * (1 - cos(radrotation)))
);
}
*/

float degreesToRadians(float degrees) {return degrees * (3.141592 / 180.0f);}







Perspective::Perspective(Vec3 position, float focalLength) : position(position), focalLength(focalLength) {}

void Perspective::rotatetioni(float radrotation){
//v’ = v * cosθ + (u × v) * sinθ + u * (u · v) * (1 − cosθ)
j = add3v(
        add3v(
            scelar3v(j, cos(radrotation)), 
            scelar3v(crossProduct3v(i, j), sin(radrotation))
        ), 
        scelar3v(i, dotProduct3v(i, j) * (1 - cos(radrotation)))
);
k = add3v(
        add3v(
            scelar3v(k, cos(radrotation)), 
            scelar3v(crossProduct3v(i, k), sin(radrotation))
        ), 
        scelar3v(i, dotProduct3v(i, k) * (1 - cos(radrotation)))
);
}
void Perspective::rotatetionj(float radrotation){
//v’ = v * cosθ + (u × v) * sinθ + u * (u · v) * (1 − cosθ)
i = add3v(
        add3v(
            scelar3v(i, cos(radrotation)), 
            scelar3v(crossProduct3v(j, i), sin(radrotation))
        ), 
        scelar3v(j, dotProduct3v(j, i) * (1 - cos(radrotation)))
);
k = add3v(
        add3v(
            scelar3v(k, cos(radrotation)), 
            scelar3v(crossProduct3v(j, k), sin(radrotation))
        ), 
        scelar3v(j, dotProduct3v(j, k) * (1 - cos(radrotation)))
);
}
void Perspective::rotatetionk(float radrotation){
//v’ = v * cosθ + (u × v) * sinθ + u * (u · v) * (1 − cosθ)
i = add3v(
        add3v(
            scelar3v(i, cos(radrotation)), 
            scelar3v(crossProduct3v(k, i), sin(radrotation))
        ), 
        scelar3v(k, dotProduct3v(k, i) * (1 - cos(radrotation)))
);
j = add3v(
        add3v(
            scelar3v(j, cos(radrotation)), 
            scelar3v(crossProduct3v(k, j), sin(radrotation))
        ), 
        scelar3v(k, dotProduct3v(k, j) * (1 - cos(radrotation)))
);
}
void Perspective::reOrthonormalization(){
    i = normalize3v(i);
    j = normalize3v(j);
    k = crossProduct3v(i, j);
    j = crossProduct3v(k, i); 
}

// Vec3 version
Vec3 Perspective::globaToLocalv(Vec3 world) {
    return {
        i.x * world.x + j.x * world.y + k.x * world.z,
        i.y * world.x + j.y * world.y + k.y * world.z,
        i.z * world.x + j.z * world.y + k.z * world.z
    };
}

// Float version
Vec3 Perspective::globaToLocal(float xo, float yo, float zo) {
    return {
        i.x * xo + j.x * yo + k.x * zo,
        i.y * xo + j.y * yo + k.y * zo,
        i.z * xo + j.z * yo + k.z * zo
    };
}

// Vec3 version
Vec3 Perspective::localToGlobalv(Vec3 local) {
    return {
        i.x * local.x + j.x * local.y + k.x * local.z,
        i.y * local.x + j.y * local.y + k.y * local.z,
        i.z * local.x + j.z * local.y + k.z * local.z
    };
}

// Float version
Vec3 Perspective::localToGlobal(float xo, float yo, float zo) {
    return {
        i.x * xo + j.x * yo + k.x * zo,
        i.y * xo + j.y * yo + k.y * zo,
        i.z * xo + j.z * yo + k.z * zo
    };
}


