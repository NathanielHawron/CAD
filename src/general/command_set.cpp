#include "CAD/general/engine.hpp"
#include "CAD/general/engine_macro.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandSet(std::queue<std::string> &promptComponents){
    if(promptComponents.size() >= 2){
        if(promptComponents.size() > 2){
            PUSH_MSG2(Engine::COLORS_WARNING,
                "Warning: ",
                "Too many arguments provided for command \"set\", ignoring extra arguments"
            )
        }
        if(promptComponents.front() == "promptSize"){
            promptComponents.pop();
            try{
                int newSize = std::stoi(promptComponents.front());
                if(newSize < 32){
                    PUSH_MSG2(Engine::COLORS_WARNING,
                        "Warning: ",
                        "Prompt size too small ("+promptComponents.front()+"), setting to 32 instead"
                    )
                    newSize = 32;
                }
                this->resizePromptBuffer(newSize+1);
                PUSH_MSG1(Engine::COLORS_INFO[1],
                    "Set promptSize to "+promptComponents.front()
                )
            }catch(std::exception e){
                PUSH_MSG2(Engine::COLORS_ERROR,
                    "Error: ",
                    "Could not parse value for command \"set\", aborting"
                )
            }
        }else if(promptComponents.front() == "promptHistory"){
            promptComponents.pop();
            try{
                int newHistory = std::stoi(promptComponents.front());
                if(newHistory < 8){
                    PUSH_MSG2(Engine::COLORS_WARNING,
                        "Warning: ",
                        "Prompt history too small, ("+promptComponents.front()+") setting to 8 instead"
                    )
                    newHistory = 8;
                }
                this->resizePromptHistory(newHistory);
                PUSH_MSG1(Engine::COLORS_INFO[1],
                    "Set promptHistory to "+promptComponents.front()
                )
            }catch(std::exception e){
                PUSH_MSG2(Engine::COLORS_ERROR,
                    "Error: ",
                    "Could not parse value for command \"set\", aborting"
                )
            }
        }else{
            PUSH_MSG2(Engine::COLORS_ERROR,
                "Error: ",
                "Unrecognized variable: "+promptComponents.front()+", aborting"
            )
        }
    }else{
        PUSH_MSG2(Engine::COLORS_ERROR,
            "Error: ",
            "Not enough arguments provided for command \"set\", aborting"
        )
    }
}
