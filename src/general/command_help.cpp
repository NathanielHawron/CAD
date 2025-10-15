#include "CAD/general/engine.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandHelp(std::queue<std::string> &promptComponents){
    std::string cmd = "";
    if(promptComponents.size() > 0){
        cmd = promptComponents.back();
        promptComponents.pop();
    }
    if(cmd == "" || cmd == "help"){
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"help <command>"},
                {Engine::COLOR_INFO2," - Display help message for a command, or this message if empty or help."},
            };
            this->promptHistory.push(msg);
        }
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"skip <type>"},
                {Engine::COLOR_INFO2," - Skips the next ID for the type of object. Useful if an item is deleted, to skip the overhead of creating an item while retaining indices."}
            };
            this->promptHistory.push(msg);
        }
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"set <parameter> <value>"},
                {Engine::COLOR_INFO2," - Sets the value of a parameter."}
            };
            this->promptHistory.push(msg);
        }
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"sphere <add | list> <radius> <x> <y> <z>"},
                {Engine::COLOR_INFO2," - Creates a sphere, or lists all spheres."}
            };
            this->promptHistory.push(msg);
        }
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"union <aType> <aIndex> <bType> <bIndex>"},
                {Engine::COLOR_INFO2," - Creates a union (A + B)."}
            };
            this->promptHistory.push(msg);
        }
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"difference <aType> <aIndex> <bType> <bIndex>"},
                {Engine::COLOR_INFO2," - Creates a difference (A - B)."}
            };
            this->promptHistory.push(msg);
        }
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"intersection <aType> <aIndex> <bType> <bIndex>"},
                {Engine::COLOR_INFO2," - Creates an intersection (A * B)."}
            };
            this->promptHistory.push(msg);
        }
    }else if(cmd == "set"){
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"set <parameter> <value>"},
                {Engine::COLOR_INFO2," - Sets the value of a parameter."}
            };
            this->promptHistory.push(msg);
        }
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"Available parameters include:"},
            };
            this->promptHistory.push(msg);
        }
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"promptSize"},
                {Engine::COLOR_INFO2," - How many characters can be typed in the CLI (min 32)."}
            };
            this->promptHistory.push(msg);
        }
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"promptHistory"},
                {Engine::COLOR_INFO2," - How many prompts to remember (min 8)."}
            };
            this->promptHistory.push(msg);
        }
    }else if(cmd == "skip"){
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"skip <type>"},
                {Engine::COLOR_INFO2," - Skips the next ID for the type of object. Useful if an item is deleted, to skip the overhead of creating an item while retaining indices."}
            };
            this->promptHistory.push(msg);
        }
    }else if(cmd == "sphere"){
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"sphere <add | list> <radius> <x> <y> <z>"},
                {Engine::COLOR_INFO2," - Creates a sphere, or lists all spheres."}
            };
            this->promptHistory.push(msg);
        }
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"<add>: "},
                {Engine::COLOR_INFO2,"The sphere has a radius and xyz position. The ID of the sphere will be printed."}
            };
            this->promptHistory.push(msg);
        }
        {
            std::vector<std::pair<Engine::Color, std::string>> msg{
                {Engine::COLOR_INFO,"<list>: "},
                {Engine::COLOR_INFO2,"Lists all spheres by id, radius, and xyz position."}
            };
            this->promptHistory.push(msg);
        }
    }
}
