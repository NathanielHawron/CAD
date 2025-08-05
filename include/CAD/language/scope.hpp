#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>

#include "CAD/language/object.hpp"

#include "CAD/general/vec.hpp"
#include "CAD/general/mat.hpp"
#include "CAD/geometry/sketch.hpp"

namespace CAD{
    namespace language{
        class Scope{
            std::shared_ptr<Scope> parent;

            std::unordered_map<std::size_t, Object<Scope, std::weak_ptr>> scopes;


            // "primatives"
            std::unordered_map<std::size_t, Object<double, std::shared_ptr>> floats;
            std::unordered_map<std::size_t, Object<int32_t, std::shared_ptr>> ints;
            std::unordered_map<std::size_t, Object<uint32_t, std::shared_ptr>> uints;
            std::unordered_map<std::size_t, Object<uint64_t, std::shared_ptr>> indices;
            std::unordered_map<std::size_t, Object<std::string, std::shared_ptr>> strings;
            std::unordered_map<std::size_t, Object<general::vec2, std::shared_ptr>> vec2s;
            std::unordered_map<std::size_t, Object<general::vec3, std::shared_ptr>> vec3s;
            // mat2
            // mat3
            // mat4
            std::unordered_map<std::size_t, Object<geometry::Sketch, std::shared_ptr>> sketches;
            // surfaces
            // volume
        };
    }
}