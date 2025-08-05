#include "CAD/geometry/sketch.hpp"

using namespace CAD;
using namespace geometry;

Sketch Sketch::centerRectangle(general::vec2 center, general::vec2 size, double angle){
    Sketch res;

    double sin = std::sin(angle);
    double cos = std::cos(angle);
    general::vec2 ref = {size.x*cos + size.y*sin, size.y*cos - size.x*sin};
    ref *= 0.5;

    res.addPoint(center + ref);
    res.addPoint(center + general::vec2{ref.y, -ref.x});
    res.addPoint(center - ref);
    res.addPoint(center - general::vec2{ref.y, -ref.x});

    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{0,1}));
    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{1,2}));
    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{2,3}));
    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{3,0}));

    return res;
}
Sketch Sketch::cornerRectangle(general::vec2 corner1, general::vec2 corner2){
    Sketch res;

    res.addPoint(corner1);
    res.addPoint(general::vec2{corner2.x, corner1.y});
    res.addPoint(corner2);
    res.addPoint(general::vec2{corner1.x, corner2.y});

    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{0,1}));
    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{1,2}));
    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{2,3}));
    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{3,0}));

    return res;
}
Sketch Sketch::cornerRectangle(general::vec2 corner1, general::vec2 corner2, general::vec2 point1){
    Sketch res;

    auto deltaC = corner2-corner1;
    auto deltaP = point1-corner1;
    auto proj = deltaC * deltaP.dot(deltaC) / deltaC.magSq();
    auto perp = deltaP - proj;
    
    res.addPoint(corner1);
    res.addPoint(corner2);
    res.addPoint(corner2 + perp);
    res.addPoint(corner1 + perp);

    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{0,1}));
    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{1,2}));
    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{2,3}));
    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{3,0}));

    return res;
}
Sketch Sketch::edgeHeightRectangle(general::vec2 corner1, general::vec2 corner2, double height){
    Sketch res;

    auto delta = corner2-corner1;
    auto perp = general::vec2{delta.y, -delta.x}.normalized() * height;
    
    res.addPoint(corner1);
    res.addPoint(corner2);
    res.addPoint(corner2 + perp);
    res.addPoint(corner1 + perp);

    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{0,1}));
    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{1,2}));
    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{2,3}));
    res.addSegment(std::make_unique<geometry::Edge>(geometry::Edge{3,0}));

    return res;
}

Sketch::operator std::string(){
    return "Sketch with " + std::to_string(this->points.size()) + " points and " + std::to_string(this->segments.size()) + " segments";
}

void Sketch::addPoint(general::vec2 point){
    this->points.push_back(point);
}
void Sketch::addSegment(std::unique_ptr<Segment> seg){
    this->segments.push_back(std::move(seg));
}
void Sketch::tagPoint(std::string tag, std::size_t index){
    this->pointTags.at(tag).insert(index);
}
void Sketch::tagSegment(std::string tag, std::size_t index){
    this->segmentTags.at(tag).insert(index);
}