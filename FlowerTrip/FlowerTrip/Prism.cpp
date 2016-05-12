#include "Config.hpp"
#include "Prism.hpp"
#include "Storyboard.hpp"
#include "Spectrum.hpp"
#include <algorithm>

Prism::Prism(const std::vector<float>&	 scaleData, Vector3 initPosition)
	: scaleData(scaleData), position(initPosition) {

}

void Prism::ToSprite() {
	Vector3 baseCenter = position;
	Vector3 horzCross = baseCenter.Cross(Vector3(0, 1, 0));
	Vector3 horzVec = horzCross.Normalize() * prismRadius;
	Vector3 vertCross = horzCross.Cross(baseCenter);
	Vector3 vertVec = vertCross.Normalize() * prismRadius;

	int corners = 4;
	// TopLeft, TopRight, BotLeft, BotRight
	std::vector<Vector3> baseVecs(corners, baseCenter);
	baseVecs[0] += vertVec - horzVec;
	baseVecs[1] += vertVec + horzVec;
	baseVecs[2] += -vertVec - horzVec;
	baseVecs[3] += -vertVec + horzVec;

	// Sprite setup
	std::vector<Sprite*> baseSprs(corners);
	std::vector<Sprite*> farSprs(corners);
	std::vector<Sprite*> lineSprs(corners);
	float lineRot = Vector2(-1, 0).AngleBetween(Vector2(baseCenter));
	if (baseCenter.y > 0) {
		lineRot *= -1;
	}
	for (int i = 0; i < corners; ++i) {
		// 3D calculations are all done with 0,0,0 set as the center
		// Need to account for midpoint
		baseSprs[i] = new Sprite(pointPath, midpoint + Vector2(baseVecs[i]));
		baseSprs[i]->Scale(0, songEnd, pointScale, pointScale);

		farSprs[i] = new Sprite(pointPath, baseSprs[i]->position);
		farSprs[i]->Scale(0, songEnd, pointScale, pointScale);

		lineSprs[i] = new Sprite(linePath, baseSprs[i]->position, Layer::Foreground, Origin::CentreRight);
		lineSprs[i]->ScaleVector(0, 0, Vector2(0, lineScaleHeight), Vector2(0, lineScaleHeight));
		lineSprs[i]->Rotate(0, songEnd, lineRot, lineRot);
	}

	for (int i = 0; i < (int)scaleData.size(); ++i) {
		float len = scaleData[i] * prismScale;
		Vector3 lenVec = baseCenter.Normalize() * len;

		std::vector<Vector3> farVecs(corners);
		for (int j = 0; j < corners; ++j) {
			farVecs[j] = baseVecs[j] + lenVec;
		}

		// Handle projection here later
		for (int j = 0; j < corners; ++j) {
			int endTime = i * Spectrum::Instance()->snapshotRate;
			int startTime;
			if (i == 0) {
				startTime = 0;
			}
			else {
				startTime = endTime - Spectrum::Instance()->snapshotRate;
			}
			Vector2 screenPos = midpoint + Vector2(farVecs[j]);
			Vector2 lineDist = screenPos - baseSprs[j]->position;
			float lineLen = lineDist.Magnitude();
			float lineLenScale = lineLen / lineWidth;
			lineSprs[j]->ScaleVector(startTime, endTime, lineSprs[j]->scaleVector, Vector2(lineLenScale, lineSprs[j]->scaleVector.y));

			farSprs[j]->Move(startTime, endTime, farSprs[j]->position, screenPos);
		}
	}
}