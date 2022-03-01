#pragma once
#include <iostream>
#include <fmt/core.h>

struct rect2d { // объявлен в другой библиотеке
    float x, y, width, height;
    // hidden friend
    friend auto operator<<(std::ostream &s, rect2d const &r) noexcept
        -> std::ostream & {
        return s << fmt::format("rect2d({},{},{},{})", r.x, r.y, r.width,
                                r.height);
    }
};