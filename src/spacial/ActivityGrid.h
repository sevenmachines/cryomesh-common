/*
 * ActivityGrid.h
 *
 *  Created on: 26 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef ACTIVITYGRID_H_
#define ACTIVITYGRID_H_

#include <boost/multi_array.hpp>
#include "Point.h"

namespace cryomesh {

namespace spacial {

template<class T>

class ActivityGrid {

	typedef boost::multi_array<T, 3> SpacialArray;
	typedef typename SpacialArray::index SpacialIndex;
public:
	ActivityGrid(const int x, const int y, const int z, const double sc = 1.0) :
		gridX(x), gridY(y), gridZ(z), scale(sc), MIN_SCALE(1.0 / 1000.0), MAX_SCALE(1000) {
		SpacialArray temp_grid(boost::extents[x][y][z]);
		activityGrid = temp_grid;
	}

	virtual ~ActivityGrid() {

	}

	virtual T getInterpolatedActivity(const Point & point) =0;
	virtual void setNearestActivity(const Point & point, const T & activity) =0;

	const T & getGridPoint(const int x, const int y, const int z) const {
		return activityGrid[x][y][z];
	}
	const T & getScaledGridPoint(const int x, const int y, const int z) const {
		return activityGrid[x][y][z];
	}
	const SpacialArray & getActivityGrid() const {
		return activityGrid;
	}

	void setGridSize(const int x, const int y, const int z) {
		this->reScale(x, y, z, this->scale);
	}
	double getScale() const {
		return scale;
	}
	void setScale(double d) {
		double clamped_scale = std::max(MIN_SCALE, d);
		clamped_scale = std::min(MAX_SCALE, d);
		this->reScale(gridX, gridY, gridZ, clamped_scale);
	}

protected:
	virtual void clearGrid(const T & blank_value) {
		//activityGrid.reindex(blank_value);
	}
	virtual void reScale(const int x, const int y, const int z, const double sc) {
		SpacialArray temp_grid(boost::extents[x][y][z]);
		for (SpacialIndex i = 0; i < x; i++) {
			for (SpacialIndex j = 0; i < y; i++) {
				for (SpacialIndex k = 0; i < z; i++) {
					double scaled_x = i * sc;
					double scaled_y = j * sc;
					double scaled_z = k * sc;
					T interpolated_act = this->getInterpolatedActivity(Point(scaled_x, scaled_y, scaled_z));
					temp_grid[i][j][k] = interpolated_act;
				}
			}
		}
		activityGrid = temp_grid;
		gridX = x;
		gridY = y;
		gridZ = z;
		scale = sc;
	}

protected:
	int gridX;
	int gridY;
	int gridZ;
	double scale;
	const double MIN_SCALE;
	const double MAX_SCALE;
	SpacialArray activityGrid;
};

}

}

#endif /* ACTIVITYGRID_H_ */
