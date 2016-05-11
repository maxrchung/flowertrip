#include "Config.hpp"

// Common values
Vector2 midpoint(320, 240);
const float PI = 3.1415927f;

// Paths
std::string scaleDataPath(R"(C:\Users\Wax Chug da Gwad\Desktop\flowertrip\FlowerTrip\MusicAnalysis\ScaleData.txt)");
std::string storyboardPath(R"(C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\281721 Expander - aura\Expander - aura (Midge).osb)");
std::string linePath("line.png");
std::string pointPath("point.png");

// Prism
// Distance from center
float prismBuffer = 50;
// Thickness of prism
float prismRadius = 5;
float prismScale = 1.0f;
float lineScaleHeight = 0.1f;
// Width of image
float lineWidth = 5000;
float pointScale = 0.04f;

float songEnd = 200000;