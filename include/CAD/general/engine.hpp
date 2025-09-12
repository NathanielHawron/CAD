#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "NRA_visionGL/camera.h"
#include "NRA_visionGL/controlCamera.h"
#include "NRA_visionGL/frameBufferObject.h"

#include "CAD/geometry/sketch.hpp"

namespace CAD{
    namespace general{
        class Viewport{
        private:
            NRA::VGL::CameraOrbit camera;
            NRA::VGL::FBO_flexible canvas;
            std::string id;
        };
        using sketchID = std::size_t;
        class Engine{
        private:
            std::string name;
            std::vector<Viewport> renderers;
            std::unordered_map<std::size_t, geometry::Sketch> sketches;
            sketchID nextSketchID;
        public:
            Engine(std::string name);
            ~Engine();
            virtual void cliWindow();
            // Sketch functions
            inline sketchID addSketch(){sketchID res = this->nextSketchID++;this->sketches.insert({res,geometry::Sketch{}});return res;};
            inline geometry::Sketch *getSketch(sketchID id){return &this->sketches.at(id);};
            inline void removeSketch(sketchID id){this->sketches.erase(id);};
        };
    }
}