#pragma once

#include "CAD/general/engine.hpp"

namespace CAD{
    namespace gui{
        class EngineGUI : public general::Engine{            
        public:
            EngineGUI(std::string name, std::size_t promptSize = 255, std::size_t promptHistoryCount = 100):Engine(name,promptSize,promptHistoryCount){};
            ~EngineGUI(){};
            void aboutWindow() override;
            void cliWindow() override;
        };
    }
}