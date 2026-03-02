# CPU Software Renderer — SDL3 + Modern C++

A **minimal CPU-based 3D software renderer** built completely from scratch using **SDL3** and **modern C++ (C++23/26)**.

This project manually generates rays, performs all math internally, writes pixels into a framebuffer, and uploads them to an SDL streaming texture every frame.

The goal is **full control over the rendering pipeline**, deep understanding of 3D math, and learning how real graphics engines work — without hiding anything behind libraries.

---

## Features

- Manual framebuffer (`std::vector<Pixel>`)
- Full CPU-based rendering (no GPU pipeline)
- Ray generation per pixel
- Ray–sphere intersection
- Custom 3D vector math library
- Perspective camera with orthonormal basis
- Rodrigues rotation formula
- Local ↔ global space transformations
- Diffuse lighting (Lambert shading)
- SDL3 streaming texture pipeline
- Real-time camera rotation

---

## Rendering Pipeline

Ray Generation → Intersection Tests → Lighting → Framebuffer → SDL_Texture → Screen

Each frame:

1. Generate a ray for every pixel  
2. Compute direction in camera-local space  
3. Transform direction into world space  
4. Perform intersection tests  
5. Apply lighting  
6. Write final color to framebuffer  
7. Upload framebuffer to SDL texture  
8. Present to screen  

---

## Build Instructions (Windows / MSYS2 / MinGW)

Compile using:

```bash
g++ src/main.cpp src/objects.cpp -o main.exe \
-I "~\code_projects\SDL\x86_64-w64-mingw32\include" \
-L "~\code_projects\SDL\x86_64-w64-mingw32\lib" \
-lSDL3
````

Make sure:

* SDL3 headers and libraries are installed
* SDL3 `.dll` is available at runtime
* Compiler matches the SDL build (MinGW → MinGW SDL)

---

## Mathematics Used

### 1. Ray Equation

r(t) = a + t·b

Where:

* a = ray origin
* b = ray direction
* t = distance parameter

---

### 2. Sphere Intersection

Sphere equation:

x² + y² + z² = r²

Substituting the ray equation yields:

(b·b)t² + 2(a·b)t + (a·a − r²) = 0

Solved using the quadratic formula.

Intersection exists if:

Δ ≥ 0

---

### 3. Orthonormal Camera Basis

The camera maintains three perpendicular unit vectors:

* i — right
* j — up
* k — forward

Properties:

i·j = 0
i·k = 0
j·k = 0
|i| = |j| = |k| = 1

This allows clean local ↔ world space transformations.

---

### 4. Rodrigues Rotation Formula

v' = v cosθ + (u × v) sinθ + u(u·v)(1 − cosθ)

Where:

* u = rotation axis
* v = vector being rotated

---

## Camera Model

Each pixel generates a ray using perspective projection:

u = normalized screen x
v = normalized screen y
direction = normalize(localToGlobal(u, v, focalLength))

Aspect ratio correction ensures proper geometry.

---

## Core Structures

### Framebuffer

```cpp
std::vector<Pixel> pixels;

Pixel& at(int x, int y) {
    return pixels[y * width + x];
}
```

Row-major layout:

index = y * width + x

---

### Perspective Camera

* Position
* Focal length
* Orthonormal basis vectors (i, j, k)
* Rotation using Rodrigues formula
* Space transformations

---

## Design Philosophy

* No external math libraries
* No GPU pipeline
* Everything built from scratch
* Math-first approach
* Educational clarity over abstraction

---

## Project Goals

Current:

* Ray generation
* Sphere intersections
* Camera movement & rotation
* Lighting

Next steps:

* Multiple objects
* Shadows
* Reflections
* Materials
* Acceleration structures (BVH / spatial partitioning)

---

## Requirements

* C++23 / C++26 compatible compiler
* SDL3
* Windows + MSYS2 / MinGW toolchain

---

## Notes

> If it works — don’t touch it.

This project is intentionally built as a **learning-focused renderer**, evolving through experimentation, mistakes, and iteration.


