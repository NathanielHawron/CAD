#include "CAD/general/engine.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandSphere(std::queue<std::string> &promptComponents){
    std::string subcmd = promptComponents.front();
    promptComponents.pop();
    if(subcmd == "add"){
        if(promptComponents.size() != 4){
            std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_ERROR,"Error: 4 arguments expected for command sphere add, aborting"}};
            this->promptHistory.push(msg);
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
                std::vector<std::pair<Engine::Color, std::string>> msg{
                    {Engine::COLOR_INFO,"Added sphere with id: "},
                    {Engine::COLOR_INFO2,std::to_string(id)}
                };
                this->promptHistory.push(msg);
            }catch(std::exception e){
                std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_ERROR,"Error: could not parse arguments for command sphere add, aborting"}};
                this->promptHistory.push(msg);
            }
        }
    }else if(subcmd == "list"){
        for(auto &s : this->spheres){
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,std::to_string(s.first)},
                {Engine::COLOR_INFO2,s.second.toString()}
            };
            this->promptHistory.push(msg);
        }
    }else{
        std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_ERROR,"Error: unknown subcommand for sphere, aborting"}};
        this->promptHistory.push(msg);
    }
}
