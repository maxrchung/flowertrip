#include "Config.hpp"
#include "Spectrum.hpp"
#include "Storyboard.hpp"
#include <iostream>
#include <string>

void main() {
	Spectrum spectrum(scaleDataPath);

	Storyboard::Instance()->Write(storyboardPath);
}