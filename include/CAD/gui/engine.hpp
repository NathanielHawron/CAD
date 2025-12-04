#pragma once

#include "CAD/general/engine.hpp"

namespace CAD{
    namespace gui{
        class EngineGUI : public general::Engine{ 
        public:
            int width, height;
            std::array<NRA::VGL::ControlBind, 17> controls;
            NRA::VGL::Shader &shader;
        private:
            std::string id;
            bool rename = false;
            int renameVPIndex = -1;
            char renameBuff[64];
        public:
            EngineGUI(std::string name, std::string id, int width, int height, std::array<NRA::VGL::ControlBind, 17> controls, NRA::VGL::Shader &shader, std::size_t promptSize = 256, std::size_t consoleSize = 100, std::size_t promptHistoryCount = 50, std::size_t subdivisions = 10);
            ~EngineGUI(){};
            void viewportWindow(general::Viewport &vp) override;
            static void aboutWindow();
            void renderWindowMenu() override;
            void cliWindow() override;
        };
    }
}