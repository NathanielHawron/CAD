#pragma once

#include <string>

namespace CAD{
    namespace general{
        template <class T, int D>
        struct vecT;

        template <class T, int D>
        struct matT{
            std::array<std::array<T,D>,D> mat;
            /*// Scalar operations
            virtual mat operator*(T s)&;
            virtual mat operator/(T s)&;
            // Vector operations
            virtual mat operator+(mat &other);
            virtual mat operator-(mat &other);
            virtual mat operator*(mat &other);
            virtual mat translate(std::array<T,D-1> d);
            virtual mat rotateX(T r);
            virtual mat rotateY(T r);
            virtual mat rotateZ(T r);
            virtual mat rotateQuat(T r, T i, T j, T k);
            virtual mat scale(std::array<T,D-1> s);
            virtual mat conj();
            // Matrix operations
            virtual vec<T,D> operator*(mat<T,D> &m);
            // Helper
            virtual operator std::string()const;
            virtual operator std::array<T,D>()const;*/
        };
    };
};