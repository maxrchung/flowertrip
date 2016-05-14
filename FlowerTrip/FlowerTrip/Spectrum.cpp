#include "Config.hpp"
#include "Spectrum.hpp"
#include "Sprite.hpp"
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
	
	// Start on left
	Vector3 startPos(-prismBuffer);
	// Rotate to y-z position
	Vector3 cross = startPos.Cross(Vector3(0, -1, 0));
	// Start on bottom and rotate around
	float bandDivision = 2 * PI / bandCount;
	for (int i = 0; i < bandCount; ++i) {
		std::vector<float> scaleData;
		for (int j = 0; j < sampleCount; ++j) {
			float scale;
			data >> scale;
			scaleData.push_back(scale);
		}

		// Rotation around origin
		float prismRot = i * bandDivision;
		// Counter-clockwise
		Vector3 initPos = startPos.RotateAround(cross, -prismRot);

		instance->prisms.push_back(new Prism(scaleData, initPos));
	}
	
	data.close();
}

void Spectrum::ToSprite() {
	for (int i = 0; i < (int)prisms[0]->scaleData.size(); ++i) {
		Vector3 turnVec = prisms[1]->position.Cross(prisms[0]->position);

		for (int p = 0; p < prisms.size(); ++p) {
			Vector3 baseCenter = prisms[p]->position;
			Vector3 horzCross = turnVec;
			Vector3 horzVec = horzCross.Normalize() * prismRadius;
			Vector3 vertCross = horzCross.Cross(baseCenter);
			Vector3 vertVec = vertCross.Normalize() * prismRadius;

			// TopLeft, TopRight, BotRight, BotLeft
			std::vector<Vector3> baseVecs(corners, baseCenter);
			baseVecs[0] += vertVec - horzVec;
			baseVecs[1] += vertVec + horzVec;
			baseVecs[2] += -vertVec + horzVec;
			baseVecs[3] += -vertVec - horzVec;
			std::vector<Vector3> farVecs(corners);

			float len = prisms[p]->scaleData[i] * prismScale;
			Vector3 lenVec = baseCenter.Normalize() * len;
			float lineRot = Vector2(-1, 0).AngleBetween(Vector2(baseCenter));
			if (baseCenter.y < 0) {
				lineRot *= -1;
			}

			// Handles case when flipping from -180 to 180 or vice versa
			float startRot = prisms[p]->lineSprs[0]->rotation;
			float endRot = lineRot;
			if (fabs(startRot - endRot) > PI) {
				if (endRot < 0) {
					startRot -= PI * 2;
				}
				else {
					startRot += PI * 2;
				}
			}

			for (int j = 0; j < corners; ++j) {
				int endTime = i * Spectrum::Instance()->snapshotRate;
				int startTime;
				if (i == 0) {
					startTime = 0;
				}
				else {
					startTime = endTime - Spectrum::Instance()->snapshotRate;
				}

				Vector2 basePos = midpoint + Vector2(baseVecs[j].x, -baseVecs[j].y);
				Vector2 prevBasePos = prisms[p]->baseSprs[j]->position;
				prisms[p]->baseSprs[j]->Move(startTime, endTime, prevBasePos, basePos);

				farVecs[j] = baseVecs[j] + lenVec;
				Vector2 endPos = midpoint + Vector2(farVecs[j].x, -farVecs[j].y);
				prisms[p]->farSprs[j]->Move(startTime, endTime, prisms[p]->farSprs[j]->position, endPos);

				Vector2 lineDist = endPos - prisms[p]->baseSprs[j]->position;
				float lineLen = lineDist.Magnitude();
				float lineLenScale = lineLen / lineWidth;
				prisms[p]->lineSprs[j]->Move(startTime, endTime, prevBasePos, basePos);
				prisms[p]->lineSprs[j]->ScaleVector(startTime, endTime, prisms[p]->lineSprs[j]->scaleVector, Vector2(lineLenScale, prisms[p]->lineSprs[j]->scaleVector.y));
				prisms[p]->lineSprs[j]->Rotate(startTime, endTime, startRot, endRot);
			}
		}

		for (int p = 0; p < prisms.size(); ++p) {
			prisms[p]->position = prisms[p]->position.RotateAround(turnVec, -prismTurn);
		}

		for (int p = 0; p < prisms.size(); ++p) {
			prisms[p]->position = prisms[p]->position.RotateY(prismRotY);
		}
	}
}