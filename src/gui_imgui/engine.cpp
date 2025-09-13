#include "CAD/gui/engine.hpp"

#include "CAD/general/version.hpp"

#include "imgui/imgui.h"


using namespace CAD;
using namespace gui;


void EngineGUI::aboutWindow(){
    std::string version = "CAD " + (std::string)CAD::LIB_VERSION;
    std::string credits = "Libraries used:\n\
Window:             glfw.org\n\
GUI:                https://github.com/ocornut/imgui\n\
OpenGL bindings:    https://github.com/Dav1dde/glad\n\
Math:               https://github.com/g-truc/glm\n\
Licenses are included in the source code";

    ImGui::Begin("About");
    
    ImGui::Text(version.c_str());
    ImGui::Text(credits.c_str());
    ImGui::End();
}

void EngineGUI::cliWindow(){
    ImGui::Begin("Engine CLI window");
    ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("Engine CLI window history",{size.x,size.y*0.9f});
    ImGui::NewLine();
    for(std::vector<std::pair<Color,std::string>> line : this->promptHistory){
        for(std::pair<Color, std::string> s : line){
            ImGui::SameLine();
            ImGui::TextColored(ImColor{s.first.r,s.first.g,s.first.b},s.second.c_str());
        }
        ImGui::NewLine();
    }
    ImGui::EndChild();
    if(ImGui::InputText("Engine CLI prompt",this->promptBuffer,this->promptSize,ImGuiInputTextFlags_EnterReturnsTrue)){
        std::string temp = std::string(this->promptBuffer);
        auto coloredPrompt = this->parseColors(temp);
        this->promptHistory.add(coloredPrompt);
        this->cliCommand(this->filterColors(temp));
        std::cout << this->filterColors(temp) << std::endl;
        memset(this->promptBuffer, '\0', this->promptSize);
    }
    std::string promptSizeCount = std::to_string(std::string(this->promptBuffer).size()) + "/" + std::to_string(this->promptSize-1);
    ImGui::SameLine();
    ImGui::TextColored(ImColor{0.5f,0.5f,0.5f,1.0f},promptSizeCount.c_str());
    ImGui::End();
}
