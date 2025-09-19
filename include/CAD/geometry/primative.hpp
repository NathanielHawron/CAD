#pragma once

#include <memory>

#include "glm/glm.hpp"

namespace CAD{
    namespace geometry{
        struct Primative{
            ~Primative() = default;
            virtual std::unique_ptr<Primative> uniqueClone() const = 0;
        };
        struct Sphere : public Primative{
            glm::vec3 pos;
            float radius;
            std::unique_ptr<Primative> uniqueClone() const override{
                std::unique_ptr<Sphere> res = std::make_unique<Sphere>();
                res->pos = this->pos;
                res->radius = this->radius;
                return res;
            }
        };
    }
}