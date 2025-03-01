#pragma once

#include <string.h>
#include <stdio.h>
#include <iostream>
#include "mesh.h"
#include "framework.h"
#include "image.h"
#include "shader.h"
#include "texture.h"

typedef struct sLight{

}sLight;

class Material{

    public:
        Texture *myTexture;
        Shader *myShader;
        Vector3 Ka;
        Vector3 Kd;
        Vector3 Ks;

        Material();
        Material(Texture *texture, Shader *shader);
        void Enable();
        void Disable();

};