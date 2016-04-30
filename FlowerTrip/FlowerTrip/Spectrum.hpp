#ifndef SPECTRUM_HPP
#define SPECTRUM_HPP

#include "Prism.hpp"
#include <string>
#include <vector>

class Spectrum {
public:
	Spectrum(const std::string& dataPath);
private:
	void InitPrisms(const std::string& dataPath);
	std::vector<Prism*> prisms;
	int snapshotRate;
};

#endif//SPECTRUM_HPP