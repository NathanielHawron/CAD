#include "CAD/geometry/segment.hpp"

using namespace CAD;
using namespace geometry;

general::vec2 Edge::sample(const std::vector<general::vec2> &points, double t) const {
    const general::vec2 &startPoint = points.at(this->startIndex);
    const general::vec2 &endPoint = points.at(this->endIndex);
    return startPoint * (1.0-t) + endPoint * t;
}
general::vec2 CenterArc::sample(const std::vector<general::vec2> &points, double t) const {
    const general::vec2 &centerPoint = points.at(this->centerIndex);
    double theta = this->theta0 * (1.0-t) + this->theta1 * t;
    const general::vec2 radius = general::vec2{std::cos(theta), std::sin(theta)} * this->radius;
    return centerPoint + radius;
}