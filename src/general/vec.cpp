#include "CAD/general/vec.hpp"

#include <cstdint>
#include <cmath>

using namespace CAD;
using namespace general;


// vec2 scalar operators
template <class T>
vec2T<T> vec2T<T>::operator*(T s) const {
    return {this->x * s, this->y * s};
}
template <class T>
vec2T<T> vec2T<T>::operator/(T s) const {
    return {this->x / s, this->y / s};
}

template <class T>
vec2T<T>& vec2T<T>::operator*=(T s) {
    this->x *= s;
    this->y *= s;
    return *this;
}
template <class T>
vec2T<T>& vec2T<T>::operator/=(T s) {
    this->x /= s;
    this->y /= s;
    return *this;
}

// vec2 vector operators
template <class T>
vec2T<T> vec2T<T>::operator+(const vec2T<T> &other) const {
    return {this->x+other.x, this->y+other.y};
}
template <class T>
vec2T<T> vec2T<T>::operator-(const vec2T<T> &other) const {
    return {this->x-other.x, this->y-other.y};
}
template <class T>
vec2T<T> vec2T<T>::operator*(const vec2T<T> &other) const {
    return {this->x*other.x, this->y*other.y};
}
template <class T>
vec2T<T> vec2T<T>::operator/(const vec2T<T> &other) const {
    return {this->x/other.x, this->y/other.y};
}

template <class T>
vec2T<T>& vec2T<T>::operator+=(const vec2T<T> &other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
}
template <class T>
vec2T<T>& vec2T<T>::operator-=(const vec2T<T> &other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}
template <class T>
vec2T<T>& vec2T<T>::operator*=(const vec2T<T> &other) {
    this->x *= other.x;
    this->y *= other.y;
    return *this;
}
template <class T>
vec2T<T>& vec2T<T>::operator/=(const vec2T<T> &other) {
    this->x /= other.x;
    this->y /= other.y;
    return *this;
}

// vec2 other operations
template <class T>
T vec2T<T>::cross(const vec2T<T> &other) const {
    return this->x*other.y - this->y*other.x;
}
template <class T>
vec2T<T> vec2T<T>::rotate(T r) const {
    T sin = std::sin(r);
    T cos = std::cos(r);
    return {this->x * cos - this->y * sin, this->x * sin + this->y * cos};
}
template <class T>
vec2T<T> vec2T<T>::normalized() const {
    T mag = this->mag();
    T imag = mag == 0 ? 0.0 : 1.0 / mag;
    return {this->x*imag, this->y*imag};
}


// vec3 scalar operators
template <class T>
vec3T<T> vec3T<T>::operator*(T s) const {
    return {this->x * s, this->y * s, this->z * s};
}
template <class T>
vec3T<T> vec3T<T>::operator/(T s) const {
    return {this->x / s, this->y / s, this->z / s};
}

template <class T>
vec3T<T>& vec3T<T>::operator*=(T s) {
    this->x *= s;
    this->y *= s;
    this->z *= s;
    return *this;
}
template <class T>
vec3T<T>& vec3T<T>::operator/=(T s) {
    this->x /= s;
    this->y /= s;
    this->z /= s;
    return *this;
}

// vec3 vector operators
template <class T>
vec3T<T> vec3T<T>::operator+(const vec3T<T> &other) const {
    return {this->x+other.x, this->y+other.y, this->z+other.z};
}
template <class T>
vec3T<T> vec3T<T>::operator-(const vec3T<T> &other) const {
    return {this->x-other.x, this->y-other.y, this->z-other.z};
}
template <class T>
vec3T<T> vec3T<T>::operator*(const vec3T<T> &other) const {
    return {this->x*other.x, this->y*other.y, this->z*other.z};
}
template <class T>
vec3T<T> vec3T<T>::operator/(const vec3T<T> &other) const {
    return {this->x/other.x, this->y/other.y, this->z/other.z};
}

template <class T>
vec3T<T>& vec3T<T>::operator+=(const vec3T<T> &other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}
template <class T>
vec3T<T>& vec3T<T>::operator-=(const vec3T<T> &other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}
template <class T>
vec3T<T>& vec3T<T>::operator*=(const vec3T<T> &other) {
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    return *this;
}
template <class T>
vec3T<T>& vec3T<T>::operator/=(const vec3T<T> &other) {
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    return *this;
}

// vec3 other operations
template <class T>
vec3T<T> vec3T<T>::cross(const vec3T<T> &other) const {
    return {
        this->y*other.z - this->z*other.y,
        this->z*other.x - this->x*other.z,
        this->x*other.y - this->y*other.x
    };
}
template <class T>
vec3T<T> vec3T<T>::normalized() const {
    T mag = this->mag();
    T imag = mag == 0 ? 0.0 : 1.0 / mag;
    return {this->x*imag, this->y*imag, this->z*imag};
}

template class vec2T<float>;
template class vec3T<float>;

template class vec2T<double>;
template class vec3T<double>;