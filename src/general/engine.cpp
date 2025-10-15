#include "CAD/general/engine.hpp"

#include <unordered_map>

using namespace CAD;
using namespace general;

std::unordered_map<std::string, std::size_t> commands = {
    {"help",            0},
    {"set",             1},
    {"skip",            2},
    {"union",           3},
    {"difference",      4},
    {"intersection",    5},
    {"transform",       6},
    {"csg",             7},
    {"sphere",          8}
};

Engine::Color Engine::COLOR_ERROR = {1.0f,0.4f,0.4f};
Engine::Color Engine::COLOR_ERROR2 = {1.0f,0.5f,0.5f};
Engine::Color Engine::COLOR_WARNING = {0.85f,0.65f,0.4f};
Engine::Color Engine::COLOR_WARNING2 = {0.9f,0.6f,0.4f};
Engine::Color Engine::COLOR_INFO = {0.4f,0.6f,1.0f};
Engine::Color Engine::COLOR_INFO2 = {0.4f,0.7f,0.9f};

const std::array<std::string,3> Engine::VolumeType_string = {
    "NONE",
    "CSG",
    "SPHERE"
};
const std::array<std::string,4> Engine::CSG::OP_string = {
    "TRANSFORM",
    "UNION",
    "DIFFERENCE",
    "INTERSECTION"
};

Viewport::Viewport(std::string name, std::string id, std::array<NRA::VGL::ControlBind,17> &controls, int width, int height, NRA::VGL::Shader &shader):
name{name},
id{id},
projectionParams{(double)width/(double)height, NRA::VGL::ProjectionParams::horizontalFOV(90.0f, (double)width/(double)height)},
camera{glm::vec3{0.0f,0.0f,0.0f},glm::quat{glm::vec3{0.0f,0.0f,0.0f}},100.0f,this->projectionParams,controls},
canvas{width, height, {NRA::VGL::FBO_flexible::texDescription::COLOR_TEX_2D_DESCRIPTION}, {NRA::VGL::FBO_flexible::rboDescription::DEPTH_STENCIL_RBO_DESCRIPTION}},
shader{shader}{
}
void Viewport::render(NRA::VGL::Renderable &r){
    GLfloat meshColor[4] = {255.0f,0.0f,0.0f,255.0f};
    glm::mat4 vpMat = glm::mat4(1.0f);
    glm::mat4 mMat = glm::mat4(1.0f);//glm::translate(glm::scale(glm::mat4(1.0f),glm::vec3(0.5,0.5,0.5)),glm::vec3(1.25f,1.25f,1.25f));
    this->camera.transformP(vpMat);

    this->canvas.bind();
    this->shader.bind();
    this->shader.setUniform4<GLfloat>("U_Color", meshColor);
    this->shader.setUniformMat<4>("U_vpMat", &vpMat[0][0]);
    this->shader.setUniformMat<4>("U_mMat", &mMat[0][0]);
    glClearColor(0.7f,0.7f,0.75f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    r.bindBuffers();
    r.render();
    this->canvas.unbind();
}

std::string Engine::CSG::toString() const {
    std::string a = VolumeType_string[(uint8_t)this->aType] + " " + std::to_string(this->aIndex);
    if(this->op==OP::TRANSFORM){
        return OP_string.at((uint8_t)this->op) + ": " + a;
    }else{
        std::string b = VolumeType_string[(uint8_t)this->bType] + " " + std::to_string(this->bIndex);
        return OP_string.at((uint8_t)this->op) + ": " + a + (this->op==OP::UNION?"+":(this->op==OP::DIFFERENCE?"-":"*")) + b;
    }
}

Engine::Engine(std::string name, std::size_t promptSize, std::size_t promptHistoryCount):
name{name},
promptSize{promptSize},
promptBuffer{new char[this->promptSize]},
promptHistory{promptHistoryCount},
mesh{nullptr}{
    memset(this->promptBuffer, '\0', this->promptSize);
    this->renderable.init();
    this->renderable.setVBOLayout(geometry::Graph::Vertex::layout);
}
Engine::~Engine(){
    delete[] this->promptBuffer;
    if(this->mesh != nullptr){
        delete this->mesh;
    }
}
void Engine::renderWindowMenu(){

}
void Engine::renderWindows(NRA::VGL::Controls &controls){
    if(this->renderWindowCLI){
        this->cliWindow();
    }
    for(auto &vp : this->viewports){
        if(vp.focus){
            vp.control(controls);
        }
        if(vp.visible){
            this->viewportWindow(vp);
        }
    }
}
void Engine::cliWindow(){
    
}
void Engine::viewportWindow(Viewport &vp){
    if(this->renderable.renderReady){
        vp.render(this->renderable);
    }
}
void Engine::aboutWindow(){
    
}

uint8_t hexColor(char h){
    if('0' <= h && h <= '9'){
        return h-'0';
    }else if('A' <= h && h <= 'F'){
        return h-'A'+10;
    }else if('a' <= h && h <= 'f'){
        return h-'a'+10;
    }
    return 0;
}

Engine::Color Engine::parseColorString(std::string str, Engine::Color defaultColor){
    Engine::Color res = defaultColor;

    if(str.size() >= 1 && str.at(0) == '#'){
        // Hex color
        if(str.length() == 4){
            // Single digit per color hex code
            res.r = (hexColor(str.at(1)) + 16*hexColor(str.at(1))) / 255.0f;
            res.g = (hexColor(str.at(2)) + 16*hexColor(str.at(2))) / 255.0f;
            res.b = (hexColor(str.at(3)) + 16*hexColor(str.at(3))) / 255.0f;
        }else if(str.length() == 7){
            // Two digit per color hex code
            res.r = (hexColor(str.at(1)) + 16*hexColor(str.at(2))) / 255.0f;
            res.g = (hexColor(str.at(3)) + 16*hexColor(str.at(4))) / 255.0f;
            res.b = (hexColor(str.at(5)) + 16*hexColor(str.at(6))) / 255.0f;
        }else{
            // Unknown format, set to default
        }
    }else if(str.size() >= 3 && str.at(0) == 'r' && str.at(1) == 'g' && str.at(2) == 'b'){
        // Decimal color
        std::string col = str.substr(3) + ",";
        uint8_t colors[3] = {0,0,0};
        uint8_t colIndex = 0;
        std::string temp;
        for(std::size_t i=0;i<col.size();++i){
            if(col.at(i) == ','){
                if(colIndex <= 3){
                    for(int j=0;j<temp.size();++j){
                        colors[colIndex] += (temp.at(j)-'0') * std::powl(10,temp.size()-j);
                    }
                }
                temp = "";
                ++colIndex;
            }else if('0' <= col.at(i) && col.at(i) <= '9'){
                temp.push_back(col.at(i));
            }
        }
        if(colIndex == 3){
            res.r = colors[0] / 255.0f;
            res.g = colors[1] / 255.0f;
            res.b = colors[2] / 255.0f;
        }
    }else{
        // Unknown format, set to default
    }
    return res;
}
std::vector<std::pair<Engine::Color, std::string>> Engine::parseColors(std::string str){
    std::vector<std::pair<Engine::Color, std::string>> res;
    bool color = false;
    bool escape = false;
    std::string colorStr;
    std::string substr;
    for(char c : str){
        if(color){
            if(escape){
                colorStr.push_back(c);
                escape = false;
            }else if(c == '\\'){
                escape = true;
            }else if(c == ';'){
                color = false;
            }else{
                colorStr.push_back(c);
            }
        }else{
            if(escape){
                escape = false;
                if(c == '\\'){
                    substr.push_back(c);
                }else{
                    if(substr.length() > 0){
                        res.push_back({this->parseColorString(colorStr),substr});
                    }
                    substr = "";
                    colorStr = "";
                    colorStr.push_back(c);
                    color = true;
                }
            }else if(c == '\\'){
                escape = true;
            }else{
                substr.push_back(c);
            }
        }
    }

    if(substr.length() > 0){
        res.push_back({this->parseColorString(colorStr),substr});
    }
    
    return res;
}
std::string Engine::filterColors(std::string str){
    std::string res;
    bool color = false;
    bool escape = false;
    for(char c : str){
        if(color){
            if(escape){
                escape = false;
            }else if(c == '\\'){
                escape = true;
            }else if(c == ';'){
                color = false;
            }else{
            }
        }else{
            if(escape){
                escape = false;
                if(c == '\\'){
                    res.push_back(c);
                }else{
                    color = true;
                }
            }else if(c == '\\'){
                escape = true;
            }else{
                res.push_back(c);
            }
        }
    }
    
    return res;
}
void Engine::resizePromptBuffer(std::size_t newSize){
    char *temp = new char[newSize];
    memset(temp, 0, newSize);
    memcpy(temp, this->promptBuffer, std::min(this->promptSize,newSize-1));
    delete[] this->promptBuffer;
    this->promptBuffer = temp;
    this->promptSize = newSize;
}
void Engine::resizePromptHistory(std::size_t newHistory){
    this->promptHistory.resize(newHistory);
}
void Engine::cliCommand(std::string command){
    if(command.size() > 0){
        std::queue<std::string> promptComponents;
        // Split prompt
        {
            std::size_t index0 = 0;
            std::size_t index1 = command.find(" ");
            while(index1 != command.npos){
                if(index1-index0 > 0){
                    promptComponents.push(command.substr(index0,index1-index0));
                }
                index0 = index1+1;
                index1 = command.find(" ", index0);
            }
            promptComponents.push(command.substr(index0));
        }
        
        auto commandName = commands.find(promptComponents.front());
        if(commandName == commands.end()){
            // Not a recognized command, do nothing
        }else{
            std::size_t commandID = commandName->second;
            promptComponents.pop();
            switch(commandID){
                case 0:{ // help
                    this->cliCommandHelp(promptComponents);
                }break;
                case 1:{ // set
                    this->cliCommandSet(promptComponents);
                }break;
                case 2:{ // skip

                }break;
                case 3:{ // union
                    this->cliCommandCSG(promptComponents, CSG::OP::UNION);
                }break;
                case 4:{ // difference
                    this->cliCommandCSG(promptComponents, CSG::OP::DIFFERENCE);
                }break;
                case 5:{ // intersection
                    this->cliCommandCSG(promptComponents, CSG::OP::INERSECTION);
                }break;
                case 6:{ // transform
                    this->cliCommandTransform(promptComponents);
                }break;
                case 7:{ // csg
                    std::string cmd = promptComponents.front();
                    promptComponents.pop();
                    if(cmd == "list"){
                        this->cliCommandCSGList(promptComponents);
                    }else{
                        auto cmd2 = commands.find(cmd);
                        switch(cmd2 == commands.end() ? 0 : cmd2->second){
                            case 3:{ // union
                                this->cliCommandCSG(promptComponents, CSG::OP::UNION);
                            }break;
                            case 4:{ // difference
                                this->cliCommandCSG(promptComponents, CSG::OP::DIFFERENCE);
                            }break;
                            case 5:{ // intersection
                                this->cliCommandCSG(promptComponents, CSG::OP::INERSECTION);
                            }break;
                            case 6:{ // transform
                                this->cliCommandTransform(promptComponents);
                            }break;
                            default:{
                                std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_ERROR,"Error: Unrecognized CSG operation, aborting"}};
                                this->promptHistory.push(msg);
                            }break;
                        };
                    }
                }break;
                case 8:{ // sphere
                    this->cliCommandSphere(promptComponents);
                }break;
                default:{

                }
            }
        }
    }
}

void Engine::addViewport(std::array<NRA::VGL::ControlBind, 17> controls, int width, int height, NRA::VGL::Shader &shader){
    this->viewports.emplace_back(std::string{"Viewport "}+std::to_string(this->viewports.size()),std::to_string(this->nextViewportID++), controls, width, height, shader);
}

void Engine::generateMesh(){
    if(this->mesh != nullptr){
        delete this->mesh;
    }
    this->mesh = new NRA::VGL::Mesh{sizeof(geometry::Graph::Vertex)/4};

    geometry::Graph::Vertex vertices[4] = {
        {{-0.5f, -0.5f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}}
    };
    GLuint indices[6] = {
        0,1,2,
        0,2,3
    };
    this->mesh->add(vertices, indices, 4, 6);

    // @TODO: Generate actual mesh
    std::unordered_map<csgID, geometry::Graph> csgRes;


    this->renderable.loadMesh(*this->mesh);
    this->renderable.renderReady = true;
}