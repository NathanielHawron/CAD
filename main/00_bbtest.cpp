#include "CAD/general/args.hpp"
#include "CAD/geometry/boundingBox.hpp"

#include <iostream>

using namespace CAD;
using namespace geometry;

void testBB(BoundingBox::BB<float> &a, BoundingBox::BB<float> &b, bool expected){
    std::cout << (std::string)a << " : " << (std::string)b << std::endl;
    std::cout << a.intersects(b) << "/" << expected << std::endl;
}

int main(){
    BoundingBox::Box<float> a{1.0f,1.0f,1.0f,       0.5f,0.5f,0.5f};
    BoundingBox::Sphere<float> b{1.0f,1.0f,1.0f,    1.0f};

    testBB(a, a, 1);
    testBB(b, b, 1);
    testBB(a, b, 1);
    testBB(b, a, 1);

    b.x = 2.5;
    testBB(a,b,1);
    b.y = 2.5;
    testBB(b,a,0);
}