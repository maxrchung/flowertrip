#include "Config.hpp"
#include "Spectrum.hpp"
#include <fstream>

Spectrum::Spectrum(const std::string& dataPath) {
	InitPrisms(dataPath);
}

void Spectrum::InitPrisms(const std::string& dataPath) {
	std::ifstream data(dataPath);
	data >> snapshotRate;

	int bandCount;
	data >> bandCount;

	int sampleCount;
	data >> sampleCount;
	
	float bandDivision = PI / (bandCount - 1);

	for (int i = 0; i < bandCount; ++i) {
		std::vector<float> scaleData;
		for (int j = 0; j < sampleCount; ++j) {
			float scale;
			data >> scale;

			scaleData.push_back(scale);
		}

		// Start on left
		Vector3 startPos(-prismBuffer);
		// Rotate to y-z position


		// Rotation around origin
		float prismRot = i * bandDivision;

		prisms.push_back(new Prism(scaleData));
	}
	
	data.close();
}