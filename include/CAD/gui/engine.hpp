#pragma once

#include "CAD/general/engine.hpp"

namespace CAD{
    namespace gui{
        class EngineGUI : public general::Engine{ 
        public:
            int width, height;
            std::array<NRA::VGL::ControlBind, 17> controls;
        private:
            std::string id;
            std::size_t viewportID = 0;
            bool rename = false;
            int renameVPIndex = -1;
            char renameBuff[64];
        public:
            EngineGUI(std::string name, std::string id, int width, int height, std::array<NRA::VGL::ControlBind, 17> controls, std::size_t promptSize = 255, std::size_t promptHistoryCount = 100);
            ~EngineGUI(){};
            void viewportWindow(general::Viewport &vp) override;
            static void aboutWindow();
            void renderWindowMenu() override;
            void cliWindow() override;
        };
    }
}