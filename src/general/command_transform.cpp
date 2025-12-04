#include "CAD/general/engine.hpp"
#include "CAD/general/engine_macro.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandTransform(std::queue<std::string> &promptComponents){
    std::string cmd = "";
    if(promptComponents.size() > 0){
        cmd = promptComponents.back();
        promptComponents.pop();
    }
    if(cmd == "" || cmd == "identity"){
        geometry::Transform t = geometry::Transform{glm::mat4(1.0f)};
        this->addTransform(t);
    }else if(cmd == "t" || cmd == "translate"){
        if(promptComponents.size() != 3){
            PUSH_MSG2(Engine::COLORS_ERROR,
                "Error: ",
                "Expected 3 parameters for translate, got " + std::to_string(promptComponents.size())
            )
            return;
        }
        try{
                float x = std::stof(promptComponents.front());
                promptComponents.pop();
                float y = std::stof(promptComponents.front());
                promptComponents.pop();
                float z = std::stof(promptComponents.front());
                promptComponents.pop();
                geometry::Transform t = geometry::Transform{glm::translate(glm::mat4(1.0f),glm::vec3{x,y,z})};
                transformID id = this->addTransform(t);
                PUSH_MSG2(Engine::COLORS_INFO,
                    "Added sphere with id: ",
                    std::to_string(id)
                )
            }catch(std::exception e){
                PUSH_MSG2(Engine::COLORS_ERROR,
                    "Error: ",
                    "Could not parse arguments for command sphere add, aborting"
                )
            }
    }
}
