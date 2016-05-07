#include "Config.hpp"
#include "Spectrum.hpp"
#include "Storyboard.hpp"
#include <iostream>
#include <string>

void main() {
	Spectrum spectrum(scaleDataPath);
	spectrum.ToSprite();

	Storyboard::Instance()->Write(storyboardPath);
}