#pragma once

#include "splinekit/WeightManager.hpp"

namespace CAD{
    namespace general{
        template <class T, class WM>
        class Interpolator{
            WM weightManager;
            T calculateLength();
            void calculatePoints();
        };
    }
}