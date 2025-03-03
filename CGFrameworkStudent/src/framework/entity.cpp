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
#include "entity.h"

Entity::Entity(){
    
}


Entity::Entity(Mesh &mesh){
    this->mesh = mesh;
    this->modelMatrix = Matrix44();
    this->myTexture = NULL;
    this->mode = eRenderMode::TEXTURES;
    this->oclussions = true;
}
Entity::Entity(Mesh &mesh, Image *texture){
    this->mesh = mesh;
    this->modelMatrix = Matrix44();
    this->myTexture = texture;
    this->mode = eRenderMode::TEXTURES;
    this->oclussions = true;
}

Entity::Entity(Mesh &mesh, Matrix44 modelMatrix){
    this->mesh = mesh;
    this->modelMatrix = modelMatrix;
    this->mode = eRenderMode::TEXTURES;
    this->oclussions = true;
}

Entity::Entity(Mesh &mesh, Image *texture, Shader* shader) {
    this->mesh = mesh;
    this->myTexture = texture;
    this->shader = shader; 
    this->modelMatrix = Matrix44();
    this->mode = eRenderMode::TEXTURES;
    this->oclussions = true;
}

Entity::Entity(Mesh &mesh, Material* mat){
    this->mesh = mesh;
    this->material = mat; 
    this->modelMatrix = Matrix44();
    this->mode = eRenderMode::TEXTURES;
    this->oclussions = true;
}

void Entity::ChangeMode(int n){
    switch(n){
        case 0: mode = eRenderMode::POINTCLOUD; break;
        case 1: mode = eRenderMode::WIREFRAME; break;
        case 2: mode = eRenderMode::TRIANGLES; break;
        case 3: mode = eRenderMode::TRIANGLES_INTERPOLATED; break;
        case 4: mode = eRenderMode::TEXTURES; break;
    }
}

int Entity::GetMode(){
    switch(mode){
        case eRenderMode::POINTCLOUD: return 0;
        case eRenderMode::WIREFRAME: return 1;
        case eRenderMode::TRIANGLES: return 2;
        case eRenderMode::TRIANGLES_INTERPOLATED: return 3;
        case eRenderMode::TEXTURES: return 4;
    }
}

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c, FloatImage* zBuffer){
    const std::vector<Vector3>& vertices = mesh.GetVertices();
    const std::vector<Vector2>& uvs = mesh.GetUVs();
    std::vector<Vector3> points;
    Matrix44 viewProjectionMatrix = camera->viewprojection_matrix;
    bool negZ = false;
    bool clip = true;
    for(int i = 0; i < vertices.size(); i++){
        Vector3 p = camera->ProjectVector(modelMatrix * vertices[i], negZ);
        if((abs(p.x)>1 || abs(p.y)>1)||(abs(p.z)>1)){
            clip = false;
        }

        p.x = ((p.x+1)/2)*(framebuffer->width-1);
        p.y = ((p.y+1)/2)*(framebuffer->height-1);
        points.push_back(p);
        if(i%3==2){
            if(clip){
                TriangleInfo *t = new TriangleInfo;
                switch (mode)
                {
                case eRenderMode::POINTCLOUD:
                    framebuffer->SetPixel(points[i-2].x, points[i-2].y, c);
                    framebuffer->SetPixel(points[i-1].x, points[i-1].y, c);
                    framebuffer->SetPixel(points[i].x, points[i].y, c);
                    break;
                
               case eRenderMode::WIREFRAME:
                    framebuffer->DrawLineDDA(points[i-2].x, points[i-2].y, points[i-1].x, points[i-1].y, c);
                    framebuffer->DrawLineDDA(points[i-2].x, points[i-2].y, points[i].x, points[i].y, c);
                    framebuffer->DrawLineDDA(points[i].x, points[i].y, points[i-1].x, points[i-1].y, c);
                    break;
                case eRenderMode::TRIANGLES:
                    framebuffer->DrawTriangle(Vector2(points[i-2].x, points[i-2].y), 
                     Vector2(points[i-1].x, points[i-1].y), Vector2(points[i].x, points[i].y), c, true, c);
                    break;
                case eRenderMode::TRIANGLES_INTERPOLATED:
                    t->p0 = points[i-2];
                    t->p1 = points[i-1];
                    t->p2 = points[i];
                    t->c0 = Color::GREEN;
                    t->c1 = Color::RED;
                    t->c2 = Color::BLUE;
                    framebuffer->DrawTriangleInterpolated(t, zBuffer, NULL, false, oclussions);
                    break;
                case eRenderMode::TEXTURES:
                    if(myTexture == nullptr) {
                        t->p0 = points[i-2];
                        t->p1 = points[i-1];
                        t->p2 = points[i];
                        t->c0 = Color::GREEN;
                        t->c1 = Color::RED;
                        t->c2 = Color::BLUE;
                        framebuffer->DrawTriangleInterpolated(t, zBuffer, NULL, false, oclussions);
                    } else {
                        t->p0 = points[i-2];
                        t->p1 = points[i-1];
                        t->p2 = points[i];
                        t->uv0 = Vector2(uvs[i-2].x*(myTexture->width-1), uvs[i-2].y*(myTexture->height-1));
                        t->uv1 = Vector2(uvs[i-1].x*(myTexture->width-1), uvs[i-1].y*(myTexture->height-1));
                        t->uv2 = Vector2(uvs[i].x*(myTexture->width-1), uvs[i].y*(myTexture->height-1));
                        framebuffer->DrawTriangleInterpolated(t, zBuffer, myTexture, true, oclussions);
                    }
                    break;
                }
                
            }
            clip = true;
        }
    }
}


void Entity::Render(Camera *camera){
    if(!shader) return;

    //material->Enable();
    material->myShader->SetMatrix44("u_model", modelMatrix);
    material->myShader->SetMatrix44("u_viewprojection", camera->GetViewProjectionMatrix());
    material->myShader->SetTexture("u_texture", material->myTexture);

    glEnable(GL_DEPTH_TEST);
    mesh.Render();

    material->Disable();
}

void Entity::Render(sUniformData uniformData){
    if(!shader) return;

    uniformData.model = modelMatrix;

    material->Enable(uniformData);

    glEnable(GL_DEPTH_TEST);
    mesh.Render();

    material->Disable();
}

void Entity::Update(float seconds_elapsed, int animationMode) {
    totalTime += seconds_elapsed;
    switch (animationMode) {
        case 0: {  //Rotation
            float angle = seconds_elapsed * 2.5f;
            Matrix44 rotation;
            rotation.SetRotation(angle, Vector3(0, 1, 0));
            modelMatrix = modelMatrix * rotation;
            break;
        }
        
        case 1: {  //Translation 
            float yOffset = sin(totalTime * 2.0f) * 1.0f; 
            
            Matrix44 translationMatrix;
            translationMatrix.SetTranslation(0, yOffset, 0); 
            
            modelMatrix = translationMatrix;
            break;
        }

        case 2: {  //Scaling 
            float scaleFactor = 1.0f + sin(totalTime * 2.0f); 
            
            Matrix44 scaleMatrix;
            scaleMatrix.Set(
                scaleFactor, 0, 0, 0,
                0, scaleFactor, 0, 0,
                0, 0, scaleFactor, 0,
                0, 0, 0, 1
            );
            
            Matrix44 baseMatrix = modelMatrix;
            baseMatrix.M[0][0] = 1.0f;  //Reset X scale
            baseMatrix.M[1][1] = 1.0f;  //Reset Y scale
            baseMatrix.M[2][2] = 1.0f;  //Reset Z scale
            
            modelMatrix = baseMatrix * scaleMatrix;
            break;
        }
        default:
            break;
    }
}