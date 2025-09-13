#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "NRA_visionGL/camera.h"
#include "NRA_visionGL/controlCamera.h"
#include "NRA_visionGL/frameBufferObject.h"

#include "CAD/geometry/sketch.hpp"
#include "CAD/general/ringbuffer.hpp"


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
        public:
            struct Color{
                float r, g, b;
            };
        protected:
            std::string name;
            std::vector<Viewport> renderers;

            std::size_t promptSize;
            char *promptBuffer;
            general::RingBuffer<std::vector<std::pair<Color, std::string>>> promptHistory;

            std::unordered_map<std::size_t, geometry::Sketch> sketches;
            sketchID nextSketchID;
        public:
            Engine(std::string name, std::size_t promptSize = 255, std::size_t promptHistoryCount = 100);
            ~Engine();
            virtual void cliWindow();
            void cliCommand(std::string command);
            Color parseColorString(std::string str, Color defaultColor = {1.0f,1.0f,1.0f});
            std::vector<std::pair<Color, std::string>> parseColors(std::string str);

            // Sketch functions
            inline sketchID addSketch(){sketchID res = this->nextSketchID++;this->sketches.insert({res,geometry::Sketch{}});return res;};
            inline geometry::Sketch *getSketch(sketchID id){return &this->sketches.at(id);};
            inline void removeSketch(sketchID id){this->sketches.erase(id);};
        };
    }
}