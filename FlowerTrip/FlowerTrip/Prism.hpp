#ifndef PRISM_HPP
#define PRISM_HPP

#include "Vector2.hpp"
#include "Vector3.hpp"
#include <string>
#include <vector>

class Prism {
public:
	Prism(const std::vector<float>& scaleData, Vector3 initPosition);
	Vector3 position;
	std::vector<float> scaleData;
private:
};

#endif//PRISM_HPP