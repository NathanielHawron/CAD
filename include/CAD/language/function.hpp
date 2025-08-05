#pragma once

#include <vector>

#include "CAD/general/vec.hpp"
#include "CAD/general/mat.hpp"

namespace CAD{
    namespace language{
        class Sketch{
            std::vector<general::vec2> points;
        };
    }
}