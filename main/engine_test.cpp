#include <list>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "NRA_visionGL/config.h"
#include "NRA_visionGL/window.h"
#include "NRA_visionGL/shader.h"
#include "NRA_visionGL/renderable.h"
#include "NRA_visionGL/mesh.h"
#include "NRA_visionGL/controls.h"
#include "NRA_visionGL/camera.h"
#include "NRA_visionGL/spacial.h"
#include "NRA_visionGL/texture.h"
#include "NRA_visionGL/frameBufferObject.h"

#include "CAD/gui/engine.hpp"
#include "CAD/general/version.hpp"

namespace TestControls{
    // First block (0 - 31):    Movement
    // WASD + up/down
    const NRA::VGL::ControlBind forwards =      {false, 0};
    const NRA::VGL::ControlBind backwards =     {false, 1};
    const NRA::VGL::ControlBind left =          {false, 2};
    const NRA::VGL::ControlBind right =         {false, 3};
    const NRA::VGL::ControlBind up =            {false, 4};
    const NRA::VGL::ControlBind down =          {false, 5};
    // Arrow keys + QE
    const NRA::VGL::ControlBind pitchUp =       {false, 6};
    const NRA::VGL::ControlBind pitchDown =     {false, 7};
    const NRA::VGL::ControlBind yawLeft =       {false, 8};
    const NRA::VGL::ControlBind yawRight =      {false, 9};
    const NRA::VGL::ControlBind rollLeft =      {};
    const NRA::VGL::ControlBind rollRight =     {};
    // Mouse controls
    const NRA::VGL::ControlBind rotate =        {false, 12};
    const NRA::VGL::ControlBind pan =           {false, 13};
    const NRA::VGL::ControlBind zoom =          {false, 14};
    // Zoom keys
    const NRA::VGL::ControlBind zoomIn =        {false, 15};
    const NRA::VGL::ControlBind zoomOut =       {false, 16};

    // Second block (32 - 63):  Admin
    const NRA::VGL::ControlBind pause =         {false, 32};
};

struct OpenWindows{
    bool cliWindow = false;
};

int main(){
    std::cout << "NRA_visionGL v" << (std::string)NRA_visionGL_VERSION << std::endl;
    std::cout << "CAD " << (std::string)CAD::LIB_VERSION << std::endl;

    NRA::VGL::Window::init();

    std::vector<NRA::VGL::ControlsInit> controlsList;
    {
        using namespace NRA;
        using namespace VGL;
        using namespace TestControls;
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_W,                  forwards});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_S,                  backwards});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_A,                  left});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_D,                  right});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_SPACE,              up});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_LEFT_SHIFT,         down});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_ESCAPE,             pause});

        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_UP,                 pitchUp});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_DOWN,               pitchDown});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_LEFT,               yawLeft});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_RIGHT,              yawRight});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_Q,                  rollLeft});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_E,                  rollRight});

        controlsList.emplace_back(ControlsInit{ButtonType::MOUSE,GLFW_MOUSE_BUTTON_MIDDLE,  rotate});
        controlsList.emplace_back(ControlsInit{ButtonType::MOUSE,GLFW_MOUSE_BUTTON_LEFT,    pan});
        controlsList.emplace_back(ControlsInit{ButtonType::MOUSE,GLFW_MOUSE_BUTTON_RIGHT,   zoom});

        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_Z,                 zoomIn});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY, GLFW_KEY_X,                 zoomOut});
    }
    std::array<NRA::VGL::ControlBind,17> cameraFPVControlBinds = {
        TestControls::forwards,     TestControls::backwards,
        TestControls::left,         TestControls::right,
        TestControls::up,           TestControls::down,
        TestControls::pitchUp,      TestControls::pitchDown,
        TestControls::yawLeft,      TestControls::yawRight,
        TestControls::rollLeft,     TestControls::rollRight,
        TestControls::rotate,       TestControls::pan,          TestControls::zoom,
        TestControls::zoomIn,       TestControls::zoomOut
    };
    NRA::VGL::Window::init();

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    const GLFWvidmode *vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    
    NRA::VGL::Controls controls{controlsList};
    std::string windowTitle = std::string("CAD test ") + (std::string)CAD::LIB_VERSION;
    NRA::VGL::Window window(800,800,windowTitle.c_str(),controls);
    NRA::VGL::FBO minimapFBO(vidMode->width,vidMode->height);
    window.makeCurrent();
    window.swapInterval(1);
    
    // Setup ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    // OpenGL options
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glClearColor(0.2f, 0.5f, 0.6f, 1.0f);
    
    std::list<CAD::gui::EngineGUI> engines;
    std::size_t engineID = 0;
    
    bool showAbout = false;
    
    while(!window.shouldClose()){
        window.reset();
        NRA::VGL::Window::update();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        
        ImGuiWindowFlags mainWindowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar \
        | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize \
        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus \
        | ImGuiWindowFlags_NoNavFocus;
        
        ImGui::Begin("Main Dockspace Window", nullptr, mainWindowFlags);
        ImGuiID dockspaceID = ImGui::GetID("Main Dockspace");
        ImGui::DockSpace(dockspaceID);
        ImGui::End();
        
        ImGui::BeginMainMenuBar();
        if(ImGui::BeginMenu("File")){
            if(ImGui::MenuItem("New",nullptr,false)){
                engines.emplace_back("New Engine "+std::to_string(engineID), std::to_string(engineID++), vidMode->width, vidMode->height, cameraFPVControlBinds);
            }
            // ImGui::MenuItem("Open",nullptr,nullptr,false);
            // ImGui::MenuItem("Save",nullptr,nullptr,false);
            // ImGui::MenuItem("Save As",nullptr,nullptr,false);
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Info")){
            ImGui::MenuItem("Documentation","",nullptr,true);
            if(ImGui::MenuItem("About","",nullptr,true)){
                showAbout = !showAbout;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        ImGui::Begin("Open Files");
        for(auto &engine : engines){
            engine.renderWindowMenu();
        }
        ImGui::End();
        for(auto &engine : engines){
            engine.renderWindows();
        }
        engines.remove_if([](const CAD::gui::EngineGUI &engine){
            return engine.shouldClose;
        });

        if(showAbout){
            CAD::gui::EngineGUI::aboutWindow();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBuffer();
    }
}