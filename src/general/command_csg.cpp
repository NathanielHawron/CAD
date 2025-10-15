#include "CAD/general/engine.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandCSG(std::queue<std::string> &promptComponents, CSG::OP op){
    if(promptComponents.size() != 4){
        std::vector<std::pair<Engine::Color, std::string>> msg{
            {Engine::COLOR_ERROR,"Error:"},
            {Engine::COLOR_ERROR2," expected 4 parameters for csg operation, got " + std::to_string(promptComponents.size())}
        };
        this->promptHistory.push(msg);
        return;
    }
    std::string aType = promptComponents.front();
    promptComponents.pop();
    std::string aIndex = promptComponents.front();
    promptComponents.pop();
    std::string bType = promptComponents.front();
    promptComponents.pop();
    std::string bIndex = promptComponents.front();
    promptComponents.pop();
    
    std::string aTypeU = aType;
    for(auto & c : aTypeU){
        c = std::toupper(c);
    }
    std::string bTypeU = bType;
    for(auto & c : bTypeU){
        c = std::toupper(c);
    }
    
    VolumeType at = VolumeType::NONE, bt = VolumeType::NONE;
    try{
        for(std::size_t i=0;i<VolumeType_string.size();++i){
            if(VolumeType_string.at(i) == aTypeU){
                at = (VolumeType)i;
                break;
            }
        }
        for(std::size_t i=0;i<VolumeType_string.size();++i){
            if(VolumeType_string.at(i) == bTypeU){
                bt = (VolumeType)i;
                break;
            }
        }
    }catch(std::exception e){
        std::vector<std::pair<Engine::Color, std::string>> msg{
            {Engine::COLOR_ERROR,"Error:"},
            {Engine::COLOR_ERROR2," could not parse type"}
        };
        this->promptHistory.push(msg);
        return;
    }

    std::size_t ai, bi;
    try{
        ai = std::stoull(aIndex);
        bi = std::stoull(bIndex);
    }catch(std::exception e){
        std::vector<std::pair<Engine::Color, std::string>> msg{
            {Engine::COLOR_ERROR,"Error:"},
            {Engine::COLOR_ERROR2," could not parse id"}
        };
        this->promptHistory.push(msg);
        return;
    }

    csgID id = this->addCSG({op,at,ai,bt,bi});
    
    {
        std::vector<std::pair<Engine::Color, std::string>> msg{
            {Engine::COLOR_INFO,"Added csg with id: "},
            {Engine::COLOR_INFO2,std::to_string(id)}
        };
        this->promptHistory.push(msg);
    }
}
