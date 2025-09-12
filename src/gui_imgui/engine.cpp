#include "CAD/gui/engine.hpp"

#include "imgui/imgui.h"

using namespace CAD;
using namespace gui;

void EngineGUI::cliWindow(){
    ImGui::Begin("Engine CLI window");
    ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("Engine CLI window history",{size.x,size.y*0.9f});
    for(std::string s : this->promptHistory){
        ImGui::Text(s.c_str());
    }
    ImGui::EndChild();
    if(ImGui::InputText("Engine CLI prompt",this->promptBuffer,this->promptSize,ImGuiInputTextFlags_EnterReturnsTrue)){
        std::string temp = std::string(this->promptBuffer);
        this->promptHistory.add(temp);
        memset(this->promptBuffer, '\0', this->promptSize);
    }
    ImGui::End();
}
