#include "Config.hpp"

// Common values
Vector2 midpoint(320, 240);
const float PI = 3.1415927f;

// Paths
std::string scaleDataPath(R"(C:\Users\Wax Chug da Gwad\Desktop\flowertrip\FlowerTrip\MusicAnalysis\ScaleData.txt)");
std::string storyboardPath(R"(C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\OSU_Final_Sketch!\Adi - Flower Trip (Osuuki).osb)");
std::string linePath("line.png");
std::string pointPath("point.png");

// Prism
// Distance from center
float prismBuffer = 50;
// Thickness of prism
float prismRadius = 7;
float prismScale = 2.5f;
float lineScaleHeight = 0.1f;
// Width of image
float lineWidth = 1000;
float prismRotY = PI / 45.0f;
float prismTurn = prismRotY;
std::vector<Color> prismColors = { Color(218, 42, 91),
								   Color(255, 152, 94),
								   Color(252, 218, 120),
								   Color(178, 212, 117),
								   Color(86, 200, 252),
								   Color(154, 38, 173) };

float offset = 1000;
float songStart = 1000;
float songStartOffset = songStart - offset;
float songEnd = 119999;
float songEndOffset = songEnd + offset;