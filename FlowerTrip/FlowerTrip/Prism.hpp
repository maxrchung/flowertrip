#ifndef PRISM_HPP
#define PRISM_HPP

#include "Config.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include <string>
#include <vector>

class Prism {
public:
	Prism(std::vector<float> scaleData, Vector3 initPosition);
	void ToSprite();
	Vector3 position;
private:
	std::vector<float> scaleData;
};

#endif//PRISM_HPP