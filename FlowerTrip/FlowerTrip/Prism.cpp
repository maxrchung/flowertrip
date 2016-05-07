#include "Config.hpp"
#include "Prism.hpp"
#include "Storyboard.hpp"

Prism::Prism(std::vector<float> scaleData, Vector3 initPosition)
	: scaleData(scaleData), position(initPosition) {

}

void Prism::ToSprite() {
	Vector3 baseCenter = position;
	Vector3 horzCross = baseCenter.Cross(Vector3(0, 1, 0));
	Vector3 horzVec = horzCross.Normalize() * prismRadius;
	Vector3 vertCross = baseCenter.Cross(horzCross);
	Vector3 vertVec = vertCross.Normalize() * prismRadius;

	Vector3 baseTL = position + vertVec - horzVec;
	Vector3 baseTR = position + vertVec + horzVec;
	Vector3 baseBL = position - vertVec - horzVec;
	Vector3 baseBR = position - vertVec + horzVec;

	Sprite* baseTLSpr = new Sprite(pointPath, midpoint + Vector2(baseTL));

	for (auto scale : scaleData) {
		if (scale > 0) {
			float len = scale * prismScale;
			Vector3 lenVec = position.Normalize() * len;

			Vector3 farTL = baseTL + lenVec;
			Vector3 farTR = baseTR + lenVec;
			Vector3 farBL = baseBL + lenVec;
			Vector3 farBR = baseBR + lenVec;
		}
	}
}