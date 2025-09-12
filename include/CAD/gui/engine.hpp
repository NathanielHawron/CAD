#pragma once

#include "CAD/general/engine.hpp"
#include "CAD/general/ringbuffer.hpp"

namespace CAD{
    namespace gui{
        class EngineGUI : public general::Engine{
        private:
            std::size_t promptSize;
            char *promptBuffer;
            general::RingBuffer<std::string> promptHistory;
        public:
            EngineGUI(std::string name, std::size_t promptSize = 255, std::size_t promptHistoryCount = 100):
                Engine(name),promptSize{promptSize},promptBuffer{new char[this->promptSize]},promptHistory{promptHistoryCount}{
                    memset(this->promptBuffer, '\0', this->promptSize);
                };
            ~EngineGUI(){delete[] this->promptBuffer;};
            void cliWindow() override;
        };
    }
}