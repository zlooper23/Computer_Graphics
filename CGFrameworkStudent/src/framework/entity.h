#pragma once

#include <string.h>
#include <stdio.h>
#include <iostream>
#include "mesh.h"
#include "framework.h"

class Entity{
    public:
        Mesh mesh;
        Matrix44 modelMatrix;

        Entity(Mesh &mesh);
        Entity(Mesh &mesh, Matrix44 modelMatrix);
        Entity(Matrix44 modelMatrix);

        void Render(Image* framebuffer, Camera* camera, const Color& c);
};