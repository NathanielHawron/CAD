#include "CAD/general/engine.hpp"
#include "CAD/general/engine_macro.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandCSGList(std::queue<std::string> &promptComponents){
    for(auto &s : this->csgOperations){
        PUSH_MSG2(Engine::COLORS_INFO,
            std::to_string(s.first),
            s.second.toString()
        )
    }
}
