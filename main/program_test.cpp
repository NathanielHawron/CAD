#include <iostream>
#include <memory>
#include <assert.h>
#include <vector>
#include <string>

#include "CAD/general/version.hpp"
#include "CAD/geometry/sketch.hpp"
#include "CAD/language/object.hpp"
#include "CAD/language/program.hpp"

const CAD::Version testVersion{0,0,0,'a'};

void testProgram(std::filesystem::path filePath, std::string fileName){
    std::vector<std::string> info;
    std::vector<std::string> warnings;
    std::vector<std::string> errors;
    std::vector<std::string> tokens;

    std::filesystem::path infoPath = "info_"+fileName+".log";
    std::filesystem::path warningPath = "warning_"+fileName+".log";
    std::filesystem::path errorPath = "error_"+fileName+".log";
    std::filesystem::path tokenPath = "token_"+fileName+".log";

    CAD::language::Program::CompileInfo compileInfo;
    compileInfo.readFile(filePath);
    compileInfo.info = &info;
    compileInfo.warnings = &warnings;
    compileInfo.errors = &errors;
    compileInfo.parentPath = std::filesystem::current_path() / "saves" / "program_test" / "logs";
    compileInfo.infoPath = &infoPath;
    compileInfo.warningsPath = &warningPath;
    compileInfo.errorsPath = &errorPath;
    compileInfo.tokensPath = &tokenPath;

    CAD::language::Program program;
    std::cout << (program.compile(compileInfo) ? "Compilation Successful" : "Compilation Failed") << std::endl;
    std::cout << "Info: " << info.size() << std::endl;
    for(auto i : info){
        std::cout << i << std::endl;
    }
    std::cout << "Warnings:" << warnings.size() << std::endl;
    for(auto w : warnings){
        std::cout << w << std::endl;
    }
    std::cout << "Errors:" << errors.size() << std::endl;
    for(auto e : errors){
        std::cout << e << std::endl;
    }
}


int main(){
    std::cout << testVersion.toString() << std::endl;
    std::cout << CAD::LIB_VERSION.toString() << " (lib) : " << CAD::HEADER_VERSION.toString() << " (header) / v0.0.2-a (tested)" << std::endl;

    // testProgram(std::filesystem::current_path() / "saves" / "mismatched_brackets" / "mismatched_open_bracket.OOCAD", "open");
    // testProgram(std::filesystem::current_path() / "saves" / "mismatched_brackets" / "mismatched_closed_bracket.OOCAD", "closed");
    // testProgram(std::filesystem::current_path() / "saves" / "mismatched_brackets" / "mismatched_type_bracket.OOCAD", "type");
    testProgram(std::filesystem::current_path() / "saves" / "program_test" / "test_program.OOCAD", "testProgram");
}