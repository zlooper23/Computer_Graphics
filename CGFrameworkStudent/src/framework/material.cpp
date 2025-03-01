#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"
#include "material.h"

Material::Material(){

}

Material::Material(Texture *texture, Shader *shader){
    shader->Enable();
    myTexture = texture;
    shader->SetTexture("u_texture", myTexture);
    shader->Disable();
    myShader = shader;
    Ka = Vector3(1.0, 1.0, 1.0);
    Kd = Vector3(1.0, 1.0, 1.0);
    Ks = Vector3(1.0, 1.0, 1.0);
}

void Material::Enable(){
    myShader->Enable();
}

void Material::Disable(){
    myShader->Disable();
}
