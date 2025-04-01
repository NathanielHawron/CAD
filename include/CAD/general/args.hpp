#pragma once
#include <string>
#include <unordered_map>

void parseArgs(int argc, char **argv, std::unordered_map<std::string, std::string> &res){
    for(int i=0;i<argc;++i){
        std::string s = argv[i];
        std::string c;
        std::string v;
        int eq;
        if(s.find("--") == 0){
            s = s.substr(2);
            eq = s.find("=");
            
            if(eq >= 0){    // If equals sign is found, treat it like a key=value pair
                // If equal sign is last character, set option to empty string
                if(eq < s.size()){
                    c = s.substr(0,eq);
                    v = s.substr(s.find("=")+1);
                    if(res.find(c) != res.end()){
                        res.at(c) = v;
                    }
                }else{
                    c = s.substr(0,eq);
                    if(res.find(c) != res.end()){
                        res.at(c) = v;
                    }
                }
            }else{      // If equals sign is not found, treat it like a key=true
                // @TODO
            }
        }
    }
}