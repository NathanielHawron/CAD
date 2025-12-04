#pragma once

#include <vector>
#include <queue>
#include <list>
#include <string>
#include <unordered_map>
#include <thread>

#include "NRA_visionGL/camera.h"
#include "NRA_visionGL/controlCamera.h"
#include "NRA_visionGL/frameBufferObject.h"
#include "NRA_visionGL/mesh.h"
#include "NRA_visionGL/shader.h"
#include "NRA_visionGL/renderable.h"

#include "CAD/general/ringbuffer.hpp"
#include "CAD/geometry/sketch.hpp"
#include "CAD/geometry/graph.hpp"
#include "CAD/geometry/primative.hpp"
#include "CAD/geometry/transform.hpp"


namespace CAD{
    namespace general{
        class Viewport{
        public:
            bool visible = false;
            bool ortho = false;
            bool focus = false;
            std::string name;
            NRA::VGL::ProjectionParams projectionParams;
        private:
            std::string id;
            NRA::VGL::CameraOrbit camera;
            NRA::VGL::FBO_flexible canvas;
            NRA::VGL::Shader &shader;
        public:
            Viewport(std::string name, std::string id, std::array<NRA::VGL::ControlBind,17> &controls, int width, int height, NRA::VGL::Shader &shader);
            std::string getId()const{return this->id;};
            void render(NRA::VGL::Renderable &r);
            inline int getWidth(){return this->canvas.getWidth();};
            inline int getHeight(){return this->canvas.getHeight();};
            inline unsigned int getTex(){return this->canvas.getTex();};
            inline unsigned int getTex(int8_t index){return this->canvas.getTex(index);};
            inline void control(NRA::VGL::Controls &controls){this->camera.control(controls);};
        };
        using csgID = std::size_t;
        using sketchID = std::size_t;
        using sphereID = std::size_t;
        using transformID = std::size_t;
        class Engine{
        public:
            struct Color{
                float r, g, b;
            };
            static std::array<Color, 2> COLORS_ERROR;
            static std::array<Color, 2> COLORS_WARNING;
            static std::array<Color, 2> COLORS_INFO;
            static std::array<Color, 2> COLORS_SUCCESS;
            enum class VolumeType : uint8_t{
                NONE = 0, CSG, TRANSFORM,
                // Primatives
                SPHERE
            };
            static const std::array<std::string, 4> VolumeType_string;
            struct CSG{
                enum class OP : uint8_t{
                    // Note: Transform must have b type of TRANSFORM
                    TRANSFORM = 0,
                    UNION, DIFFERENCE, INTERSECTION
                };
                static const std::array<std::string,4> OP_string;
                OP op;
                VolumeType aType;
                std::size_t aIndex;
                VolumeType bType;
                std::size_t bIndex;
                
                glm::mat4 transform = glm::mat4(1.0f);
                
                std::string toString() const;
            };
        public:
            std::string name;
            bool renderWindowCLI = false;
            bool shouldClose = false;
            protected:
            std::list<Viewport> viewports;
            std::size_t nextViewportID = 0;
            
            std::size_t promptSize;
            char *promptBuffer;
            general::RingBuffer<std::vector<std::pair<Color, std::string>>> console;
            general::RingBuffer<std::string> promptHistory;
            general::RingBufferIterator<std::string> promptHistoryIndex;
            
            NRA::VGL::Mesh<GLuint> *mesh;
            NRA::VGL::Renderable renderable;
            
            std::unordered_map<csgID, CSG> csgOperations;
            csgID nextcsgID = 0;
            
            std::unordered_map<transformID, geometry::Transform> transforms;
            transformID nextTransformID = 0;
            
            std::unordered_map<sketchID, geometry::Sketch> sketches;
            sketchID nextSketchID = 0;
            
            std::unordered_map<sketchID, geometry::Sphere> spheres;
            sphereID nextSphereID = 0;
            std::size_t subdivisions;

        private:
            static std::queue<std::string> cli_cin;
            static std::thread *cli_cin_listener;
            static bool cli_cout_en;
            static bool cli_cin_en;
            
        public:
            Engine(std::string name, std::size_t promptSize = 256, std::size_t consoleSize = 100, std::size_t promptHistoryCount = 50, std::size_t subdivisions = 10);
            ~Engine();
            static void startCin();
            static void stopCin();
            static inline bool queryCinEn(){return Engine::cli_cin_en;};
            static inline std::size_t queryCinSize(){return Engine::cli_cin.size();};
            void cinCliCommand();
            static inline void nextCommand(){if(!Engine::cli_cin.empty()){Engine::cli_cin.pop();}};
            // Checks equivelance based on pointer address
            bool operator==(const Engine &e){return this == &e;};
            virtual void renderWindowMenu();
            void renderWindows(NRA::VGL::Controls &controls);
            virtual void cliWindow();
            virtual void viewportWindow(Viewport &vp);
            static void aboutWindow();
            void cliCommand(std::string command);
            Color parseColorString(std::string str, Color defaultColor = {1.0f,1.0f,1.0f});
            std::vector<std::pair<Color, std::string>> parseColors(std::string str);
            std::string filterColors(std::string str);
            void resizePromptBuffer(std::size_t newSize);
            void resizeConsole(std::size_t newSize);
            void resizePromptHistory(std::size_t newSize);

            void addViewport(std::array<NRA::VGL::ControlBind, 17> controls, int width, int height, NRA::VGL::Shader &shader);

            void generateMesh();

            // Sketch functions
            inline sketchID addSketch(){sketchID res = this->nextSketchID++;this->sketches.insert({res,geometry::Sketch{}});return res;};
            inline geometry::Sketch *getSketch(sketchID id){return &this->sketches.at(id);};
            inline void removeSketch(sketchID id){this->sketches.erase(id);};

            // CSG functions
            inline csgID addCSG(CSG csg){
                this->csgOperations.insert({this->nextcsgID, csg});
                return this->nextcsgID++;
            }
            inline transformID addTransform(geometry::Transform t){
                this->transforms.insert({this->nextTransformID, t});
                return this->nextTransformID++;
            }

            // Primative functions
            inline sphereID addSphere(float radius, float x, float y, float z){
                this->spheres[this->nextSphereID] = geometry::Sphere{radius, x, y, z, this->subdivisions};
                return this->nextSphereID++;
            }
        private:
            void cliCommandHelp(std::queue<std::string> &promptComponents);
            void cliCommandMesh(std::queue<std::string> &promptComponents);
            void cliCommandSet(std::queue<std::string> &promptComponents);
            void cliCommandGet(std::queue<std::string> &promptComponents);
            void cliCommandSkip(std::queue<std::string> &promptComponents);
            void cliCommandCSG(std::queue<std::string> &promptComponents, CSG::OP op);
            void cliCommandCSGList(std::queue<std::string> &promptComponents);
            void cliCommandTransform(std::queue<std::string> &promptComponents);
            void cliCommandSphere(std::queue<std::string> &promptComponents);
        };
    }
}