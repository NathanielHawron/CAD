#include "CAD/gui/engine.hpp"

#include "imgui/imgui.h"


using namespace CAD;
using namespace gui;


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
        this->cliCommand(temp);
        memset(this->promptBuffer, '\0', this->promptSize);

    }
    ImGui::End();
}
