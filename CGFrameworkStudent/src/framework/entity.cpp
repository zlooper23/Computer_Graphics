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

Entity::Entity(Mesh mesh){
    this->mesh = mesh;
    this->modelMatrix = Matrix44();
}

Entity::Entity(Mesh mesh, Matrix44 modelMatrix){
    this->mesh = mesh;
    this->modelMatrix = modelMatrix;
}

Entity::Entity(Matrix44 modelMatrix){
    this->modelMatrix = modelMatrix;
}

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c){
    std::vector<Vector3> vertices = mesh.GetVertices();
    int size = vertices.size();
    modelMatrix;
    for(int i = 0; i<size; i++){
        Vector3 v = vertices[i];
        mesh.vertices[i] = modelMatrix*v;
    }
    mesh.Render(1);
}