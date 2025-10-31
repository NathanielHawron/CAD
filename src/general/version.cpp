#include "CAD/general/version.hpp"


constexpr CAD::Version CAD::LIB_VERSION{0,0,10,'a'};

std::string CAD::Version::toString() const{
    return "v" + std::to_string(this->MAJOR) + "." + std::to_string(this->MINOR) + "." + std::to_string(this->PATCH) + "-" + this->TAG;
}
bool CAD::Version::operator==(const CAD::Version &v) const {
    return this->MAJOR == v.MAJOR && this->MINOR == v.MINOR && this->PATCH == v.PATCH && this->TAG == v.TAG;
}