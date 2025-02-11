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
}

Entity::Entity(Mesh &mesh, Matrix44 modelMatrix){
    this->mesh = mesh;
    this->modelMatrix = modelMatrix;
}

Entity::Entity(Matrix44 modelMatrix){
    this->modelMatrix = modelMatrix;
}

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c, FloatImage* zBuffer){
    const std::vector<Vector3>& vertices = mesh.GetVertices();
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
                framebuffer->DrawTriangleInterpolated(points[i-2], points[i-1], points[i], Color::GREEN, Color::RED, Color::BLUE, zBuffer);
 
            }
            clip = true;
        }
    }
}