#include <iostream>
#include <memory>
#include <assert.h>

#include "CAD/general/version.hpp"
#include "CAD/geometry/sketch.hpp"
#include "CAD/language/object.hpp"

const CAD::Version testVersion{0,0,0,'a'};

using Obj_t = CAD::language::Object<CAD::geometry::Sketch, std::shared_ptr>;

int main(){
    std::cout << testVersion.toString() << std::endl;
    std::cout << CAD::LIB_VERSION.toString() << " (lib) : " << CAD::HEADER_VERSION.toString() << " (header) / v0.0.1-a (tested)" << std::endl;

    
    Obj_t test{Obj_t::FLAG_MODIFIER_NONE};
    std::string str = test;
    std::cout << str << std::endl;
    assert(str == "UNDEFINED");

    std::shared_ptr<Obj_t::FlaggedValue> nullVal = std::make_shared<Obj_t::FlaggedValue>(nullptr, Obj_t::FLAG_DATA_NULL);
    assert(test.assign(nullVal, Obj_t::FLAG_MODIFIER_NONE, Obj_t::FLAG_ASSIGNMENT_NONE));
    str = test;
    std::cout << str << std::endl;
    assert(str == "NULL");

    std::shared_ptr<Obj_t::FlaggedValue> testVal = std::make_shared<Obj_t::FlaggedValue>(std::make_unique<CAD::geometry::Sketch>(), Obj_t::FLAG_DATA_NONE);
    assert(test.assign(testVal, Obj_t::FLAG_MODIFIER_NONE, Obj_t::FLAG_ASSIGNMENT_NONE));
    str = test;
    std::cout << str << std::endl;
    assert(str == "Sketch with 0 points and 0 segments");
    
    testVal->value->addPoint({0.0,0.0});
    testVal->value->addPoint({1.0,1.0});
    testVal->value->addSegment(std::make_unique<CAD::geometry::Edge>(0,1));
    str = test;
    std::cout << str << std::endl;
    assert(str == "Sketch with 2 points and 1 segments");
}