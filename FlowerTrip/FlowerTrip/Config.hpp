#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Color.hpp"
#include "Vector2.hpp"
#include <string>

extern Vector2 midpoint;
extern const float PI;

extern std::string scaleDataPath;
extern std::string storyboardPath;
extern std::string linePath;
extern std::string pointPath;

extern float prismBuffer;
extern float prismRadius;
extern float prismScale;
extern float lineScaleHeight;
extern float lineWidth;
extern float prismRotY;
extern float prismTurn;
extern std::vector<Color> prismColors;

extern float offset;
extern float songStart;
extern float songStartOffset;
extern float songEnd;
extern float songEndOffset;

#endif//CONFIG_HPP