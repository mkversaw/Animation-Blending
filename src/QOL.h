#pragma once

#include <glm/glm.hpp>
#include <ostream>

inline std::ostream& operator<< (std::ostream& out, const glm::vec3& vec) {
    out << "{"
        << vec.x << " " << vec.y << " " << vec.z
        << "}";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::vec4& vec) {
    out << "{"
        << vec.x << " " << vec.y << " " << vec.z << " " << vec.w
        << "}";

    return out;
}