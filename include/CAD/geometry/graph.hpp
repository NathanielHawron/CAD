#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "NRA_visionGL/mesh.h"
#include "NRA_visionGL/vertexBufferLayout.h"

#include "CAD/geometry/volume.hpp"

namespace CAD{
    namespace geometry{
        class Graph : public Volume {
        public:
            struct Face{
                std::size_t p[3];
            };
            struct Position{
                glm::vec3 pos;
                glm::vec3 color;
            };
            struct Vertex{
                static const NRA::VGL::VertexBufferLayout layout;
                GLfloat pos[3];
            };
        private:
            std::vector<Position> positions;
            std::vector<Face> faces;
            bool faceIntersectsLine(std::size_t f0, std::size_t p0, std::size_t p1);
            bool faceIntersectsFace(std::size_t f0, std::size_t f1);
        public:
            ~Graph() = default;
            template <class I>
            void addToMesh(NRA::VGL::Mesh<I> &mesh) const;

            bool inside(glm::vec3 point) override;
            glm::vec3 project(glm::vec3 origin, glm::vec3 ray) override;
            
            Graph generateGraph() override;
            Graph unionGraph(Volume *other) override;
            Graph differenceGraph(Volume *other) override;
            Graph intersectionGraph(Volume *other) override;
        };
    }
}