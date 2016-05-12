#include "Config.hpp"
#include "Spectrum.hpp"
#include <fstream>

Spectrum* Spectrum::instance = NULL;

Spectrum* Spectrum::Instance() {
	if (!instance) {
		throw "Spectrum not initialized yet";
	}
	return instance;
}

void Spectrum::InitPrisms(const std::string& dataPath) {
	instance = new Spectrum;

	std::ifstream data(dataPath);
	data >> instance->snapshotRate;

	int bandCount;
	data >> bandCount;

	int sampleCount;
	data >> sampleCount;
	
	float bandDivision = 2 * PI / bandCount;
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
		Vector3 posRotY = startPos; //.RotateY(-PI / 4);
		// Rotation around origin
		float prismRot = i * bandDivision;
		Vector3 cross = posRotY.Cross(Vector3(0, -1, 0));
		Vector3 initPos = posRotY.RotateAround(cross, -prismRot);

		instance->prisms.push_back(new Prism(scaleData, initPos));
	}
	
	data.close();
}

void Spectrum::ToSprite() {
	for (auto prism : prisms) {
		prism->ToSprite();
	}
}