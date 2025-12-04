#include <iostream>

#include "CAD/geometry/primative.hpp"

int main(){
    using namespace CAD;
    using namespace geometry;

    Sphere s{2.0f, 1.0f, 1.25f, 1.5f, 3};
    Graph g = s.generateGraph();
}