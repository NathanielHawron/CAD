#pragma once

#include <vector>

#include "CAD/general/vec.hpp"
#include "CAD/general/interpolator.hpp"

namespace CAD{
    namespace geometry{
        class Segment{
        public:
            Segment(){};
            ~Segment() = default;
            virtual general::vec2 sample(const std::vector<general::vec2> &points, double t) const = 0;
        };
        // Straight line between two points
        class Edge : public Segment{
        public:
            std::size_t startIndex, endIndex;
            Edge(std::size_t startIndex, std::size_t endIndex):startIndex{startIndex},endIndex{endIndex}{};
            general::vec2 sample(const std::vector<general::vec2> &points, double t) const override;
        };
        // Arc with a center, radius, and start/end angles wrt +x axis CCW
        class CenterArc : public Segment{
        public:
            std::size_t centerIndex;
            double radius, theta0, theta1;
            CenterArc(std::size_t centerIndex, double radius, double theta0, double theta1):centerIndex{centerIndex},radius{radius},theta0{theta0},theta1{theta1}{};
            general::vec2 sample(const std::vector<general::vec2> &points, double t) const override;
        };
    }
}