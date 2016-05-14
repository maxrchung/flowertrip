#include "Config.hpp"
#include "Prism.hpp"
#include "Spectrum.hpp"

Prism::Prism(const std::vector<float>& scaleData, Vector3 initPosition)
	: scaleData(scaleData), position(initPosition) {
	for (int i = 0; i < Spectrum::Instance()->corners; ++i) {
		// 3D calculations are all done with 0,0,0 set as the center
		// Need to account for midpoint
		// osu! coordinate system starts on top left corner
		baseSprs.push_back(new Sprite(pointPath));
		baseSprs[i]->Scale(0, songEnd, pointScale, pointScale);

		farSprs.push_back(new Sprite(pointPath));
		farSprs[i]->Scale(0, songEnd, pointScale, pointScale);

		lineSprs.push_back(new Sprite(linePath, Vector2(320, 240), Layer::Foreground, Origin::CentreRight));
		lineSprs[i]->ScaleVector(0, 0, Vector2(0, lineScaleHeight), Vector2(0, lineScaleHeight));
	}
}