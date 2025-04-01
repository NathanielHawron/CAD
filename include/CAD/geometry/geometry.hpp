#pragma once

#include <vector>

namespace CAD{
    namespace geometry{
        class Geometry{
            public:
                virtual bool intersects(const Geometry &g) const;
        };
    }
};