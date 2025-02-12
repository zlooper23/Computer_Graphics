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
}
Entity::Entity(Mesh &mesh, Image texture){
    this->mesh = mesh;
    this->modelMatrix = Matrix44();
    this->myTexture = &texture;
}

Entity::Entity(Mesh &mesh, Matrix44 modelMatrix){
    this->mesh = mesh;
    this->modelMatrix = modelMatrix;
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
                //framebuffer->DrawTriangle(Vector2(points[i-2].x, points[i-2].y), Vector2(points[i-1].x, points[i-1].y), Vector2(points[i].x, points[i].y), c, true, c);

                TriangleInfo *t = new TriangleInfo;
                t->p0 = points[i-2];
                t->p1 = points[i-1];
                t->p2 = points[i];

                if(myTexture == nullptr) {
                    t->c0 = Color::GREEN;
                    t->c1 = Color::RED;
                    t->c2 = Color::BLUE;
                    framebuffer->DrawTriangleInterpolated(t, zBuffer);
                } else {
                    t->c0 = myTexture->GetPixel(uvs[i-2].x, uvs[i-2].y);
                    t->c1 = myTexture->GetPixel(uvs[i-1].x, uvs[i-1].y);
                    t->c2 = myTexture->GetPixel(uvs[i].x, uvs[i].y);
                    framebuffer->DrawTriangleInterpolated(t, zBuffer);
                }
                
 
            }
            clip = true;
        }
    }
}