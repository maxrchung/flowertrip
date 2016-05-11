#include "Config.hpp"
#include "Spectrum.hpp"
#include "Storyboard.hpp"
#include <iostream>
#include <string>

void main() {
	std::cout << "Initializing prisms" << std::endl;
	Spectrum::InitPrisms(scaleDataPath);
	std::cout << "Computing prisms ToSprite()" << std::endl;
	Spectrum::Instance()->ToSprite();

	Sprite* bg = new Sprite("blank.png", Vector2(320.0f, 240.0f), Layer::Background);
	bg->ScaleVector(0, songEnd, Vector2(1.366f, 0.768f), Vector2(1.366f, 0.768f));
	bg->Color(0, songEnd, Color(0), Color(0));

	Storyboard::Instance()->Write(storyboardPath);
}