#include "CAD/gui/engine.hpp"

#include "CAD/general/version.hpp"

#include "imgui/imgui.h"


using namespace CAD;
using namespace gui;

EngineGUI::EngineGUI(std::string name, std::string id, int width, int height, std::array<NRA::VGL::ControlBind, 17> controls, std::size_t promptSize, std::size_t promptHistoryCount):
Engine(name,promptSize,promptHistoryCount),
id{id},
width{width},
height{height},
controls{controls}{
}

void EngineGUI::viewportWindow(general::Viewport &vp){
    std::string title = vp.name + "###VPW" + this->id+" "+vp.getId();
    ImGui::Begin(title.c_str());

    ImGui::End();
}

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

void EngineGUI::renderWindowMenu(){
    std::string label = this->name+"##"+this->id;
    std::string childRenderCLI = this->name+" cli##"+this->id;
    ImGui::BeginGroup();
    if(this->rename){
        if(ImGui::InputText(label.c_str(),this->renameBuff,64,ImGuiInputTextFlags_EnterReturnsTrue)){
            this->rename = false;
            this->name = std::string(this->renameBuff);
        }
        ImGui::SameLine();
        if(ImGui::Button(("cancel##"+this->id).c_str())){
            this->rename = false;
        }
    }else{
        if(ImGui::Button((this->name+"##"+this->id).c_str())){
            this->renameVPIndex = -1;
            this->rename = true;
            memset(this->renameBuff,'\0',64);
            memcpy(this->renameBuff,this->name.c_str(),this->name.size());
        }
    }
    ImGui::Checkbox(childRenderCLI.c_str(),&this->renderWindowCLI);
    int index = 0;
    for(auto &vp : this->viewports){
        if(index != this->renameVPIndex){
            std::string vpLabel = vp.name + "###VPCB" + this->id+" "+vp.getId();
            ImGui::Checkbox(vpLabel.c_str(),&vp.visible);
            ImGui::SameLine();
            if(ImGui::Button(("rename##"+this->id+" "+vp.getId()).c_str())){
                this->rename = false;
                this->renameVPIndex = index;
                memset(this->renameBuff,'\0',64);
                memcpy(this->renameBuff,vp.name.c_str(),vp.name.size());
            }
            ImGui::SameLine();
            if(ImGui::Button(("delete##"+this->id+vp.getId()).c_str())){
                this->renameVPIndex = -1;
            }
        }else{
            std::string vpLabel = "###VPCB" + this->id+" "+vp.getId();
            std::string renameLabel = "###VPRN" + this->id+" "+vp.getId();
            ImGui::Checkbox(vpLabel.c_str(),&vp.visible);
            ImGui::SameLine();
            if(ImGui::InputText(renameLabel.c_str(),this->renameBuff,64,ImGuiInputTextFlags_EnterReturnsTrue)){
                vp.name = std::string(this->renameBuff);
                this->renameVPIndex = -1;
            }
            ImGui::SameLine();
            if(ImGui::Button(("cancel##"+this->id+" "+vp.getId()).c_str())){
                this->renameVPIndex = -1;
            }
        }
        ++index;
    }
    if(ImGui::Button(("+##"+this->id).c_str())){
        this->viewports.emplace_back(std::string{"Viewport "}+std::to_string(this->viewports.size()),std::to_string(this->viewportID++),this->controls,this->width,this->height);
    }
    ImGui::EndGroup();
}

void EngineGUI::cliWindow(){
    ImGui::Begin((this->name+" CLI window###CLI"+this->id).c_str());
    ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::BeginChild((this->name+" CLI window history###"+this->id).c_str(),{size.x,size.y*0.9f});
    ImGui::NewLine();
    for(std::vector<std::pair<Color,std::string>> line : this->promptHistory){
        for(std::pair<Color, std::string> s : line){
            ImGui::SameLine();
            ImGui::TextColored(ImColor{s.first.r,s.first.g,s.first.b},s.second.c_str());
        }
        ImGui::NewLine();
    }
    ImGui::EndChild();
    if(ImGui::InputText((this->name+" CLI prompt").c_str(),this->promptBuffer,this->promptSize,ImGuiInputTextFlags_EnterReturnsTrue)){
        std::string temp = std::string(this->promptBuffer);
        auto coloredPrompt = this->parseColors(temp);
        this->promptHistory.push(coloredPrompt);
        this->cliCommand(this->filterColors(temp));
        memset(this->promptBuffer, '\0', this->promptSize);
    }
    std::string promptSizeCount = std::to_string(std::string(this->promptBuffer).size()) + "/" + std::to_string(this->promptSize-1);
    ImGui::SameLine();
    ImGui::TextColored(ImColor{0.5f,0.5f,0.5f,1.0f},promptSizeCount.c_str());
    ImGui::End();
}
