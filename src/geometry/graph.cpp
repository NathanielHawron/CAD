#include "CAD/geometry/graph.hpp"

using namespace CAD;
using namespace geometry;

void Graph::CSGintersect(const Graph &g){

}
void Graph::CSGdifference(const Graph &g){
    
}
void Graph::CSGunion(const Graph &g){
    
}

template <class I>
void Graph::addToMesh(NRA::VGL::Mesh<I> &mesh) const {
    std::vector<Vertex> vertices;
    std::vector<I> indices;
    vertices.reserve(this->positions.size());
    indices.reserve(this->faces.size()*3);
    for(const Face & face : this->faces){
        Position p[3] = {this->positions.at(face.p[0]),this->positions.at(face.p[1]),this->positions.at(face.p[2])};
        glm::vec3 dp[2] = {p[0].pos-p[1].pos, p[1].pos-p[2].pos};
        glm::vec3 norm = glm::normalize(glm::cross(dp[0],dp[1]));
        for(uint_fast8_t i=0;i<3;++i){
            indices.push_back(vertices.size());
            vertices.push_back({{p[i].pos.x,p[i].pos.y,p[i].pos.z},{norm.x,norm.y,norm.z},{p[i].color.x,p[i].color.y,p[i].color.z},{0.0f,0.0f}});
        }
    }
}


template void Graph::addToMesh(NRA::VGL::Mesh<GLubyte> &mesh) const;
template void Graph::addToMesh(NRA::VGL::Mesh<GLushort> &mesh) const;
template void Graph::addToMesh(NRA::VGL::Mesh<GLuint> &mesh) const;
template void Graph::addToMesh(NRA::VGL::Mesh<GLuint64> &mesh) const;