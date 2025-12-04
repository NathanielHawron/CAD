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
                // Move positions to end of array of size o
                inline Face operator+(std::size_t o){return Face{{p[0]+o,p[1]+o,p[2]+o}};};
                // Remove o positions from start of array
                inline Face operator-(std::size_t o){return Face{{p[0]-o,p[1]-o,p[2]-o}};};
            };
            struct Position{
                glm::vec3 pos;
                glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f);
                bool isAlmostEqual(Position p, float e);
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
            inline std::size_t positionOffset(){return this->positions.size();};
            void pushPos(Position p);
            void pushFace(Face f);
            std::pair<bool,std::size_t> findPos(Position p);
            void mergePositions(float e);
            Graph &operator+=(Graph &g);

            bool inside(glm::vec3 point) override;
            glm::vec3 project(glm::vec3 origin, glm::vec3 ray) override;
            
            Graph generateGraph() override;
            
            Graph unionGraph(Volume *other) override;
            Graph differenceGraph(Volume *other) override;
            Graph intersectionGraph(Volume *other) override;
            Graph transform(geometry::Transform t) override;
            bool intersects(Volume *other) override;
            bool encloses(Volume *other) override;
            bool enclosed(Volume *other) override;
        };
    }
}