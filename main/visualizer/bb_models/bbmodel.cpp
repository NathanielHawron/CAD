#include "bbmodel.hpp"


using namespace CAD;
using namespace geometry;
using namespace visualizer;

template <class T>
BoundingBoxVisualizer<T>::BoundingBoxVisualizer(BoundingBox::BB<T> &bb):bb{bb}{
    glGenBuffers(2, this->buffers.id);
    glGenVertexArrays(1, &this->vao);
}

template <class T>
void BoundingBoxVisualizer<T>::bindBuffers(){
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers.bo.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers.bo.ibo);
}

template <class T>
void BoundingBoxVisualizer<T>::generateMesh(){
    if(auto *box = dynamic_cast<BoundingBox::Box<T>*>(&this->bb)){
        this->generateMesh(box);
    }else if(auto *sphere = dynamic_cast<BoundingBox::Sphere<T>*>(&this->bb)){
        this->generateMesh(sphere);
    }
}

template <class T>
bool BoundingBoxVisualizer<T>::uploadMesh(){
    if(this->isMeshEmpty()){
        return false;
    }
    this->bindBuffers();
    glBufferData(GL_ARRAY_BUFFER, this->vertexCount*sizeof(vertex), this->vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexCount, this->indices, GL_STATIC_DRAW);
    this->deleteMesh();
    return true;
}

template <class T>
void BoundingBoxVisualizer<T>::render(){
    this->bindBuffers();
    glDrawArrays(GL_TRIANGLES, 0, this->indexCount);
}


template <class T>
bool BoundingBoxVisualizer<T>::isMeshEmpty(){
    return this->vertices == nullptr && this->indices == nullptr && this->vertexCount == 0 && this->indexCount == 0;
}

template <class T>
void BoundingBoxVisualizer<T>::createMesh(std::size_t vertexCount, std::size_t indexCount){
    if(!this->isMeshEmpty()){
        return;
    }
    this->vertexCount = vertexCount;
    this->vertices = new vertex[vertexCount];
    this->indexCount = indexCount;
    this->indices = new GLuint[indexCount];
}

template <class T>
void BoundingBoxVisualizer<T>::deleteMesh(){
    if(this->vertices != nullptr){
        delete[] this->vertices;
    }
    if(this->indices != nullptr){
        delete[] this->indices;
    }
    this->vertices = nullptr;
    this->indices = nullptr;
    this->vertexCount = 0;
    this->indexCount = 0;
}

template <class T>
void BoundingBoxVisualizer<T>::generateMesh(BoundingBox::Box<T> *box){
    this->createMesh(8,36);
    T minx = box->x - box->dx/2;
    T miny = box->y - box->dy/2;
    T minz = box->z - box->dz/2;
    T maxx = box->x + box->dx/2;
    T maxy = box->y + box->dy/2;
    T maxz = box->z + box->dz/2;
    
    this->vertices[0] = {minx, miny, minz};
    this->vertices[1] = {minx, maxy, minz};
    this->vertices[2] = {maxx, maxy, minz};
    this->vertices[3] = {maxx, miny, minz};
    
    this->vertices[4] = {minx, miny, maxz};
    this->vertices[5] = {minx, maxy, maxz};
    this->vertices[6] = {maxx, maxy, maxz};
    this->vertices[7] = {maxx, miny, maxz};

    // NZ
    this->indices[ 0] = 0;
    this->indices[ 1] = 1;
    this->indices[ 2] = 2;
    this->indices[ 3] = 2;
    this->indices[ 4] = 3;
    this->indices[ 5] = 0;

    // PZ
    this->indices[ 6] = 0;
    this->indices[ 7] = 3;
    this->indices[ 8] = 2;
    this->indices[ 9] = 2;
    this->indices[10] = 1;
    this->indices[11] = 0;
}

template <class T>
void BoundingBoxVisualizer<T>::generateMesh(BoundingBox::Sphere<T> *sphere){
    
}



template class BoundingBoxVisualizer<float>;