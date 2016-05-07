#include "Config.hpp"

// Common values
Vector2 midpoint(320, 240);
const float PI = 3.1415927;

// Paths
std::string scaleDataPath(R"(C:\Users\Wax Chug da Gwad\Desktop\flowertrip\FlowerTrip\MusicAnalysis\ScaleData.txt)");
std::string storyboardPath(R"(C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\281721 Expander - aura\Expander - aura (Midge).osb)");
std::string linePath("line.png");
std::string pointPath("point.png");

// Prism
// Distance from center
float prismBuffer = 50.0f;
// Thickness of prism
float prismRadius = 5.0f;
float prismScale = 0.25f;
float lineScaleHeight = 0.2f;
float pointScale = 0.1f;