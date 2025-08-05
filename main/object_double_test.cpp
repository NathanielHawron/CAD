#include <iostream>
#include <memory>
#include <assert.h>

#include "CAD/general/version.hpp"
#include "CAD/language/object.hpp"

const CAD::Version testVersion{0,0,0,'a'};

using Obj_t = CAD::language::Object<double, std::shared_ptr>;

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

    std::shared_ptr<Obj_t::FlaggedValue> testVal = std::make_shared<Obj_t::FlaggedValue>(1.0, Obj_t::FLAG_DATA_NONE);
    assert(test.assign(testVal, Obj_t::FLAG_MODIFIER_NONE, Obj_t::FLAG_ASSIGNMENT_CLONE));
    str = test;
    std::cout << str << std::endl;
    assert(str == std::to_string(1.0));

    assert(test.assign(nullVal, Obj_t::FLAG_MODIFIER_NONE, Obj_t::FLAG_ASSIGNMENT_CONDITIONAL_RIGHT));
    str = test;
    std::cout << str << std::endl;
    assert(str == std::to_string(1.0));

    *testVal->value = 2.0;
    assert(test.assign(testVal, Obj_t::FLAG_MODIFIER_NONE, Obj_t::FLAG_ASSIGNMENT_CONDITIONAL_LEFT));
    str = test;
    std::cout << str << std::endl;
    assert(str == std::to_string(1.0));

    assert(test.assign(testVal, Obj_t::FLAG_MODIFIER_NONE, Obj_t::FLAG_ASSIGNMENT_NONE));
    *testVal->value = 3.0;
    str = test;
    std::cout << str << std::endl;
    assert(str == std::to_string(3.0));
}