#include "CAD/general/engine.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandCSGList(std::queue<std::string> &promptComponents){
    for(auto &s : this->csgOperations){
        std::vector<std::pair<Engine::Color, std::string>> msg{
            {Engine::COLOR_INFO,std::to_string(s.first)},
            {Engine::COLOR_INFO2,s.second.toString()}
        };
        this->promptHistory.push(msg);
    }
}
