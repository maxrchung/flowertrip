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
	// Handle coloring
	for (int p = 0; p < prisms.size(); ++p) {
		float fadeFrac = (prisms.size() - p) / (float) prisms.size();
		float fade = fadeFrac * 0.75f + 0.25f;
		for (int j = 0; j < corners; ++j) {
			prisms[p]->baseSprs[j]->Fade(0, songEnd, fade, fade);
			prisms[p]->farSprs[j]->Fade(0, songEnd, fade, fade);
			prisms[p]->lineSprs[j]->Fade(0, songEnd, fade, fade);
		}
	}

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

			float startLineRot = prisms[p]->lineSprs[0]->rotation;
			float endLineRot;
			lineRotAdjust(baseCenter, startLineRot, endLineRot);

			// Timing
			int endTime = i * Spectrum::Instance()->snapshotRate;
			int startTime;
			if (i == 0) {
				startTime = 0;
			}
			else {
				startTime = endTime - Spectrum::Instance()->snapshotRate;
			}

			// Screen positions
			std::vector<Vector2> basePts(corners, midpoint);
			std::vector<Vector2> farPts(corners, midpoint);

			// Handle lines
			for (int j = 0; j < corners; ++j) {
				basePts[j] += Vector2(baseVecs[j].x, -baseVecs[j].y);
				farVecs[j] = baseVecs[j] + lenVec;
				farPts[j] += Vector2(farVecs[j].x, -farVecs[j].y);

				Vector2 lineDist = farPts[j] - basePts[j];
				float lineLen = lineDist.Magnitude();
				float lineLenScale = lineLen / lineWidth;
				prisms[p]->lineSprs[j]->Move(startTime, endTime, prisms[p]->lineSprs[j]->position, basePts[j]);
				prisms[p]->lineSprs[j]->ScaleVector(startTime, endTime, prisms[p]->lineSprs[j]->scaleVector, Vector2(lineLenScale, prisms[p]->lineSprs[j]->scaleVector.y));
				prisms[p]->lineSprs[j]->Rotate(startTime, endTime, startLineRot, endLineRot);
			}

			// Handle faces after
			for (int j = 0; j < corners; ++j) {
				Vector3 leftVec = baseVecs[j];
				Vector3 rightVec = baseVecs[(j + 1) % corners];
				Vector2 edge = Vector2(leftVec) - Vector2(rightVec);

				Vector2 basePt = midpoint + Vector2(rightVec.x, -rightVec.y);
				Vector3 farVec = farVecs[(j + 1) % corners];
				Vector2 farPt = midpoint + Vector2(farVec.x, -farVec.y);

				float edgeLen = edge.Magnitude();
				float edgeLenScale = edgeLen / lineWidth;
				prisms[p]->baseSprs[j]->Move(startTime, endTime, prisms[p]->baseSprs[j]->position, basePt);
				prisms[p]->baseSprs[j]->ScaleVector(startTime, endTime, prisms[p]->baseSprs[j]->scaleVector, Vector2(edgeLenScale, prisms[p]->baseSprs[j]->scaleVector.y));
				prisms[p]->farSprs[j]->Move(startTime, endTime, prisms[p]->farSprs[j]->position, farPt);
				prisms[p]->farSprs[j]->ScaleVector(startTime, endTime, prisms[p]->farSprs[j]->scaleVector, Vector2(edgeLenScale, prisms[p]->farSprs[j]->scaleVector.y));

				if (edge.Magnitude() > 0) {
					float startEdgeRot = prisms[p]->baseSprs[j]->rotation;
					float endEdgeRot;
					lineRotAdjust(edge, startEdgeRot, endEdgeRot);
					prisms[p]->baseSprs[j]->Rotate(startTime, endTime, startEdgeRot, endEdgeRot);
					prisms[p]->farSprs[j]->Rotate(startTime, endTime, startEdgeRot, endEdgeRot);
				}
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

// Handles case when flipping from -180 to 180 or vice versa
void Spectrum::lineRotAdjust(Vector3 baseCenter, float& startRot, float& endRot) {
	endRot = Vector2(-1, 0).AngleBetween(Vector2(baseCenter));
	if (baseCenter.y < 0) {
		endRot *= -1;
	}

	if (fabs(startRot - endRot) > PI) {
		if (endRot < 0) {
			startRot -= PI * 2;
		}
		else {
			startRot += PI * 2;
		}
	}
}