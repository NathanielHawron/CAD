#pragma once

#include "glm/glm.hpp"

#include <string>
#include <memory>
#include <cstdint>

#include "CAD/geometry/transform.hpp"

namespace CAD{
    namespace geometry{
        class Sphere;
        class Graph;
        struct Volume{
            ~Volume() = default;

            virtual std::string toString() const {return "(string representation NYI)";};

            // Checks if the point is inside the volume
            virtual bool inside(glm::vec3 point) = 0;
            // Calculates the point where the ray, starting from the origin, intersects the volume
            virtual glm::vec3 project(glm::vec3 orgin, glm::vec3 ray) = 0;

            // Generate a graph for the volume
            virtual Graph generateGraph() = 0;

            // Generate a graph that is the union of this and other (no duplicate volumes)
            virtual Graph unionGraph(Volume *other) = 0;
            // Generate a graph that is the difference between this and other, where other is subtracted from this
            virtual Graph differenceGraph(Volume *other) = 0;
            // Generate a graph that is the intersection of this and other
            virtual Graph intersectionGraph(Volume *other) = 0;

            // Generate a graph with transformed vertices
            virtual Graph transform(geometry::Transform t) = 0;

            // Check if volumes intersect (usefull to early kill or simplify operations)
            virtual bool intersects(Volume *other) = 0;
            // Check if other is completely inside or coincident
            virtual bool encloses(Volume *other) = 0;
            // Check if completely inside other or coincident
            virtual bool enclosed(Volume *other) = 0;
        };
    }
}