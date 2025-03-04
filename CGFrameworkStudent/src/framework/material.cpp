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
    myTexture = texture;
    myShader = shader;

    Ka = Vector3(1.0, 1.0, 1.0);
    Kd = Vector3(0.8, 0.8, 0.8);
    Ks = Vector3(0.8, 0.8, 0.8);
    Shininess = 10.0;
}

void Material::Enable(const sUniformData& uniformData){
    myShader->Enable();
    myShader->SetMatrix44("u_model", uniformData.model);
    myShader->SetMatrix44("u_viewprojection", uniformData.cam->GetViewProjectionMatrix());
    myShader->SetTexture("u_texture", myTexture);
    myShader->SetVector3("u_eye", uniformData.cam->eye);
    myShader->SetVector3("u_lightPos", uniformData.light.position);
    myShader->SetVector3("u_Ia", uniformData.Ia);
    myShader->SetVector3("u_ka", Ka);
    myShader->SetVector3("u_kd", Kd);
    myShader->SetVector3("u_ks", Ks);
    myShader->SetFloat("u_s", Shininess);
    myShader->SetVector3("u_I", uniformData.light.intensity);
}

void Material::Disable(){
    myShader->Disable();
}
