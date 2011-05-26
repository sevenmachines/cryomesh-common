/*
 * OneDimensionalActivityGrid.h
 *
 *  Created on: 26 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef ONEDIMENSIONALACTIVITYGRID_H_
#define ONEDIMENSIONALACTIVITYGRID_H_

#include "ActivityGrid.h"
namespace cryomesh {

namespace spacial {

class OneDimensionalActivityGrid : public ActivityGrid<double>{
public:
	OneDimensionalActivityGrid(const int x, const int y, const int z);
	virtual ~OneDimensionalActivityGrid();
	virtual double getInterpolatedActivity(const Point & point);
	virtual void setNearestActivity(const Point & point, const double & activity);
};

}

}

#endif /* ONEDIMENSIONALACTIVITYGRID_H_ */
