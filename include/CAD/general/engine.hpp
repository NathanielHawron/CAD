#pragma once

#include <vector>
#include <list>
#include <string>
#include <unordered_map>

#include "NRA_visionGL/camera.h"
#include "NRA_visionGL/controlCamera.h"
#include "NRA_visionGL/frameBufferObject.h"
#include "NRA_visionGL/mesh.h"

#include "CAD/geometry/sketch.hpp"
#include "CAD/general/ringbuffer.hpp"


namespace CAD{
    namespace general{
        class Viewport{
        public:
            bool visible;
            bool ortho = false;
            std::string name;
            NRA::VGL::ProjectionParams projectionParams;
        private:
            std::string id;
            NRA::VGL::CameraOrbit camera;
            NRA::VGL::FBO_flexible canvas;
        public:
            Viewport(std::string name, std::string id, std::array<NRA::VGL::ControlBind,17> &controls, int width, int height);
            std::string getId()const{return this->id;};
        };
        struct vertex{
            GLfloat pos[3];
            GLfloat norm[3];
            GLfloat color[3];
            GLfloat tex[2];
        };
        using sketchID = std::size_t;
        class Engine{
        public:
            struct Color{
                float r, g, b;
            };
            static Color COLOR_ERROR;
            static Color COLOR_WARNING;
            static Color COLOR_INFO;
        public:
            std::string name;
            bool renderWindowCLI = false;
        protected:
            std::list<Viewport> viewports;

            std::size_t promptSize;
            char *promptBuffer;
            general::RingBuffer<std::vector<std::pair<Color, std::string>>> promptHistory;

            NRA::VGL::Mesh<GLuint> mesh;

            std::unordered_map<std::size_t, geometry::Sketch> sketches;
            sketchID nextSketchID;
        public:
            Engine(std::string name, std::size_t promptSize = 255, std::size_t promptHistoryCount = 100);
            ~Engine();
            virtual void renderWindowMenu();
            void renderWindows();
            virtual void cliWindow();
            virtual void viewportWindow(Viewport &vp);
            static void aboutWindow();
            void cliCommand(std::string command);
            Color parseColorString(std::string str, Color defaultColor = {1.0f,1.0f,1.0f});
            std::vector<std::pair<Color, std::string>> parseColors(std::string str);
            std::string filterColors(std::string str);
            void resizePromptBuffer(std::size_t newSize);
            void resizePromptHistory(std::size_t newSize);

            // Sketch functions
            inline sketchID addSketch(){sketchID res = this->nextSketchID++;this->sketches.insert({res,geometry::Sketch{}});return res;};
            inline geometry::Sketch *getSketch(sketchID id){return &this->sketches.at(id);};
            inline void removeSketch(sketchID id){this->sketches.erase(id);};
        };
    }
}