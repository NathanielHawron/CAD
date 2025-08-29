#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <thread>
#include <memory>

#include "CAD/language/instruction.hpp"
#include "CAD/language/object.hpp"
#include "CAD/language/scope.hpp"

namespace CAD{
    namespace language{
        class Program{
        public:
            static constexpr uint32_t FLAG_COMPILER_NONE            = 0;
            static constexpr uint32_t FLAG_COMPILER_TOKENIZE_ONLY   = 1 << 0;
        public:
            struct CompileInfo{
            public:
                std::string file;
                bool readFile(std::filesystem::path filePath);
                uint32_t flags;
                // Log to vector
                std::vector<std::string> *info = nullptr;
                std::vector<std::string> *warnings = nullptr;
                std::vector<std::string> *errors = nullptr;
                std::vector<std::string> *tokens = nullptr;
                // Log to file
                std::filesystem::path parentPath = std::filesystem::current_path();
                std::filesystem::path *infoPath = nullptr;
                std::filesystem::path *warningsPath = nullptr;
                std::filesystem::path *errorsPath = nullptr;
                std::filesystem::path *tokensPath = nullptr;
            };
        private:
            std::vector<language::Instruction> instructions;
            std::vector<std::shared_ptr<language::Scope>> scopes;
            bool valid;
        public:
            bool compile(CompileInfo info);
            bool isValid(){return this->valid;};
        };
    }
}