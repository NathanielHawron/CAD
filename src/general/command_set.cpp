#include "CAD/general/engine.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandSet(std::queue<std::string> &promptComponents){
    if(promptComponents.size() >= 2){
        if(promptComponents.size() > 2){
            std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_WARNING,"Error: Too many arguments provided for command set, ignoring extra arguments"}};
            this->promptHistory.push(msg);
        }
        if(promptComponents.front() == "promptSize"){
            promptComponents.pop();
            try{
                int newSize = std::stoi(promptComponents.front());
                if(newSize < 32){
                    std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_WARNING,"Prompt size too small, setting to 32 instead"}};
                    this->promptHistory.push(msg);
                    newSize = 32;
                }
                this->resizePromptBuffer(newSize+1);
                std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_INFO,"Set promptSize to "+promptComponents.front()}};
                this->promptHistory.push(msg);
            }catch(std::exception e){
                std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_ERROR,"Error: Could not parse value for command set, aborting"}};
                this->promptHistory.push(msg);
            }
        }else if(promptComponents.front() == "promptHistory"){
            promptComponents.pop();
            try{
                int newHistory = std::stoi(promptComponents.front());
                if(newHistory < 8){
                    std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_WARNING,"Prompt history too small, setting to 8 instead"}};
                    this->promptHistory.push(msg);
                    newHistory = 8;
                }
                this->resizePromptHistory(newHistory);
                std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_INFO,"Set promptHistory to "+promptComponents.front()}};
                this->promptHistory.push(msg);
            }catch(std::exception e){
                std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_ERROR,"Error: Could not parse value for command set, aborting"}};
                this->promptHistory.push(msg);
            }
        }else{
            std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_ERROR,"Error: Unrecognized variable: "+promptComponents.front()+", aborting"}};
            this->promptHistory.push(msg);
        }
    }else{
        std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_ERROR,"Error: Not enough arguments provided for command set, aborting"}};
        this->promptHistory.push(msg);
    }
}
