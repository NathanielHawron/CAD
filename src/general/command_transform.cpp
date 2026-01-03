#include "CAD/general/engine.hpp"
#include "CAD/general/engine_macro.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandTransform(std::queue<std::string> &promptComponents){
    std::string cmd = "";
    
    if(promptComponents.size() > 0){
        cmd = promptComponents.front();
        promptComponents.pop();
    }

    if(cmd == "" || cmd == "i" || cmd == "identity"){
        geometry::Transform t = geometry::Transform{glm::mat4(1.0f)};
        transformID id = this->addTransform(t);
        PUSH_MSG2(Engine::COLORS_INFO,    
            "Added transform with id: ",
            std::to_string(id)
        )
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
    }else if(cmd == "r" || cmd == "rotate"){

    }else if(cmd == "s" || cmd == "scale"){

    }else if(cmd == "a" || cmd == "apply"){
        if(promptComponents.size() != 3){
            PUSH_MSG2(Engine::COLORS_ERROR,
                "Error: ",
                " expected 3 parameters for transform operation, got " + std::to_string(promptComponents.size())
            )
            return;
        }
        std::string aType = promptComponents.front();
        promptComponents.pop();
        std::string aIndex = promptComponents.front();
        promptComponents.pop();
        std::string bIndex = promptComponents.front();
        promptComponents.pop();
        std::string aTypeU = aType;
        for(auto & c : aTypeU){
            c = std::toupper(c);
        }
        VolumeType at = VolumeType::NONE;
        try{
            for(std::size_t i=0;i<VolumeType_string.size();++i){
                if(VolumeType_string.at(i) == aTypeU){
                    at = (VolumeType)i;
                    break;
                }
            }
        }catch(std::exception e){
            PUSH_MSG2(Engine::COLORS_ERROR,
                "Error: ",
                "Could not parse type"
            )
            return;
        }
        std::size_t ai, bi;
        try{
            ai = std::stoull(aIndex);
            bi = std::stoull(bIndex);
        }catch(std::exception e){
            PUSH_MSG2(Engine::COLORS_ERROR,
                "Error: ",
                "Could not parse id"
            )
            return;
        }
        csgID id = this->addCSG({Engine::CSG::OP::TRANSFORM,at,ai,Engine::VolumeType::TRANSFORM,0});
        PUSH_MSG2(Engine::COLORS_INFO,    
            "Added csg with id: ",
            std::to_string(id)
        )
    }
}
