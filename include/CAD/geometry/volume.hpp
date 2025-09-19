#pragma once

#include <memory>


namespace CAD{
    namespace geometry{
        struct Volume{
            ~Volume() = default;
            virtual std::unique_ptr<Volume> uniqueClone() const = 0;
        };
    }
}