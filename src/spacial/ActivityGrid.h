/*
 * ActivityGrid.h
 *
 *  Created on: 26 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef ACTIVITYGRID_H_
#define ACTIVITYGRID_H_

#include <boost/multi_array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include "BoundingBox.h"
#include "Point.h"
#include "common/Maths.h"
#include <vector>

namespace cryomesh {

namespace spacial {

class ActivityGrid {

public:
	typedef boost::multi_array<double, 3> SpacialArray;
	typedef typename SpacialArray::index SpacialIndex;
	typedef SpacialArray::extent_gen SpacialExtent;
	typedef boost::tuple<int, int, int> Coordinates;

	ActivityGrid(const int x, const int y, const int z, const double sc = 1.0);

	virtual ~ActivityGrid() ;

	boost::shared_ptr<BoundingBoxElement> getBoundingBoxElement(Point element, Point reference) const ;

	const boost::shared_ptr<BoundingBox> getBoundingBox(Point reference, int depth =1 ) ;

	virtual double getInterpolatedActivity(const Point & point, int depth = 1, const BoundingBox::InterpolationStyle style = BoundingBox::InterpolationStyle::INVERSE_R) ;

	virtual void setNearestActivity(const Point & point, const double & activity);

	const double getGridPointActivity(const int x, const int y, const int z) const ;

	const std::map<Coordinates, double> & getActivityGrid() const;

	double getScale() const ;

	void setScale(double d);

	virtual void randomise(const double min = 0.0, const double max = 1.0) ;

	virtual void clearGrid(const double val = 0.0);

	virtual void reScale(const int x, const int y, const int z, const double sc) ;

	const Coordinates & getBoundingCoordinates() const ;

	double getActivityDecay() const ;

	void setActivityDecay(double d) ;

private:
	double activityDecay;
	Coordinates boundingCoordinates;
	double scale;
	const double MIN_SCALE;
	const double MAX_SCALE;
	std::map<Coordinates, double> activityGrid;
};

}

}

#endif /* ACTIVITYGRID_H_ */
