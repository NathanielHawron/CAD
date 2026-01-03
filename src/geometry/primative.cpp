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

    // Define Constants
    const glm::vec3 up = {0.0f,0.0f,this->radius};
    const std::size_t vCount = std::max(this->subdivisions,(std::size_t)2)+1;
    const std::size_t uCount = std::max(this->subdivisions,(std::size_t)2)*2;
    const double dPhi = pi / (double)(vCount-1);
    const double dTheta = 2.0 * pi / (double)uCount;
    // Precalculate unit circle coordinates
    double rComp[uCount][2];
    for(std::size_t i=0;i<uCount;++i){
        rComp[i][0] = -std::sin(dTheta * (double)i);
        rComp[i][1] = std::cos(dTheta * (double)i);
    }
    // First point is top
    res.pushPos({this->pos + up, {}});
    // For rendering efficiency, build top half, remember radii and heights, then build bottom half
    double rh[vCount][2];
    for(std::size_t i=1;i<(vCount+1)/2;++i){
        rh[i][0] = this->radius * std::sin(dPhi * (double)i);
        rh[i][1] = this->radius * std::cos(dPhi * (double)i);
        for(std::size_t j=0;j<uCount;++j){
            glm::vec3 dPos = {rComp[j][0] * rh[i][0], rComp[j][1] * rh[i][0], rh[i][1]};
            res.pushPos({this->pos + dPos, {}});
        }
    }
    for(std::size_t i=1;i<(vCount-1)/2;++i){
        for(std::size_t j=0;j<uCount;++j){
            glm::vec3 dPos = {rComp[j][0] * rh[i][0], rComp[j][1] * rh[i][0], -rh[i][1]};
            res.pushPos({this->pos + dPos, {}});
        }
    }
    res.pushPos({this->pos - up, {}});
    // Start with top fan
    CAD::geometry::Graph::Face tempFace;
    tempFace.p[0] = 0;
    for(std::size_t u=0;u<uCount;++u){
        tempFace.p[1] = (u+1)%(uCount)+1;
        tempFace.p[2] = u+1;
        res.pushFace(tempFace);
    }
    std::size_t offset = 1;
    for(std::size_t v=0;v<vCount-3;++v){
        for(std::size_t u=0;u<uCount;++u){
            tempFace.p[0] = offset+u;
            tempFace.p[1] = offset+(u+1)%(uCount);
            tempFace.p[2] = offset+uCount+u;
            res.pushFace(tempFace);
            tempFace.p[0] = offset+(u+1)%(uCount);
            tempFace.p[1] = offset+uCount+(u+1)%(uCount);
            tempFace.p[2] = offset+uCount+u;
            res.pushFace(tempFace);
        }
        offset += uCount;
    }
    tempFace.p[0] = uCount * (vCount-2)+1;
    for(std::size_t u=0;u<uCount;++u){
        tempFace.p[1] = tempFace.p[0]-((u+1)%(uCount)+1);
        tempFace.p[2] = tempFace.p[0]-(u+1);
        res.pushFace(tempFace);
    }

    /*res.pushPos({{-0.5f,-0.5f, 0.0f}});
    res.pushPos({{-0.5f, 0.5f, 0.0f}});
    res.pushPos({{ 0.5f, 0.5f, 0.0f}});
    res.pushPos({{ 0.5f,-0.5f, 0.0f}});

    res.pushFace({0,1,2});
    res.pushFace({0,2,3});*/

    // glm::vec3 up = {0.0f, this->radius, 0.0f};
    // const std::size_t v = std::max(this->subdivisions,(std::size_t)3);
    // const double dphi = pi / (double)v;
    // const std::size_t u = std::max(this->subdivisions,(std::size_t)3)*2;
    // const double dtheta = 2.0 * pi / (double)u;

    // res.pushPos({this->pos + up,{}});

    // glm::vec3 ring[u] = {};
    // for(int i=0;i<u;++i){
    //     ring[i] = this->radius*glm::vec3{std::cos((double)i * dtheta), 0, std::sin((double)i * dtheta)};
    // }

    // {   // Vertices
    //     for(std::size_t j=1;j<v-1;++j){
    //         float y = this->radius*std::cos(dphi * j);
    //         float r = std::sin(dphi * j);
    //         for(std::size_t i=0;i<u;++i){
    //             res.pushPos({this->pos + (ring[i] * r) + glm::vec3{0,y,0},{}});
    //         }
    //     }
    // }

    // { // Indices
    //     // Top row
    //     for(uint32_t i=0;i<u;++i){
    //         res.pushFace({0,i+1,((i+1)%u)+1});
    //     }
    // }

    // res.pushPos({this->pos - up,{}});
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