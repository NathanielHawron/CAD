#pragma once

#include <array>

#include "CAD/geometry/boundingBox.hpp"
#include "glad/gl.h"


namespace CAD{
    namespace geometry{
        namespace visualizer{
            template <class T>
            class BoundingBoxVisualizer{
                private:
                    enum class MESH_STATE : uint_fast8_t{
                        UNINITIALIZED,
                        EMPTY,
                        FULL
                    };
                    struct vertex{
                        T x, y, z;
                    };
                    BoundingBox::BB<T> &bb;
                    union{struct{GLuint vbo, ibo;} bo; GLuint id[2];} buffers;
                    GLuint vao;
                    MESH_STATE meshState = MESH_STATE::UNINITIALIZED;
                    vertex *vertices = nullptr;
                    GLuint *indices = nullptr;
                    std::size_t vertexCount = 0, indexCount = 0;
                public:
                    BoundingBoxVisualizer(BoundingBox::BB<T> &bb);
                    void bindBuffers();
                    void generateMesh();
                    bool uploadMesh();
                    void render();
                private:
                    bool isMeshEmpty();
                    void createMesh(std::size_t vertexCount, std::size_t indexCount);
                    void deleteMesh();
                    void generateMesh(BoundingBox::Box<T> *box);
                    void generateMesh(BoundingBox::Sphere<T> *sphere);
            };
        }
    }
}