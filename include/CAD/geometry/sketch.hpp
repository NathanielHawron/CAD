#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "CAD/general/vec.hpp"
#include "CAD/geometry/segment.hpp"

namespace CAD{
    namespace geometry{
        class Sketch{
        private:
            std::vector<general::vec2> points;
            std::vector<std::unique_ptr<Segment>> segments;
            std::unordered_map<std::string, std::unordered_set<std::size_t>> pointTags;
            std::unordered_map<std::string, std::unordered_set<std::size_t>> segmentTags;

        public:
            // Generates a rectangle centered around center.
            // The vertices follow quadrant order (++, -+, --, +-).
            static Sketch centerRectangle(general::vec2 center, general::vec2 size, double angle);
            // Generates an axis aligned rectangle between both corners.
            // The vertices are ordered corner1, corner2.x corner1.y, corner2, corner1.x corner2.y
            static Sketch cornerRectangle(general::vec2 corner1, general::vec2 corner2);
            // Generates a rectangle by drawing an edge between corner1 and corner2, then makes a parallel line through point1.
            // The vertices are ordered corner1, corner2, adjacent to corner2, adjacent to corner1.
            static Sketch cornerRectangle(general::vec2 corner1, general::vec2 corner2, general::vec2 point1);
            // Generates a rectangle by drawing an edge between corner1 and corner2, then scales a perpendicular vector by height.
            // corner3 is (corner2+perp) and corner4 is (corner1+perp).
            // The vertices are ordered corner1, corner2, corner3, corner4.
            static Sketch edgeHeightRectangle(general::vec2 corner1, general::vec2 corner2, double height);
        public:
            Sketch(){};
            operator std::string();
            void addPoint(general::vec2 point);
            // unique_ptr used for polymorphism, and gets moved.
            void addSegment(std::unique_ptr<geometry::Segment> segment);
            void tagPoint(std::string tag, std::size_t index);
            void tagSegment(std::string tag, std::size_t index);
        };
    }
}