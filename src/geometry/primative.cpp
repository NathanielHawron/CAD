#include "CAD/geometry/primative.hpp"

using namespace CAD;
using namespace geometry;

std::string Sphere::toString() const {
    std::string r = std::to_string(this->radius);
    std::string x = std::to_string(this->pos.x), y = std::to_string(this->pos.y), z = std::to_string(this->pos.z);
    return "Sphere: r="+r+", pos=("+x+", "+y+", "+z+")";
}

bool Sphere::inside(glm::vec3 point){

}
glm::vec3 Sphere::project(glm::vec3 origin, glm::vec3 ray){

}
Graph Sphere::generateGraph(){
    
}
Graph Sphere::unionGraph(Volume *other){

}
Graph Sphere::differenceGraph(Volume *other){

}
Graph Sphere::intersectionGraph(Volume *other){

}