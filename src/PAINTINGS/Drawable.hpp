#ifndef CP_2025_DRAWABLE_H
#define CP_2025_DRAWABLE_H
#include <string>
#include "../../json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

class Drawable {
public:
    virtual json toJson() const = 0;
    virtual ~Drawable() {}
};

#endif  // CP_2025_DRAWABLE_H