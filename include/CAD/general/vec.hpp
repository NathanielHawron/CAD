#pragma once

#include <string>
#include <array>
#include <cstdint>
#include <cmath>

namespace CAD{
    namespace general{
        template <class T, int D>
        struct matT;

        template <class T, int D>
        struct vecT{
            // Member
            std::array<T, D> vec;

            // Member aliases
            T &x, &y;
            T &u, &v;

            // Constructor
            vecT(std::array<T, D> vec):vec{vec},x{this->vec[0]},y{this->vec[1]},u{this->vec[0]},v{this->vec[1]}{};
            virtual ~vecT() = default;

            // Boolean operators
            bool operator==(const vecT &other) const {
                for(uint_fast8_t i=0;i<D;++i){
                    if(this->vec[i] != other.vec[i]){
                        return false;
                    }
                }
                return true;
            }

            // Other operations
            T dot(const vecT &other) const {
                T res = 0;
                for(uint_fast8_t i=0;i<D;++i){
                    res += this->vec[i] * other.vec[i];
                }
                return res;
            }
            T magSq() const {
                return this->dot(*this);
            }
            T mag() const {
                return std::sqrt(this->magSq());
            }
            T normalize(){
                T mag = this->mag();
                T imag = mag == 0.0 ? 0.0 : 1/mag;
                for(uint_fast8_t i=0;i<D;++i){
                    this->vec[i] *= mag;
                }
                return mag;
            }

            // Helper
            operator std::string() const {
                std::string res = "(";
                for(uint_fast8_t i=0;i<D;++i){
                    res+= std::to_string(this->vec[i]);
                    if(i+1<D){
                        res += ", ";
                    }
                }
                res += ")";
                return res;
            }
            operator std::array<T,D>() const {return this->vec;};
        };
        template <class T>
        struct vec2T : public vecT<T, 2>{
            // Constructor
            vec2T():vecT<T,2>{{0,0}}{};
            vec2T(std::array<T,2> v):vecT<T,2>{v}{};
            vec2T(T x, T y):vecT<T,2>{{x,y}}{};
            vec2T& operator=(const vec2T<T>& v){this->vec = v.vec;return *this;};
            
            // Scalar operations
            vec2T operator*(T s)const;
            vec2T operator/(T s)const;
            vec2T& operator*=(T s);
            vec2T& operator/=(T s);
            
            // Vector (element wise) operations
            vec2T operator+(const vec2T &other) const;
            vec2T operator-(const vec2T &other) const;
            vec2T operator*(const vec2T &other) const;
            vec2T operator/(const vec2T &other) const;
            
            vec2T& operator+=(const vec2T &other);
            vec2T& operator-=(const vec2T &other);
            vec2T& operator*=(const vec2T &other);
            vec2T& operator/=(const vec2T &other);

            // Other operations
            T cross(const vec2T &other) const;
            vec2T rotate(T r) const;
            vec2T normalized() const;
            // Matrix operations
            // vecT operator*(mat<T,D> &m)const;
        };
        template <class T>
        struct vec3T : public vecT<T, 3>{
            // Member aliases
            T &z;
            T &w;
            T &r, &g, &b;

            // Constructor
            vec3T():vecT<T,3>{{0,0,0}},z{this->vec[2]},w{this->vec[2]},r{this->vec[0]},g{this->vec[1]},b{this->vec[2]}{};
            vec3T(std::array<T,3> v):vecT<T,3>{v},z{this->vec[2]},w{this->vec[2]},r{this->vec[0]},g{this->vec[1]},b{this->vec[2]}{};
            vec3T(T x, T y, T z):vecT<T,3>{{x,y,z}},z{this->vec[2]},w{this->vec[2]},r{this->vec[0]},g{this->vec[1]},b{this->vec[2]}{};
            vec3T& operator=(const vec3T& v){this->vec = v.vec; return *this;};

            // Scalar operations
            vec3T operator*(T s)const;
            vec3T operator/(T s)const;
            vec3T& operator*=(T s);
            vec3T& operator/=(T s);

            // Vector (element wise) operations
            vec3T operator+(const vec3T &other) const;
            vec3T operator-(const vec3T &other) const;
            vec3T operator*(const vec3T &other) const;
            vec3T operator/(const vec3T &other) const;
            
            vec3T& operator+=(const vec3T &other);
            vec3T& operator-=(const vec3T &other);
            vec3T& operator*=(const vec3T &other);
            vec3T& operator/=(const vec3T &other);

            // Other operations
            vec3T cross(const vec3T &other) const;
            // vecT rotateX(T r)const;
            // vecT rotateY(T r)const;
            // vecT rotateZ(T r)const;
            // vecT rotateQuat(T r, T i, T j, T k)const;
            vec3T normalized() const;
            // Matrix operations
            // vecT operator*(mat<T,D> &m)const;
        };

        using vec2f = vec2T<float>;
        using vec3f = vec3T<float>;
        using vec2 = vec2T<double>;
        using vec3 = vec3T<double>;
    };
};