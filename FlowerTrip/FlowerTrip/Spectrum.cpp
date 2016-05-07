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
	
	Vector3 s;
	Vector3 o;

	float bandDivision = 2 * PI / (bandCount - 1);

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
		Vector3 posRotY = startPos.RotateY(PI / 4);
		// Rotation around origin
		float prismRot = i * bandDivision;

		Vector3 cross = posRotY.Cross(Vector3(0, -1, 0));
		Vector3 initPos = posRotY.RotateAround(cross, prismRot);

		prisms.push_back(new Prism(scaleData, initPos));
	}
	
	data.close();
}

void Spectrum::ToSprite() {
	for (auto prism : prisms) {
		prism->ToSprite();
	}
}