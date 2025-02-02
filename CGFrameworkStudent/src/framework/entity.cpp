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

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c){
    const std::vector<Vector3>& vertices = mesh.GetVertices();
    std::vector<Vector3> points;
    Matrix44 viewProjectionMatrix = camera->GetViewProjectionMatrix();
    //modelMatrix.SetRotation(1.0, Vector3(1, 0, 0));
    modelMatrix.SetTranslation(-400, 100, 0);
    for(int i = 0; i < vertices.size(); i++){
        Vector3 p = modelMatrix * vertices[i];
        p = viewProjectionMatrix * p;
        points.push_back(p);

        if(i%3==2){
            framebuffer->DrawLineDDA(points[i-2].x, points[i-2].y, points[i-1].x, points[i-1].y, Color::WHITE);
            framebuffer->DrawLineDDA(points[i-1].x, points[i-1].y, points[i].x, points[i].y, Color::WHITE);
            framebuffer->DrawLineDDA(points[i].x, points[i].y, points[i-2].x, points[i-2].y, Color::WHITE);
        }
    }
}