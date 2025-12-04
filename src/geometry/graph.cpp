#include "CAD/geometry/graph.hpp"

using namespace CAD;
using namespace geometry;

const NRA::VGL::VertexBufferLayout Graph::Vertex::layout{{
    {GL_FLOAT, 3, GL_FALSE}
}};

bool Graph::Position::isAlmostEqual(Position p, float e){
    glm::vec3 dp = this->pos - p.pos;
    float d = glm::dot(dp,dp);
    return d*d <= e*e;
}

template <class I>
void Graph::addToMesh(NRA::VGL::Mesh<I> &mesh) const {
    geometry::Graph::Vertex vertices[4] = {
        {{-0.5f, -0.5f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}}
    };
    I indices[6] = {
        0,1,2,
        0,2,3
    };
    mesh.add(vertices, indices, 4, 6);

    // std::vector<Vertex> vertices;
    // std::vector<I> indices;
    // vertices.reserve(this->positions.size());
    // indices.reserve(this->faces.size()*3);
    /*for(const Face & face : this->faces){
        Position p[3] = {this->positions.at(face.p[0]),this->positions.at(face.p[1]),this->positions.at(face.p[2])};
        glm::vec3 dp[2] = {p[0].pos-p[1].pos, p[1].pos-p[2].pos};
        glm::vec3 norm = glm::normalize(glm::cross(dp[0],dp[1]));
        for(uint_fast8_t i=0;i<3;++i){
            indices.push_back(vertices.size());
            vertices.push_back({{p[i].pos.x,p[i].pos.y,p[i].pos.z}});//,{norm.x,norm.y,norm.z},{p[i].color.x,p[i].color.y,p[i].color.z},{0.0f,0.0f}});
        }
    }*/
    // mesh.add(vertices.data(),indices.data(),vertices.size(),indices.size());
}
void Graph::pushPos(Position p){
    this->positions.push_back(p);
}
void Graph::pushFace(Face f){
    this->faces.push_back(f);
}
std::pair<bool, std::size_t> Graph::findPos(Position p){
    // @TODO: complete funcntion, add indexing?
    return {false,0};
}
void Graph::mergePositions(float e){
    // @TODO: complete function
}
Graph &Graph::operator+=(Graph &g){
    std::size_t offset = this->positionOffset();
    this->positions.reserve(this->positions.size() + g.positions.size());
    for(Position &p : g.positions){
        this->positions.push_back(p);
    }
    for(Face &f : g.faces){
        this->faces.push_back(f + offset);
    }
    return *this;
}

bool Graph::inside(glm::vec3 point){

}
glm::vec3 Graph::project(glm::vec3 origin, glm::vec3 ray){
    
}

Graph Graph::generateGraph(){
    
}
Graph Graph::unionGraph(Volume *other){
    
}
Graph Graph::differenceGraph(Volume *other){
    
}
Graph Graph::intersectionGraph(Volume *other){
    
}
Graph Graph::transform(geometry::Transform t){
    
}
bool Graph::intersects(Volume *other){

};
bool Graph::encloses(Volume *other){

};
bool Graph::enclosed(Volume *other){

};


template void Graph::addToMesh(NRA::VGL::Mesh<GLubyte> &mesh) const;
template void Graph::addToMesh(NRA::VGL::Mesh<GLushort> &mesh) const;
template void Graph::addToMesh(NRA::VGL::Mesh<GLuint> &mesh) const;
//template void Graph::addToMesh(NRA::VGL::Mesh<GLuint64> &mesh) const;