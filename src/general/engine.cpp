#include "CAD/general/engine.hpp"

#include <queue>
#include <unordered_map>

using namespace CAD;
using namespace general;

std::unordered_map<std::string, std::size_t> commands = {
    {"set", 0}
};

Engine::Color Engine::COLOR_ERROR = {1.0f,0.4f,0.4f};
Engine::Color Engine::COLOR_WARNING = {0.9f,0.6f,0.4f};
Engine::Color Engine::COLOR_INFO = {0.4f,0.6f,1.0f};

Engine::Engine(std::string name, std::size_t promptSize, std::size_t promptHistoryCount):name{name},promptSize{promptSize},promptBuffer{new char[this->promptSize]},promptHistory{promptHistoryCount}{
    memset(this->promptBuffer, '\0', this->promptSize);
}
Engine::~Engine(){
    delete[] this->promptBuffer;
}
void Engine::cliWindow(){
    
}
void Engine::aboutWindow(){
    
}

uint8_t hexColor(char h){
    if('0' <= h && h <= '9'){
        return h-'0';
    }else if('A' <= h && h <= 'F'){
        return h-'A'+10;
    }else if('a' <= h && h <= 'f'){
        return h-'a'+10;
    }
    return 0;
}

Engine::Color Engine::parseColorString(std::string str, Engine::Color defaultColor){
    Engine::Color res = defaultColor;

    if(str.size() >= 1 && str.at(0) == '#'){
        // Hex color
        if(str.length() == 4){
            // Single digit per color hex code
            res.r = (hexColor(str.at(1)) + 16*hexColor(str.at(1))) / 255.0f;
            res.g = (hexColor(str.at(2)) + 16*hexColor(str.at(2))) / 255.0f;
            res.b = (hexColor(str.at(3)) + 16*hexColor(str.at(3))) / 255.0f;
        }else if(str.length() == 7){
            // Two digit per color hex code
            res.r = (hexColor(str.at(1)) + 16*hexColor(str.at(2))) / 255.0f;
            res.g = (hexColor(str.at(3)) + 16*hexColor(str.at(4))) / 255.0f;
            res.b = (hexColor(str.at(5)) + 16*hexColor(str.at(6))) / 255.0f;
        }else{
            // Unknown format, set to default
        }
    }else if(str.size() >= 3 && str.at(0) == 'r' && str.at(1) == 'g' && str.at(2) == 'b'){
        // Decimal color
        std::string col = str.substr(3) + ",";
        uint8_t colors[3] = {0,0,0};
        uint8_t colIndex = 0;
        std::string temp;
        for(std::size_t i=0;i<col.size();++i){
            if(col.at(i) == ','){
                if(colIndex <= 3){
                    for(int j=0;j<temp.size();++j){
                        colors[colIndex] += (temp.at(j)-'0') * std::powl(10,temp.size()-j);
                    }
                }
                temp = "";
                ++colIndex;
            }else if('0' <= col.at(i) && col.at(i) <= '9'){
                temp.push_back(col.at(i));
            }
        }
        if(colIndex == 3){
            res.r = colors[0] / 255.0f;
            res.g = colors[1] / 255.0f;
            res.b = colors[2] / 255.0f;
        }
    }else{
        // Unknown format, set to default
    }
    return res;
}
std::vector<std::pair<Engine::Color, std::string>> Engine::parseColors(std::string str){
    std::vector<std::pair<Engine::Color, std::string>> res;
    bool color = false;
    bool escape = false;
    std::string colorStr;
    std::string substr;
    for(char c : str){
        if(color){
            if(escape){
                colorStr.push_back(c);
                escape = false;
            }else if(c == '\\'){
                escape = true;
            }else if(c == ';'){
                color = false;
            }else{
                colorStr.push_back(c);
            }
        }else{
            if(escape){
                escape = false;
                if(c == '\\'){
                    substr.push_back(c);
                }else{
                    if(substr.length() > 0){
                        res.push_back({this->parseColorString(colorStr),substr});
                    }
                    substr = "";
                    colorStr = "";
                    colorStr.push_back(c);
                    color = true;
                }
            }else if(c == '\\'){
                escape = true;
            }else{
                substr.push_back(c);
            }
        }
    }

    if(substr.length() > 0){
        res.push_back({this->parseColorString(colorStr),substr});
    }
    
    return res;
}
std::string Engine::filterColors(std::string str){
    std::string res;
    bool color = false;
    bool escape = false;
    for(char c : str){
        if(color){
            if(escape){
                escape = false;
            }else if(c == '\\'){
                escape = true;
            }else if(c == ';'){
                color = false;
            }else{
            }
        }else{
            if(escape){
                escape = false;
                if(c == '\\'){
                    res.push_back(c);
                }else{
                    color = true;
                }
            }else if(c == '\\'){
                escape = true;
            }else{
                res.push_back(c);
            }
        }
    }
    
    return res;
}
void Engine::resizePromptBuffer(std::size_t newSize){
    char *temp = new char[newSize];
    memset(temp, 0, newSize);
    memcpy(temp, this->promptBuffer, std::min(this->promptSize,newSize-1));
    delete[] this->promptBuffer;
    this->promptBuffer = temp;
    this->promptSize = newSize;
}
void Engine::cliCommand(std::string command){
    if(command.size() > 0){
        std::queue<std::string> promptComponents;
        // Split prompt
        {
            std::size_t index0 = 0;
            std::size_t index1 = command.find(" ");
            while(index1 != command.npos){
                if(index1-index0 > 1){
                    promptComponents.push(command.substr(index0,index1-index0));
                }
                index0 = index1+1;
                index1 = command.find(" ", index0+1);
            }
            promptComponents.push(command.substr(index0));
        }
        
        auto commandName = commands.find(promptComponents.front());
        if(commandName == commands.end()){

        }else{
            std::size_t commandID = commandName->second;
            promptComponents.pop();
            switch(commandID){
                case 0:{ // set
                    if(promptComponents.size() >= 2){
                        if(promptComponents.size() > 2){
                            std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_WARNING,"Error: Too many arguments provided for command set, ignoring extra arguments"}};
                            this->promptHistory.add(msg);
                        }
                        if(promptComponents.front() == "promptSize"){
                            promptComponents.pop();
                            try{
                                int newSize = std::stoi(promptComponents.front());
                                if(newSize < 20){
                                    std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_WARNING,"Prompt size too small, setting to 20 instead"}};
                                    this->promptHistory.add(msg);
                                    newSize = 20;
                                }
                                this->resizePromptBuffer(newSize+1);
                                std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_INFO,"Set promptSize to "+promptComponents.front()}};
                                this->promptHistory.add(msg);
                            }catch(std::exception e){
                                std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_ERROR,"Error: Could not parse value for command set, aborting"}};
                                this->promptHistory.add(msg);
                            }
                        }else{
                            std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_ERROR,"Error: Unrecognized variable: "+promptComponents.front()+", aborting"}};
                            this->promptHistory.add(msg);
                        }
                    }else{
                        std::vector<std::pair<Engine::Color, std::string>> msg{{Engine::COLOR_ERROR,"Error: Not enough arguments provided for command set, aborting"}};
                        this->promptHistory.add(msg);
                    }
                }break;
                default:{

                }
            }
        }
    }
}