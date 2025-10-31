#include "CAD/general/engine.hpp"
#include "CAD/general/engine_macro.hpp"

using namespace CAD;
using namespace general;

void Engine::cliCommandHelp(std::queue<std::string> &promptComponents){
    std::string cmd = "";
    if(promptComponents.size() > 0){
        cmd = promptComponents.back();
        promptComponents.pop();
    }
    if(cmd == "" || cmd == "help"){
        PUSH_MSG2(COLORS_INFO, 
            "help <prefix | command>",
            " - Display help message for a command, or this message if empty or help."
        )
        PUSH_MSG2(COLORS_INFO, 
            "time *",
            " - Sets the benchmark flag, which times how long the command takes to run."
        )
        PUSH_MSG2(COLORS_INFO, 
            "clear",
            " - Clears all of the prompt history."
        )
        PUSH_MSG2(COLORS_INFO, 
            "mesh {build}",
            " - Rebuilds the mesh."
        )
        PUSH_MSG2(COLORS_INFO,
            "set <parameter> <value>",
            " - Sets the value of a parameter."
        )
        PUSH_MSG2(COLORS_INFO,
            "get <parameter>",
            " - Gets the value of a parameter."
        )
        PUSH_MSG2(COLORS_INFO,
            "skip <type>",
            " - Skips the next ID for the type of object. Useful if an item is deleted, to skip the overhead of creating an item while retaining indices."
        )
        PUSH_MSG2(COLORS_INFO,
            "union <aType> <aIndex> <bType> <bIndex>",
            " - Creates a union (A + B)."
        )
        PUSH_MSG2(COLORS_INFO,
            "difference <aType> <aIndex> <bType> <bIndex>",
            " - Creates a difference (A - B)."
        )
        PUSH_MSG2(COLORS_INFO,
            "intersection <aType> <aIndex> <bType> <bIndex>",
            " - Creates an intersection (A * B)."
        )
        PUSH_MSG2(COLORS_INFO,
            "transform (NYI)",
            " - Creates a transformation matrix."
        )
        PUSH_MSG2(COLORS_INFO,
            "csg {union | difference | intersection | transform} <aType> <aIndex> <bType> <bIndex>",
            " - Performs a csg operation."
        )
        PUSH_MSG2(COLORS_INFO,
           "sphere {add | list} <radius> <x> <y> <z>",
            " - Creates a sphere, or lists all spheres."
        )
    }else if(cmd == "prefix"){
        PUSH_MSG2(COLORS_INFO, 
            "time *",
            " - Times how long the command takes to run in microseconds."
        )
    }else if(cmd == "set" || cmd == "get"){
        if(cmd == "set"){
            PUSH_MSG2(
                COLORS_INFO,
                "set <parameter> <value>",
                " - Sets the value of a parameter."
            )
        }else if(cmd == "get"){
            PUSH_MSG2(
                COLORS_INFO,
                "get <parameter>",
                " - Gets the value of a parameter."
            )
        }
        PUSH_MSG1(
            COLORS_INFO[0],
            "Available parameters include:"
        )
        PUSH_MSG2(
            COLORS_INFO,
            "promptSize",
            " - How many characters can be typed in the CLI (min 32)."
        )
        PUSH_MSG2(
            COLORS_INFO,
            "consoleSize",
            " - How many lines for the console to remember (min 8)."
        )
        PUSH_MSG2(
            COLORS_INFO,
            "promptHistory",
            " - How many prompts to remember (min 1)."
        )
    }else if(cmd == "skip"){
        PUSH_MSG2(
            COLORS_INFO,
            "skip <type>",
            " - Skips the next ID for the type of object. Useful if an item is deleted, to skip the overhead of creating an item while retaining indices."
        )
    }else if(cmd == "sphere"){
        PUSH_MSG2(
            COLORS_INFO,
            "sphere {add | list}",
            " - Creates a sphere, or lists all spheres."
        )
        PUSH_MSG2(
            COLORS_INFO,
            "add <r> <x> <y> <z>: ",
            "The sphere has a radius of r and position of (x,y,z). The ID of the sphere will be printed."
        )
        PUSH_MSG2(
            COLORS_INFO,
            "list: ",
            "Lists all spheres by id, radius, and xyz position."
        )
    }
}
