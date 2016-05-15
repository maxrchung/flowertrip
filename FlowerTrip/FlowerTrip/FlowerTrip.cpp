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
	bg->ScaleVector(songStartOffset, songEndOffset, Vector2(1.366f, 0.768f), Vector2(1.366f, 0.768f));
	bg->Color(songStartOffset, songEndOffset, Color(0), Color(0));
	bg->Fade(songStartOffset, songStart, 0, 1.0f);
	bg->Fade(songEnd, songEndOffset, 1.0f, 0);

	Storyboard::Instance()->Write(storyboardPath);
}