#pragma once

#include "CAD/general/vec.hpp"

#include <array>

namespace CAD{
    namespace geometry{
        namespace BoundingBox{
            template <class T = float>
            class Sphere;
            template <class T = float>
            class Box;
            
            template <class T = float>
            class BB{
            public:
                virtual bool intersects(const BB &bb) const = 0;
                virtual bool intersects(const Box<T> &bb) const = 0;
                virtual bool intersects(const Sphere<T> &bb) const = 0;
                virtual operator std::string() const = 0;
            };
            template <class T>
            class Box : public BB<T>{
            public:
                T x, y, z;
                T dx, dy, dz;
            public:
                Box(T x, T y, T z, T dx, T dy, T dz);
                Box(std::array<T, 3> pos, std::array<T,3> size);
                bool intersects(const BB<T> &bb) const override;
                bool intersects(const Box<T> &b) const;
                bool intersects(const Sphere<T> &s) const;
                operator std::string() const;
            };
            template <class T>
            class Sphere : public BB<T>{
            public:
                T x, y, z;
                T r;
            public:
                Sphere(T x, T y, T z, T r);
                Sphere(std::array<T, 3> pos, T r);
                bool intersects(const BB<T> &bb) const override;
                bool intersects(const Box<T> &b) const;
                bool intersects(const Sphere<T> &s) const;
                operator std::string() const;
            };
        }
    }
};