#pragma once

#include <string>

namespace CAD{
    namespace general{
        template <class T, int D>
        struct mat{};

        template <class T, int D>
        struct vec{
            // Scalar operations
            virtual vec operator*(T &s);
            virtual vec operator/(T &s);
            // Vector operations
            virtual vec operator+(vec &other);
            virtual vec operator-(vec &other);
            virtual vec operator*(vec &other);
            virtual vec cross(vec &other);
            virtual T mag();
            virtual vec normalize();
            virtual vec scale(T s);
            virtual vec rotateX(T r);
            virtual vec rotateY(T r);
            virtual vec rotateZ(T r);
            virtual vec rotateQuat(T r, T i, T j, T k);
            // Matrix operations
            virtual vec operator*(mat<T,D> &m);
            // Helper
            virtual operator std::string()const;
            virtual operator std::array<T,D>()const;
        };
    };
};