#include "CAD/geometry/boundingBox.hpp"

#include <sstream>
#include <algorithm>

using namespace CAD;
using namespace geometry;
using namespace BoundingBox;

// Constructors
template <class T>
Box<T>::Box(T x, T y, T z, T dx, T dy, T dz):
x{x},y{y},z{z},dx{dx},dy{dy},dz{dz}{}
template <class T>
Box<T>::Box(std::array<T, 3> pos, std::array<T, 3> size):
x{pos[0]},y{pos[1]},z{pos[2]},dx{size[0]},dy{size[1]},dz{size[2]}{}

template <class T>
Sphere<T>::Sphere(T x, T y, T z, T r):
x{x},y{y},z{z},r{r}{}
template <class T>
Sphere<T>::Sphere(std::array<T, 3> pos, T r):
x{pos[0]},y{pos[1]},z{pos[2]},r{r}{}


// Methods
template <class T>
bool BB<T>::intersects(const BB<T> &bb) const{
    return bb.intersects(*this);
}
template <class T>
bool Box<T>::intersects(const BB<T> &bb) const {
    return bb.intersects(*this);
}
template <class T>
bool Box<T>::intersects(const Box<T> &b) const {
    return abs(this->x - b.x) <= (dx/2) && abs(this->y - b.y) <= (dy/2) && abs(this->z - b.z) <= (dz/2);
}
template <class T>
bool Box<T>::intersects(const Sphere<T> &s) const {
    T minx = this->x - this->dx/2;
    T maxx = this->x + this->dx/2;
    T miny = this->y - this->dy/2;
    T maxy = this->y + this->dy/2;
    T minz = this->z - this->dz/2;
    T maxz = this->z + this->dz/2;
    T nx = std::clamp(s.x, minx, maxx);
    T ny = std::clamp(s.y, miny, maxy);
    T nz = std::clamp(s.z, minz, maxz);
    T dx = abs(nx-s.x), dy = abs(ny-s.y), dz = abs(nz-s.z);
    return (dx*dx + dy*dy + dz*dz) <= (s.r*s.r);
}

template <class T>
bool Sphere<T>::intersects(const BB<T> &bb) const {
    return bb.intersects(*this);
}
template <class T>
bool Sphere<T>::intersects(const Box<T> &b) const {
    return b.intersects(*this);
}
template <class T>
bool Sphere<T>::intersects(const Sphere<T> &s) const {
    T dx = this->x-s.x, dy = this->y-s.y, dz = this->z - s.z;
    T dmax = this->r + s.r;
    return (dx*dx+dy*dy+dz*dz) <= (dmax*dmax);
}

template <class T>
Box<T>::operator std::string() const{
    std::stringstream ss;
    ss << "Bounding Box: @(" << this->x << ", " << this->y << ", " << this->z << ") Size (" << this->dx << ", " << this->dy << ", " << this->dz << ")";
    return ss.str();
}
template <class T>
Sphere<T>::operator std::string() const{
    std::stringstream ss;
    ss << "Bounding Sphere: @(" << this->x << ", " << this->y << ", " << this->z << ") Size (" << this->r << ")";
    return ss.str();
}

// Template instantiation
template class BB<float>;
template class Box<float>;
template class Sphere<float>;