#pragma once

#include <string.h>
#include <stdio.h>
#include <iostream>
#include "mesh.h"
#include "framework.h"
#include "image.h"



class Entity{

    public:
        Mesh mesh;
        Matrix44 modelMatrix;
        Image *myTexture;

        Entity();
        Entity(Mesh &mesh);
        Entity(Mesh &mesh, Image texture);
        Entity(Mesh &mesh, Matrix44 modelMatrix);

        void Render(Image* framebuffer, Camera* camera, const Color& c, FloatImage* zBuffer);
};
