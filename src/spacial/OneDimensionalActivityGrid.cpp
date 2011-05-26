/*
 * OneDimensionalActivityGrid.cpp
 *
 *  Created on: 26 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "OneDimensionalActivityGrid.h"

namespace cryomesh {

namespace spacial {

OneDimensionalActivityGrid::OneDimensionalActivityGrid(const int x, const int y, const int z) :
	ActivityGrid(x, y, z) {

}

OneDimensionalActivityGrid::~OneDimensionalActivityGrid() {
	// TODO Auto-generated destructor stub
}

double OneDimensionalActivityGrid::getInterpolatedActivity(const Point & point) {
	return 0.0;
}
void OneDimensionalActivityGrid::setNearestActivity(const Point & point, const double & activity){
	// get closes point and point to it
}

}//NAMESPACE

}//NAMESPACE
