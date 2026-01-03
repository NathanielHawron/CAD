#include "CAD/general/engine.hpp"
#include "CAD/general/engine_macro.hpp"

#include <unordered_map>
#include <chrono>

#include <poll.h>
#include <unistd.h>

using namespace CAD;
using namespace general;

#define GET_TYPE(type, index, csg, varName, f, s)                       \
    switch(type){                                                       \
        case Engine::VolumeType::NONE:{                                 \
                                                                        \
        }break;                                                         \
        case Engine::VolumeType::CSG:{                                  \
            auto it = csg.find(index);                                  \
            if(it == csg.end()){                                        \
                f;                                                      \
            }else{                                                      \
                geometry::Graph varName = it->second;                   \
                s;                                                      \
            }                                                           \
        }break;                                                         \
        case Engine::VolumeType::SPHERE:{                               \
            auto it = this->spheres.find(index);                        \
            if(it == this->spheres.end()){                              \
                f;                                                      \
            }else{                                                      \
                geometry::Sphere varName = it->second;                  \
                s;                                                      \
            }                                                           \
        }break;                                                         \
        default:{                                                       \
                                                                        \
        };                                                              \
    }

constexpr std::size_t COMMAND_PRE_TIME_INDEX =      0;

constexpr std::size_t COMMAND_HELP_INDEX =          0;
constexpr std::size_t COMMAND_CLEAR_INDEX =         1;
constexpr std::size_t COMMAND_MESH_INDEX =          2;
constexpr std::size_t COMMAND_SET_INDEX =           3;
constexpr std::size_t COMMAND_GET_INDEX =           4;
constexpr std::size_t COMMAND_SKIP_INDEX =          5;
constexpr std::size_t COMMAND_UNION_INDEX =         6;
constexpr std::size_t COMMAND_DIFFERENCE_INDEX =    7;
constexpr std::size_t COMMAND_INTERSECTION_INDEX =  8;
constexpr std::size_t COMMAND_TRANSFORM_INDEX =     9;
constexpr std::size_t COMMAND_CSG_INDEX =           10;
constexpr std::size_t COMMAND_SPHERE_INDEX =        11;

std::unordered_map<std::string, std::size_t> commandPrefixes = {
    {"time",            COMMAND_PRE_TIME_INDEX}
};
std::unordered_map<std::string, std::size_t> commands = {
    {"help",            COMMAND_HELP_INDEX},
    {"clear",           COMMAND_CLEAR_INDEX},
    {"mesh",            COMMAND_MESH_INDEX},
    {"set",             COMMAND_SET_INDEX},
    {"get",             COMMAND_GET_INDEX},
    {"skip",            COMMAND_SKIP_INDEX},
    {"union",           COMMAND_UNION_INDEX},
    {"difference",      COMMAND_DIFFERENCE_INDEX},
    {"intersection",    COMMAND_INTERSECTION_INDEX},
    {"transform",       COMMAND_TRANSFORM_INDEX},
    {"csg",             COMMAND_CSG_INDEX},
    {"sphere",          COMMAND_SPHERE_INDEX}
};

std::array<Engine::Color, 2> Engine::COLORS_ERROR =     {Engine::Color{1.0f,0.4f,0.4f},      Engine::Color{1.0f,0.5f,0.5f}};
std::array<Engine::Color, 2> Engine::COLORS_WARNING =   {Engine::Color{0.85f,0.65f,0.4f},    Engine::Color{0.9f,0.6f,0.4f}};
std::array<Engine::Color, 2> Engine::COLORS_INFO =      {Engine::Color{0.4f,0.6f,1.0f},      Engine::Color{0.4f,0.7f,0.9f}};
std::array<Engine::Color, 2> Engine::COLORS_SUCCESS =   {Engine::Color{0.1f,0.8f,0.2f},      Engine::Color{0.2f,0.9f,0.4f}};

const std::array<std::string,4> Engine::VolumeType_string = {
    "NONE",
    "CSG",
    "TRANSFORM",
    "SPHERE"
};
const std::array<std::string,4> Engine::CSG::OP_string = {
    "TRANSFORM",
    "UNION",
    "DIFFERENCE",
    "INTERSECTION"
};

std::queue<std::string> Engine::cli_cin{};
std::thread *Engine::cli_cin_listener = nullptr;
bool Engine::cli_cin_en = false;
bool Engine::cli_cout_en = false;

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

void listenCin(std::queue<std::string> &queue, bool &run){
    pollfd p{};
    p.fd = STDIN_FILENO;
    p.events = POLLIN;

    while(run){
        int r = poll(&p, 1, 1000);
        if(r > 0 && (p.revents & POLLIN)){
            std::string s;
            if(std::getline(std::cin, s)){
                queue.push(s);
            }
        }
    }
}

Engine::Engine(std::string name, std::size_t promptSize, std::size_t consoleSize, std::size_t promptHistoryCount, std::size_t subdivisions):
name{name},
promptSize{promptSize},
promptBuffer{new char[this->promptSize]},
console{consoleSize},
promptHistory{promptHistoryCount},
promptHistoryIndex{this->promptHistory.end()},
mesh{nullptr},
subdivisions{subdivisions}{
    memset(this->promptBuffer, '\0', this->promptSize);
    this->renderable.init();
    this->renderable.setVBOLayout(geometry::Graph::Vertex::layout);
}
Engine::~Engine(){
    this->stopCin();
    delete[] this->promptBuffer;
    if(this->mesh != nullptr){
        delete this->mesh;
    }
}
void Engine::startCin(){
    if(Engine::cli_cin_listener == nullptr){
        Engine::cli_cin_en = true;
        Engine::cli_cin_listener = new std::thread(listenCin, std::ref(Engine::cli_cin), std::ref(Engine::cli_cin_en));
    }
}
void Engine::stopCin(){
    if(Engine::cli_cin_listener != nullptr){
        Engine::cli_cin_en = false;
        if(Engine::cli_cin_listener->joinable()){
            Engine::cli_cin_listener->join();
        }
        delete Engine::cli_cin_listener;
    }
}
void Engine::cinCliCommand(){
    if(!Engine::cli_cin.empty()){
        std::string temp = Engine::cli_cin.front();
        this->console.push_back(this->parseColors(temp));
        this->cliCommand(this->filterColors(temp));
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
        for(std::size_t i=0;i<col.size() && colIndex < 3;++i){
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
void Engine::resizeConsole(std::size_t newSize){
    this->console.resize(newSize);
}
void Engine::resizePromptHistory(std::size_t newSize){
    this->promptHistory.resize(newSize);
    this->promptHistoryIndex = this->promptHistory.begin();
}
void Engine::cliCommand(std::string command){
    if(command.size() > 0){
        if(this->promptHistory.back() != command){
            this->promptHistory.push_back(command);
        }
        this->promptHistoryIndex = this->promptHistory.end();

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
        
        bool benchmark = false;
        auto start = std::chrono::high_resolution_clock::now();

        auto prefix = commandPrefixes.find(promptComponents.front());
        while(prefix != commandPrefixes.end()){
            std::size_t prefixID = prefix->second;
            switch(prefixID){
                case COMMAND_PRE_TIME_INDEX:{
                    benchmark = true;
                }break;
                default:{

                }
            }
            promptComponents.pop();
            prefix = commandPrefixes.find(promptComponents.front());
        }

        auto commandName = commands.find(promptComponents.front());
        if(commandName == commands.end()){
            // Not a recognized command, do nothing
        }else{
            std::size_t commandID = commandName->second;
            promptComponents.pop();
            switch(commandID){
                case COMMAND_HELP_INDEX:{
                    this->cliCommandHelp(promptComponents);
                }break;
                case COMMAND_CLEAR_INDEX:{
                    this->console.clear();
                    PUSH_MSG1(COLORS_INFO[0], "Cleared");
                }break;
                case COMMAND_MESH_INDEX:{
                    this->cliCommandMesh(promptComponents);
                }break;
                case COMMAND_SET_INDEX:{
                    this->cliCommandSet(promptComponents);
                }break;
                case COMMAND_GET_INDEX:{
                    this->cliCommandGet(promptComponents);
                }break;
                case COMMAND_SKIP_INDEX:{
                    this->cliCommandSkip(promptComponents);
                }break;
                case COMMAND_UNION_INDEX:{
                    this->cliCommandCSG(promptComponents, CSG::OP::UNION);
                }break;
                case COMMAND_DIFFERENCE_INDEX:{
                    this->cliCommandCSG(promptComponents, CSG::OP::DIFFERENCE);
                }break;
                case COMMAND_INTERSECTION_INDEX:{
                    this->cliCommandCSG(promptComponents, CSG::OP::INTERSECTION);
                }break;
                case COMMAND_TRANSFORM_INDEX:{
                    this->cliCommandTransform(promptComponents);
                }break;
                case COMMAND_CSG_INDEX:{
                    std::string cmd = promptComponents.front();
                    promptComponents.pop();
                    if(cmd == "list"){
                        this->cliCommandCSGList(promptComponents);
                    }else{
                        auto cmd2 = commands.find(cmd);
                        switch(cmd2 == commands.end() ? 0 : cmd2->second){
                            case COMMAND_UNION_INDEX:{
                                this->cliCommandCSG(promptComponents, CSG::OP::UNION);
                            }break;
                            case COMMAND_DIFFERENCE_INDEX:{
                                this->cliCommandCSG(promptComponents, CSG::OP::DIFFERENCE);
                            }break;
                            case COMMAND_INTERSECTION_INDEX:{
                                this->cliCommandCSG(promptComponents, CSG::OP::INTERSECTION);
                            }break;
                            case COMMAND_TRANSFORM_INDEX:{
                                this->cliCommandTransform(promptComponents);
                            }break;
                            default:{
                                PUSH_MSG2(COLORS_ERROR,
                                    "Error: ",
                                    "Unrecognized CSG operation, aborting"
                                )
                            }break;
                        };
                    }
                }break;
                case COMMAND_SPHERE_INDEX:{
                    this->cliCommandSphere(promptComponents);
                }break;
                default:{

                }
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::size_t runtime_us = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();

        if(benchmark){
            PUSH_MSG2(Engine::COLORS_INFO,
                "Runtime: ",
                std::to_string(runtime_us)+"us"
            )
        }
    }
}

void Engine::addViewport(std::array<NRA::VGL::ControlBind, 17> controls, int width, int height, NRA::VGL::Shader &shader){
    this->viewports.emplace_back(std::string{"Viewport "}+std::to_string(this->nextViewportID),std::to_string(this->nextViewportID), controls, width, height, shader);
    ++this->nextViewportID;
}

void Engine::generateMesh(){
    if(this->mesh != nullptr){
        delete this->mesh;
    }
    this->mesh = new NRA::VGL::Mesh{sizeof(geometry::Graph::Vertex)/4};

    PUSH_MSG2(COLORS_INFO,
        "Generating Mesh",
        "(" + std::to_string(this->csgOperations.size()) + " CSG operations)"
    )
    // @TODO: Generate actual mesh
    std::unordered_map<csgID, geometry::Graph> csgRes;
    for(auto csgOp : this->csgOperations){
        auto &csg = csgOp.second;
        if(csg.op == Engine::CSG::OP::TRANSFORM){
            GET_TYPE(
                csg.aType,csg.aIndex,csgRes,A,
                std::cout << "Failed to find A" << std::endl,
                {
                    geometry::Transform t = this->transforms.at(csg.bIndex);
                    if(csg.aType == Engine::VolumeType::CSG){
                        csgRes.insert({csgOp.first,A.transform(t)});
                    }else{
                        csgRes.insert({csgOp.first,A.generateGraph().transform(t)});
                    }
                }
            )
        }else{
            GET_TYPE(
                csg.aType,csg.aIndex,csgRes,A,
                std::cout << "Failed to find A" << std::endl,
                GET_TYPE(
                    csg.bType,csg.bIndex,csgRes,B,
                    std::cout << "Failed to find B" << std::endl,
                    {
                        switch(csg.op){
                            case CSG::OP::UNION:{
                                csgRes.insert({csgOp.first,A.unionGraph(&B)});
                                std::cout << "Union" << std::endl;
                            }break;
                            case CSG::OP::DIFFERENCE:{
                                csgRes.insert({csgOp.first,A.differenceGraph(&B)});
                                std::cout << "Difference" << std::endl;
                            }break;
                            case CSG::OP::INTERSECTION:{
                                csgRes.insert({csgOp.first,A.intersectionGraph(&B)});
                                std::cout << "Intersection" << std::endl;
                            }break;
                        }
                    }
                )
            )
        }
    }

    try{
        csgRes.at(0).addToMesh(*this->mesh);
    }catch(std::exception e){
        PUSH_MSG2(Engine::COLORS_ERROR, "Error: ", e.what())
    }


    this->renderable.loadMesh(*this->mesh);
    this->renderable.renderReady = true;
}