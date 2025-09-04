#include <string>
#include <iostream>

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

#include "TextEditor.h"

const int worldWidth = 10, worldHeight = 10;

namespace TestControls{
    // First block (0 - 31):    Movement
    const NRA::VGL::ControlBind forwards =      {false, 0};
    const NRA::VGL::ControlBind backwards =     {false, 1};
    const NRA::VGL::ControlBind left =          {false, 2};
    const NRA::VGL::ControlBind right =         {false, 3};
    const NRA::VGL::ControlBind up =            {false, 4};
    const NRA::VGL::ControlBind down =          {false, 5};
    // Second block (32 - 63):  Admin
    const NRA::VGL::ControlBind pause =         {false, 32};
};

int main(){
    std::cout << "NRA_visionGL test v" << (std::string)NRA_visionGL_VERSION << std::endl;

    NRA::VGL::Window::init();

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    std::vector<NRA::VGL::ControlsInit> controlsList;
    {
        using namespace NRA;
        using namespace VGL;
        using namespace TestControls;
        controlsList.emplace_back(ControlsInit{ButtonType::KEY,GLFW_KEY_W,          forwards});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY,GLFW_KEY_S,          backwards});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY,GLFW_KEY_A,          left});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY,GLFW_KEY_D,          right});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY,GLFW_KEY_SPACE,      up});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY,GLFW_KEY_LEFT_SHIFT, down});
        controlsList.emplace_back(ControlsInit{ButtonType::KEY,GLFW_KEY_ESCAPE,     pause});
    }

    NRA::VGL::Window window(800,800,"NRA vision GL test",controlsList);
    NRA::VGL::FBO minimapFBO(400,400);
    NRA::VGL::Controls &controls = window.getControls();
    window.makeCurrent();
    window.swapInterval(1);

    // Setup ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // OpenGL options
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glClearColor(0.0f, 0.5f, 0.8f, 1.0f);

    std::filesystem::path shaderPath = std::filesystem::current_path().append("res/shaders/");
    std::filesystem::path vertexPath = shaderPath;
    std::filesystem::path fragmentPath = shaderPath;
    vertexPath.append("shaded_texture/shaded_texture.vertex");
    fragmentPath.append("shaded_texture/shaded_texture.fragment");

    NRA::VGL::Shader shader(vertexPath,fragmentPath);

    std::filesystem::path minimapVertexPath = shaderPath;
    std::filesystem::path minimapFragmentPath = shaderPath;
    minimapVertexPath.append("texture/texture.vertex");
    minimapFragmentPath.append("texture/texture.fragment");

    NRA::VGL::Shader minimapShader(minimapVertexPath,minimapFragmentPath);

    NRA::VGL::Renderable renderable;
    NRA::VGL::Renderable minimap;
    renderable.init();
    minimap.init();
    NRA::VGL::VertexBufferLayout layout;

    layout.push(GL_FLOAT,3);
    layout.push(GL_FLOAT,2);
    layout.push(GL_FLOAT,3);

    renderable.setVBOLayout(layout);
    minimap.setVBOLayout(layout);

    typedef NRA::VGL::Mesh<GLuint> Mesh_t;
    struct vertex{
        GLfloat pos[3];
        GLfloat tex[2];
        GLfloat norm[3];
    };

    vertex positions[3*2*worldWidth*worldHeight];
    for(int x=0;x<worldWidth;++x){
        for(int y=0;y<worldHeight;++y){
            int i = x*worldHeight + y;
            positions[i*6] =   {{(float)x-4.5f       ,std::sinf(x-4.5f)*std::sin(y-4.5f), (float)y-4.5f},       {0.0f, 0.0}, {0,1,0}};
            positions[i*6+1] = {{(float)x+1.0f-4.5f  ,std::sinf(x-3.5f)*std::sin(y-4.5f), (float)y-4.5f},       {1.0f, 0.0f},{0,1,0}};
            positions[i*6+2] = {{(float)x+0.5f-4.5f  ,std::sinf(x-4.0f)*std::sin(y-3.5f), (float)y+1.0f-4.5f},  {0.5f, 0.5f},{0,1,0}};

            positions[i*6+3] = {{(float)x+1.5f-4.5f  ,std::sinf(x-3.0f)*std::sin(y-3.5f), (float)y+1.0f-4.5f},  {1.0f,1.0f}, {0,1,0}};
            positions[i*6+4] = {{(float)x+0.5f-4.5f  ,std::sinf(x-4.0f)*std::sin(y-3.5f), (float)y+1.0f-4.5f},  {0.0f,1.0f}, {0,1,0}};
            positions[i*6+5] = {{(float)x+1.0f-4.5f  ,std::sinf(x-3.5f)*std::sin(y-4.5f), (float)y-4.5f},       {0.5f,0.5f}, {0,1,0}};
        }
    }
    
    GLuint indices[3*2*worldWidth*worldHeight];
    for(int i=0;i<3*2*worldWidth*worldHeight;++i){
        indices[i] = i;
    }

    vertex minimapPositions[4] = {
        {{-0.5f, -0.5f, 0.0f},{0.0f,0.0f}, {0,0,1.0f}},
        {{-0.5f,  0.5f, 0.0f},{0.0f,1.0f}, {0,0,1.0f}},
        {{ 0.5f,  0.5f, 0.0f},{1.0f,1.0f}, {0,0,1.0f}},
        {{ 0.5f, -0.5f, 0.0f},{1.0f,0.0f}, {0,0,1.0f}}
    };
    GLuint minimapIndices[6] = {
        0,1,2,
        0,2,3
    };

    Mesh_t mesh = Mesh_t(sizeof(vertex)/4);
    mesh.add(positions,indices,3*2*worldWidth*worldHeight,3*2*worldWidth*worldHeight);
    Mesh_t minimapMesh = Mesh_t(sizeof(vertex)/4);
    minimapMesh.add(minimapPositions,minimapIndices,4,6);

    renderable.loadVertexData(mesh.getVertices(), mesh.getVertexCount()*sizeof(vertex));
    renderable.loadIndexData(mesh.getIndices(), mesh.getIndexCount());
    minimap.loadVertexData(minimapPositions, 4*sizeof(vertex));
    minimap.loadIndexData(minimapIndices, 6);

    NRA::VGL::SpacialBase cameraPos({0.0f,0.0f,10.0f},glm::quat(glm::vec3(0.0f,0.0f,0.0f)));
    NRA::VGL::ProjectionParams projectionParams = {window.getAspect(), NRA::VGL::ProjectionParams::horizontalFOV(90.0f,window.getAspect())};
    NRA::VGL::Camera camera(glm::vec3(), glm::quat(), projectionParams, &cameraPos);

    NRA::VGL::SpacialBase minimapCameraPos({0.0f,10.0f,0.0f},glm::quat(glm::vec3(-1.570796f,0.0f,0.0f)));
    NRA::VGL::ProjectionParams minimapProjectionParams = {window.getAspect(), NRA::VGL::ProjectionParams::horizontalFOV(90.0f,window.getAspect())};
    NRA::VGL::Camera minimapCamera(glm::vec3(), glm::quat(), projectionParams, &minimapCameraPos);

    int time = 0;

    glm::mat4 modelMat = glm::mat4(1.0f);
    glm::mat4 vpMat;
    glm::mat4 minimapvpMat;
    glm::mat4 guiMat = glm::mat4(1.0f);
    glm::mat4 minimapPosMat = glm::translate(glm::scale(glm::mat4(1.0f),glm::vec3(0.5,0.5,0.5)),glm::vec3(1.25f,1.25f,1.25f));

    controls.setSensitivity(0.01);

    NRA::VGL::Image worldImage(2,2);
    worldImage.buffer[0] = 255;
    worldImage.buffer[1] = 0;
    worldImage.buffer[2] = 0;
    worldImage.buffer[3] = 255;

    worldImage.buffer[4] = 0;
    worldImage.buffer[5] = 255;
    worldImage.buffer[6] = 0;
    worldImage.buffer[7] = 255;

    worldImage.buffer[8] = 0;
    worldImage.buffer[9] = 0;
    worldImage.buffer[10] = 255;
    worldImage.buffer[11] = 255;

    worldImage.buffer[12] = 0;
    worldImage.buffer[13] = 0;
    worldImage.buffer[14] = 0;
    worldImage.buffer[15] = 255;

    NRA::VGL::Texture worldTexture(worldImage);

    TextEditor textEditor;

    while(!(window.shouldClose())){
        ++time;
        // Window update calls glfwPollEvents()
        NRA::VGL::Window::update();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            glm::mat4 ref{1.0f};
            camera.transform(ref);
            glm::vec3 up =          glm::vec3(0,1,0);
            glm::vec3 forwards =    glm::vec3(0,0,-1);
            glm::vec3 right =       glm::normalize(glm::cross(up, forwards * glm::mat3(ref)));
            forwards = glm::cross(up, right);
            if(controls.queryControl(TestControls::forwards)){
                cameraPos.moveA(forwards * -0.1f);
            }
            if(controls.queryControl(TestControls::backwards)){
                cameraPos.moveA(forwards * 0.1f);
            }
            if(controls.queryControl(TestControls::left)){
                cameraPos.moveL({-0.1f,0.0f,0.0f});
            }
            if(controls.queryControl(TestControls::right)){
                cameraPos.moveL({0.1f,0.0f,0.0f});
            }
            if(controls.queryControl(TestControls::up)){
                cameraPos.moveA({0.0f,0.1f,0.0f});
            }
            if(controls.queryControl(TestControls::down)){
                cameraPos.moveA({0.0f,-0.1f,0.0f});
            }
            if(window.getIsPointerLocked()){
                auto dpos = controls.getDMousePos();
                cameraPos.rotateL(glm::vec3(1,0,0), dpos.second);
                cameraPos.rotateA(glm::vec3(0,-1,0), dpos.first);
            }
        }
        if(controls.queryControl(TestControls::pause)){
            if(window.getIsPointerLocked()){
                window.unlockPointer();
            }else{
                window.lockPointer();
            }
            controls.unsetControl(TestControls::pause);
        }
        
        if(window.getAspectChanged()){
            projectionParams.aspect = window.getAspect();
            camera.updateProjectionMatrix(projectionParams);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vpMat = glm::mat4(1.0f);
        minimapvpMat = glm::mat4(1.0f);
        camera.transformP(vpMat);
        minimapCamera.transformP(minimapvpMat);

        // Render minimap POV
        minimapFBO.bind();
        glClearColor(1.0f, 0.5f, 0.1f, 1.0f);
        worldTexture.bind(0);
        shader.bind();
        shader.setUniform1("u_Texture", 0);
        shader.setUniformMat<4>("U_vpMat", &minimapvpMat[0][0]);
        shader.setUniformMat<4>("U_mMat", &modelMat[0][0]);
        renderable.bindBuffers();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderable.render();
        minimapFBO.unbind();

        glViewport(0,0,800,800);
        glClearColor(0.0f, 0.5f, 0.8f, 1.0f);

        ImGui::BeginMainMenuBar();
        bool menuTest;
        ImGui::MenuItem("Menu Test Item", "", &menuTest);
        ImGui::EndMainMenuBar();

        ImGui::Begin("Code Editor", nullptr, ImGuiWindowFlags_MenuBar);

        ImGui::BeginMenuBar();
        ImGui::MenuItem("Save");
        ImGui::MenuItem("Load");
        ImGui::EndMenuBar();

        ImVec2 editorSize = ImGui::GetContentRegionAvail();
        textEditor.Render("Editor", editorSize);
        ImGui::End();

        ImGui::Begin("Model");
        ImVec2 imageSize = ImVec2(minimapFBO.getWidth(), minimapFBO.getHeight());
        ImGui::Image(
            (ImTextureID)minimapFBO.getTEX(),
            imageSize,
            ImVec2(0,1),
            ImVec2(1,0)
        );
        ImGui::End();

        // Render world
        worldTexture.bind(0);
        shader.bind();
        shader.setUniformMat<4>("U_vpMat", &vpMat[0][0]);
        shader.setUniformMat<4>("U_mMat", &modelMat[0][0]);
        renderable.bindBuffers();
        renderable.render();

        /*// Render minimap
        minimapFBO.bindTexture(0);
        minimapShader.bind();
        minimapShader.setUniformMat<4>("U_vpMat", &guiMat[0][0]);
        minimapShader.setUniformMat<4>("U_mMat", &minimapPosMat[0][0]);
        minimap.bindBuffers();
        minimap.render();*/

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBuffer();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}