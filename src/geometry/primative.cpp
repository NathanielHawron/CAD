#include "CAD/geometry/primative.hpp"

#include <unordered_map>

using namespace CAD;
using namespace geometry;

std::string Sphere::toString() const {
    std::string r = std::to_string(this->radius);
    std::string x = std::to_string(this->pos.x), y = std::to_string(this->pos.y), z = std::to_string(this->pos.z);
    return "Sphere: r="+r+", pos=("+x+", "+y+", "+z+")";
}

bool Sphere::inside(glm::vec3 point){
    glm::vec3 delta = point-this->pos;
    double dist = glm::dot(delta, delta);
    return dist < this->radius * this->radius;
}
glm::vec3 Sphere::project(glm::vec3 origin, glm::vec3 ray){
    glm::vec3 delta = origin-this->pos;
    double dot = glm::dot(ray,delta);
}
Graph Sphere::generateGraph(){
    static const double pi = std::acos(-1.0);
    Graph res;
    glm::vec3 up = {0.0f, this->radius, 0.0f};
    const std::size_t v = std::max(this->subdivisions,(std::size_t)3);
    const double dphi = pi / (double)v;
    const std::size_t u = std::max(this->subdivisions,(std::size_t)3)*2;
    const double dtheta = 2.0 * pi / (double)u;

    res.pushPos({this->pos + up,{}});

    glm::vec3 ring[u] = {};
    for(int i=0;i<u;++i){
        ring[i] = this->radius*glm::vec3{std::cos((double)i * dtheta), 0, std::sin((double)i * dtheta)};
    }

    {   // Vertices
        for(std::size_t j=1;j<v-1;++j){
            float y = this->radius*std::cos(dphi * j);
            float r = std::sin(dphi * j);
            for(std::size_t i=0;i<u;++i){
                res.pushPos({this->pos + (ring[i] * r) + glm::vec3{0,y,0},{}});
            }
        }
    }

    { // Indices
        // Top row
        for(uint32_t i=0;i<u;++i){
            res.pushFace({0,i+1,((i+1)%u)+1});
        }
    }

    res.pushPos({this->pos - up,{}});
    return res;
}
Graph Sphere::unionGraph(Volume *other){
    Graph res;

    return res;
}
Graph Sphere::differenceGraph(Volume *other){

}
Graph Sphere::intersectionGraph(Volume *other){

}
Graph Sphere::transform(geometry::Transform t){
    Graph res = this->generateGraph();
    res.transform(t);
    return res;
}
bool Sphere::intersects(Volume *other){

};
bool Sphere::encloses(Volume *other){

};
bool Sphere::enclosed(Volume *other){

};