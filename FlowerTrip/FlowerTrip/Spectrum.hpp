#ifndef SPECTRUM_HPP
#define SPECTRUM_HPP

#include "Prism.hpp"
#include <string>
#include <vector>

class Spectrum {
public:
	static Spectrum* Instance();
	static void InitPrisms(const std::string& dataPath);
	void ToSprite();

	int snapshotRate;
	int corners = 4;
private:
	void lineRotAdjust(Vector3 baseCenter, float& startRot, float& endRot);
	Spectrum() {};
	Spectrum(const Spectrum&) {};
	Spectrum& operator=(const Spectrum&) {};

	static Spectrum* instance;
	std::vector<Prism*> prisms;
};

#endif//SPECTRUM_HPP