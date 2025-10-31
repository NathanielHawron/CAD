#include "CAD/general/engine.hpp"
#include "CAD/general/engine_macro.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandMesh(std::queue<std::string> &promptComponents){
    std::string cmd = "";
    if(promptComponents.size() > 0){
        cmd = promptComponents.back();
        promptComponents.pop();
    }
    if(cmd == "build"){
        this->generateMesh();
    }
}
