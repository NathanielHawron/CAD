#include "CAD/gui/engine.hpp"

#include "CAD/general/version.hpp"

#include "imgui/imgui.h"


using namespace CAD;
using namespace gui;

EngineGUI::EngineGUI(std::string name, std::string id, int width, int height, std::array<NRA::VGL::ControlBind, 17> controls, NRA::VGL::Shader &shader, std::size_t promptSize, std::size_t consoleSize, std::size_t promptHistoryCount, std::size_t subdivisions):
Engine(name,promptSize,consoleSize,promptHistoryCount,subdivisions),
id{id},
width{width},
height{height},
controls{controls},
shader{shader}{
}

void EngineGUI::viewportWindow(general::Viewport &vp){
    if(this->renderable.renderReady){
        vp.render(this->renderable);
    }
    std::string title = vp.name + "###VPW" + this->id+" "+vp.getId();
    ImGui::Begin(title.c_str());
    vp.focus = ImGui::IsWindowHovered();

    if(this->renderable.renderReady){
        ImVec2 imageSize = ImVec2(vp.getWidth(), vp.getHeight());
        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        ImVec2 dSize{imageSize.x-windowSize.x,imageSize.y-windowSize.y};
        ImVec2 rdSize = {dSize.x / imageSize.x, dSize.y / imageSize.y};
        ImGui::Image(
            (ImTextureID)vp.getTex(),
            windowSize,
            ImVec2(rdSize.x*0.5f,1.0f-rdSize.y*0.5f),
            ImVec2(1.0f-rdSize.x*0.5f,rdSize.y*0.5f)
        );
    }else{
        ImGui::TextColored(ImVec4{1.0f,0.5f,0.5f,1.0f}, "No mesh to render");
    }

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
    // Render engine name / rename options
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
        ImGui::SameLine();
        if(ImGui::Button(("close##"+this->id).c_str())){
            this->shouldClose = true;
        }
        if(ImGui::Button(("rebuild mesh##"+this->id).c_str())){
            this->generateMesh();
        }
    }
    // CLI window option
    ImGui::Checkbox(childRenderCLI.c_str(),&this->renderWindowCLI);
    // Render viewport menu
    int index = 0;
    auto it = this->viewports.begin();
    while(it != this->viewports.end()){
        general::Viewport &vp = *it;
        bool inc = true;
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
                it = this->viewports.erase(it);
                inc = false;
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
        if(inc){
            ++it;
        }
    }
    if(ImGui::Button(("Add Viewport##"+this->id).c_str())){
        this->addViewport(this->controls, this->width, this->height, this->shader);
    }
    ImGui::EndGroup();
}

template <class T>
struct buffs{
    general::RingBuffer<T> &rb;
    general::RingBufferIterator<T> &it;
    const std::size_t &size;
    char *buff;
    std::string &current;
};
int CLIKeyCallback(ImGuiInputTextCallbackData *data){
    buffs<std::string> *b = (buffs<std::string>*)data->UserData;
    if(data->EventKey == ImGuiKey_UpArrow){
        if(b->it == b->rb.end()){
            b->current = std::string(b->buff);
        }else{
            (*b->it) = std::string(b->buff);
        }
        if(b->it != b->rb.begin()){
            --b->it;
            memset(b->buff,'\0',b->size);
            memcpy(b->buff,(*b->it).c_str(),std::min(b->size,(*b->it).size()));
        }
    }else if(data->EventKey == ImGuiKey_DownArrow){
        if(b->it != b->rb.end()){
            (*b->it) = std::string(b->buff);
            ++b->it;
            memset(b->buff,'\0',b->size);
            if(b->it == b->rb.end()){
                memcpy(b->buff,b->current.c_str(),std::min(b->size,b->current.size()));
            }else{
                memcpy(b->buff,(*b->it).c_str(),std::min(b->size,(*b->it).size()));
            }
        }
    }
    data->DeleteChars(0,data->BufTextLen);
    data->InsertChars(0,b->buff);
    return 0;
}

void EngineGUI::cliWindow(){
    ImGui::Begin((this->name+" CLI window###CLI"+this->id).c_str());
    ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::BeginChild((this->name+" CLI window history###"+this->id).c_str(),{size.x,size.y*0.9f});
    ImGui::NewLine();
    for(std::vector<std::pair<Color,std::string>> line : this->console){
        for(std::pair<Color, std::string> s : line){
            ImGui::SameLine();
            ImGui::TextColored(ImColor{s.first.r,s.first.g,s.first.b},s.second.c_str());
        }
        ImGui::NewLine();
    }
    ImGui::EndChild();
    static std::string current = "";
    buffs<std::string> b = {
        this->promptHistory,
        this->promptHistoryIndex,
        this->promptSize,
        this->promptBuffer,
        current
    };
    if(ImGui::InputText((this->name+" CLI prompt").c_str(),this->promptBuffer,this->promptSize,ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory, CLIKeyCallback, &b)){
        std::string temp = std::string(this->promptBuffer);
        this->console.push_back(this->parseColors(temp));
        this->cliCommand(this->filterColors(temp));
        memset(this->promptBuffer, '\0', this->promptSize);
        ImGui::SetKeyboardFocusHere(-1);
    }
    std::string promptSizeCount = std::to_string(std::string(this->promptBuffer).size()) + "/" + std::to_string(this->promptSize-1);
    ImGui::SameLine();
    ImGui::TextColored(ImColor{0.5f,0.5f,0.5f,1.0f},promptSizeCount.c_str());
    ImGui::End();
}
