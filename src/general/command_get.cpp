#include "CAD/general/engine.hpp"
#include "CAD/general/engine_macro.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandGet(std::queue<std::string> &promptComponents){
    while(!promptComponents.empty()){
        std::string s = promptComponents.front();
        promptComponents.pop();
        if(s == "promptSize"){
            PUSH_MSG2(Engine::COLORS_INFO,
                "promptSize: ",
                std::to_string(this->promptSize)
            )
        }else if(s == "consoleSize"){
            PUSH_MSG2(Engine::COLORS_INFO,
                "consoleSize: ",
                std::to_string(this->console.maxSize())
            )
        }else if(s == "promptHistory"){
            PUSH_MSG2(Engine::COLORS_INFO,
                "promptHistory: ",
                std::to_string(this->promptHistory.maxSize())
            )
        }else if(s == "subdivisions"){
            PUSH_MSG2(Engine::COLORS_INFO,
                "subdivisions: ",
                std::to_string(this->subdivisions)
            )
        }else{
            PUSH_MSG2(Engine::COLORS_WARNING,
                "Warning: ",
                "Parameter "+s+" not found, ignoring"
            )
        }
    }
}
