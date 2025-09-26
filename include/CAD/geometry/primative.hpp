#pragma once

#include <memory>

#include "glm/glm.hpp"

#include "CAD/geometry/volume.hpp"

namespace CAD{
    namespace geometry{
        struct Primative : public Volume{
            ~Primative() = default;
        };
        struct Sphere : public Primative{
            glm::vec3 pos;
            float radius;

            bool inside(glm::vec3 point) override;
            glm::vec3 project(glm::vec3 origin, glm::vec3 ray) override;

            Graph generateGraph() override;
            Graph unionGraph(Volume *other) override;
            Graph differenceGraph(Volume *other) override;
            Graph intersectionGraph(Volume *other) override;
        };
    }
}