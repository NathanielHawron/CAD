#include "CAD/general/engine.hpp"
#include "CAD/general/engine_macro.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandSphere(std::queue<std::string> &promptComponents){
    std::string subcmd = promptComponents.front();
    promptComponents.pop();
    if(subcmd == "add"){
        if(promptComponents.size() != 4){
            PUSH_MSG2(Engine::COLORS_ERROR,
                "Error: ",
                "4 arguments expected for command sphere add, aborting"
            )
        }else{
            try{
                float r = std::stof(promptComponents.front());
                promptComponents.pop();
                float x = std::stof(promptComponents.front());
                promptComponents.pop();
                float y = std::stof(promptComponents.front());
                promptComponents.pop();
                float z = std::stof(promptComponents.front());
                promptComponents.pop();
                sphereID id = this->addSphere(r, x, y, z);
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
    }else if(subcmd == "list"){
        for(auto &s : this->spheres){
            PUSH_MSG2(Engine::COLORS_INFO,
                std::to_string(s.first),
                s.second.toString()
            )
        }
    }else{
        PUSH_MSG2(Engine::COLORS_ERROR,
            "Error: ",
            "Unknown subcommand for sphere, aborting"
        )
    }
}
