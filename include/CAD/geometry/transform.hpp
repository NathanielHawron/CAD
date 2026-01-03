#pragma once

#include "glm/glm.hpp"

namespace CAD{
    namespace geometry{
        struct Transform{
            glm::mat4 m = glm::mat4(1.0f);
        };
    }
}