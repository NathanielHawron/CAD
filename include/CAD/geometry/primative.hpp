#pragma once

#include <memory>
#include <string>

#include "glm/glm.hpp"

#include "CAD/geometry/volume.hpp"
#include "CAD/geometry/graph.hpp"

namespace CAD{
    namespace geometry{
        struct Primative : public Volume{
            ~Primative() = default;
        };
        struct Sphere : public Primative{
            float radius;
            glm::vec3 pos;

            Sphere():radius{0.0f},pos{0.0f,0.0f,0.0f}{};
            Sphere(float radius, float x, float y, float z):radius{radius},pos{x,y,z}{};
            ~Sphere() = default;

            std::string toString() const override;

            bool inside(glm::vec3 point) override;
            glm::vec3 project(glm::vec3 origin, glm::vec3 ray) override;

            Graph generateGraph() override;
            Graph unionGraph(Volume *other) override;
            Graph differenceGraph(Volume *other) override;
            Graph intersectionGraph(Volume *other) override;
        };
    }
}