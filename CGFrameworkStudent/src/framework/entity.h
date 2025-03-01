#pragma once

#include <string.h>
#include <stdio.h>
#include <iostream>
#include "mesh.h"
#include "framework.h"
#include "image.h"
#include "shader.h"
#include "material.h"



class Entity{

    enum class eRenderMode {

        POINTCLOUD,

        WIREFRAME,

        TRIANGLES,

        TRIANGLES_INTERPOLATED,

        TEXTURES

    }; eRenderMode mode;


    public:
        Mesh mesh;
        Matrix44 modelMatrix;
        Image *myTexture;
        Shader *shader;
        Material *material;
        float totalTime = 0.0f;
        bool oclussions;

        Entity();
        Entity(Mesh &mesh);
        Entity(Mesh &mesh, Image *texture);
        Entity(Mesh &mesh, Matrix44 modelMatrix);
        Entity(Mesh &mesh, Image *texture, Shader* shader);
        Entity(Mesh &mesh, Material* mat);


        void ChangeMode(int n);
        int GetMode();


        void Render(Image* framebuffer, Camera* camera, const Color& c, FloatImage* zBuffer);
        void Render(Camera* camera);
        void Update(float seconds_elapsed, int animationMode);
};
