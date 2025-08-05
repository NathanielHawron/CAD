#include <iostream>
#include <cassert>

#include "CAD/general/version.hpp"
#include "CAD/general/vec.hpp"

const CAD::Version testVersion{0,0,0,'a'};

int main(){
    std::cout << testVersion.toString() << std::endl;
    std::cout << CAD::LIB_VERSION.toString() << " (lib) : " << CAD::HEADER_VERSION.toString() << " (header) / v0.0.1-a (tested)" << std::endl;

    using namespace CAD;

    general::vec2 v20;
    general::vec3 v30;

    general::vec2 v21{1.1, 1.2};
    general::vec3 v31{{-1.1, -1.2, -1.3}};

    
    std::cout << (std::string)v20 << ", " << (std::string)v21 << " | " << (std::string)v30 << ", " << (std::string)v31 << std::endl;

    v20 = {1.0,2.0};
    v21 *= 2;

    
    v30 = v31 * 3;
    v31 *= 4;

    assert((v20 == general::vec2{1.0,2.0}));
    assert((v21 == general::vec2{2.2,2.4}));

    std::cout << (std::string)v20 << ", " << (std::string)v21 << " | " << (std::string)v30 << ", " << (std::string)v31 << std::endl;
    
    assert((v30 == general::vec3{-1.1*3.0,-1.2*3.0,-1.3*3.0}));
    assert((v31 == general::vec3{-4.4,-4.8,-5.2}));

    std::cout << v20.dot(v21) << ", " << v30.dot(v31) << std::endl;
    assert(v20.dot(v21) == (2.2+4.8));
    assert(v30.dot(v31) == (4.4*(1.1*3.0)+4.8*(1.2*3.0)+5.2*(1.3*3.0)));
    std::cout << v20.cross(v21) << ", " << (std::string)v30.cross(v31) << std::endl;
    assert(v20.cross(v21) == (2.4-4.4));
    assert((std::abs(v30.cross(v31).mag()) <= 0.000001));
}