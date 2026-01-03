#include "CAD/general/engine.hpp"
#include "CAD/general/engine_macro.hpp"

#include <string>
#include <algorithm>

using namespace CAD;
using namespace general;

void Engine::cliCommandSet(std::queue<std::string> &promptComponents){
    const static std::array<std::string,4> integralParams = {
        "promptSize","consoleSize","promptHistory",         // Console params
        "subdivisions"                                      // Sphere params
    };
    const static std::array<std::size_t,4> integralMins = {
        32, 8, 1,
        2
    };
    const static std::array<std::size_t,4> integralDefaults = {
        255, 100, 50,
        10
    };
    if(promptComponents.size() >= 2){
        if(promptComponents.size() > 2){
            PUSH_MSG2(Engine::COLORS_WARNING,
                "Warning: ",
                "Too many arguments provided for command \"set\", ignoring extra arguments"
            )
        }
        std::string param = promptComponents.front();
        promptComponents.pop();
        std::size_t integralParamIndex = std::find(integralParams.begin(),integralParams.end(),param)-integralParams.begin();
        if(3 >= integralParamIndex && integralParamIndex >= 0){
            std::size_t val;
            try{
                std::size_t val;
                if(promptComponents.front() == "default"){
                    val = integralDefaults.at(integralParamIndex);
                }else{
                    val = std::stoull(promptComponents.front());
                }
                if(val < (integralMins.at(integralParamIndex))){
                    PUSH_MSG2(Engine::COLORS_WARNING,
                        "Warning: ",
                        param+" too small, "+std::to_string(integralMins.at(integralParamIndex))+" will be used instead of "+promptComponents.front()
                    )
                    val = integralMins.at(integralParamIndex);
                }
                switch(integralParamIndex){
                    case 0:{    // Prompt Size
                        this->resizePromptBuffer(val+1);
                    }break;
                    case 1:{    // Console Size
                        this->resizeConsole(val);
                    }break;
                    case 2:{    // Prompt History
                        this->resizePromptHistory(val);
                    }break;
                    case 3:{    // Subdivisions
                        this->subdivisions = val;
                    }break;
                    default:{
        
                    }
                }
                PUSH_MSG1(Engine::COLORS_INFO[1],
                    "Set "+param+" to "+std::to_string(val)
                )
            }catch(std::exception e){
                PUSH_MSG2(Engine::COLORS_ERROR,
                    "Error: ",
                    "Could not parse value for \"set "+param+"\", aborting"
                )
            }
        }
    }else{
        PUSH_MSG2(Engine::COLORS_ERROR,
            "Error: ",
            "Not enough parameters for the command \"set\" <param> <value>"
        )
    }
}
